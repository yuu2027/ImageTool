#include "NameGenerator.h"
#include "Config.h"
#include "Logger.h"
#include <filesystem>
#include <sstream> // 文字列を入出力の対象として扱うためのライブラリ
#include <iomanip>

namespace fs = std::filesystem;

namespace {
    // 文字列を小文字に変換して返す関数
    std::string toLowerCopy(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }

    // ユーザーが指定した形式文字列を、実際の拡張子表記に正規化する関数
    std::string normalizeFormatExtension(const std::string& format)
    {
        std::string f = toLowerCopy(format);

        if (f.empty()) return "";
        if (f == "jpg" || f == "jpeg") return ".jpg";
        if (f == "png") return ".png";
        if (f == "bmp") return ".bmp";
        if (f == "eps") return ".eps";

        return "";
    }
}

// コンストラクタ
// config からリネーム方法やプレフィックスを受け取り、
// outputDir は出力先フォルダとして保存する
NameGenerator::NameGenerator(const Config& config, const std::filesystem::path& outputDir)
    : outputDir_(outputDir),        // 出力フォルダを保存 
    renameMode_(config.renameMode), // keep / seq などの命名モードを保存
    prefix_(config.prefix),         // keep モード時などに使うプレフィックスを保存
    sequence_(1),                   // 連番の開始値を 1 にする
    format_(config.format)
{
}

// 入力ファイルから、出力先のファイルパスを作る関数
std::filesystem::path NameGenerator::generate(const std::filesystem::path& inputFile)
{
    std::string fileName;

    if (renameMode_ == RenameMode::Keep) {
        fileName = makeKeepName(inputFile);
        fileName = avoidCollision(fileName);
    }
    else {
        fileName = makeSeqName(inputFile);
    }

    return outputDir_ / fileName;
}

std::string NameGenerator::makeKeepName(const std::filesystem::path& inputFile) const
{
    std::string stem = inputFile.stem().string(); // 拡張子を除いた本体部分
    std::string ext = getOutputExtension(inputFile); // ファイルの拡張子
    return prefix_ + stem + ext;
}

std::string NameGenerator::makeSeqName(const std::filesystem::path& inputFile)
{
    std::string ext = getOutputExtension(inputFile); // ファイルの拡張子

    while (true) {
        std::ostringstream oss;
        // setw(4)：幅を４に指定
        // setfill('0')：足りない部分を0で埋める
        oss << std::setw(4) << std::setfill('0') << sequence_;
        std::string candidate = oss.str() + ext;
        ++sequence_;

        if (reservedNames_.count(candidate) == 0 && !std::filesystem::exists(outputDir_ / candidate)) {
            reservedNames_.insert(candidate);
            return candidate;
        }

    }
}

// 同じ名前があるとき回避
std::string NameGenerator::avoidCollision(const std::string& fileName)
{
    std::filesystem::path p(fileName);
    std::string stem = p.stem().string(); // 拡張子を除いた部分
    std::string ext = p.extension().string(); // 拡張子

    std::string candidate = fileName;
    int counter = 1;

    // reservedNames_.count(candidate)：その値があれば1無ければ0
    // 
    while (reservedNames_.count(candidate) > 0 || std::filesystem::exists(outputDir_ / candidate)) {
        candidate = stem + "_" + std::to_string(counter) + ext;
        ++counter;
    }

    reservedNames_.insert(candidate);
    return candidate;
}

// 拡張子を変換
std::string NameGenerator::getOutputExtension(const fs::path& inputFile) const
{
    std::string forcedExt = normalizeFormatExtension(format_);
    if (!forcedExt.empty()) {
        return forcedExt;
    }

    std::string inputExt = inputFile.extension().string(); // 拡張子
    // 小文字に変換
    return toLowerCopy(inputExt);
}