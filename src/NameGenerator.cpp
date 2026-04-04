#include "NameGenerator.h"
#include <filesystem>
#include <sstream> // 文字列を入出力の対象として扱うためのライブラリ

namespace fs = std::filesystem;

// コンストラクタ
// config からリネーム方法やプレフィックスを受け取り、
// outputDir は出力先フォルダとして保存する
NameGenerator::NameGenerator(const Config& config, const std::filesystem::path& outputDir)
    : outputDir_(outputDir),        // 出力フォルダを保存 
    renameMode_(config.renameMode), // keep / seq などの命名モードを保存
    prefix_(config.prefix),         // keep モード時などに使うプレフィックスを保存
    sequence_(1)                    // 連番の開始値を 1 にする
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
    std::string ext = inputFile.extension().string(); // ファイルの拡張子
    return prefix_ + stem + ext;
}

std::string NameGenerator::makeSeqName(const std::filesystem::path& inputFile)
{
    std::string ext = inputFile.extension().string();

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