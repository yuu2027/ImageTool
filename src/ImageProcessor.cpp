#include "ImageProcessor.h"
#include "Logger.h"
#include <filesystem>
#include <opencv2/imgcodecs.hpp>

namespace fs = std::filesystem;

// 入力ファイルを出力先へコピー
bool ImageProcessor::file_copy(const fs::path& inputPath, const fs::path& outputPath)
{
    try {
        // overwrite_existing：既存のファイルがあっても上書きする
        fs::copy_file(inputPath, outputPath, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (...) { // 例外処理．(...)：どんな種類の例外でも受け取る
        return false;
    }
}

// 画像の読み込み
cv::Mat ImageProcessor::load(const fs::path& inputPath) const
{
    // imread：画像ファイルを読み込む関数
    // cv::IMREAD_UNCHANGED：元画像の情報をできるだけそのまま保って読む
    cv::Mat image = cv::imread(inputPath.string(), cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        Logger::error("画像を読み込めませんでした: " + inputPath.string());
    }
    else {
        Logger::info("画像を読み込みました: " + inputPath.string());
    }

    return image;
}

// 画像の保存
bool ImageProcessor::save(const fs::path& outputPath, const cv::Mat& image) const
{
    if (image.empty()) {
        Logger::error("保存対象の画像が空です");
        return false;
    }

    // parent_path：親フォルダ部分を取り出す
    fs::path parent = outputPath.parent_path();
    if (!parent.empty() && !fs::exists(parent)) {
        fs::create_directories(parent);
    }

    // imwrite：画像をファイルとして保存する関数
    bool ok = cv::imwrite(outputPath.string(), image);

    if (!ok) {
        Logger::error("画像を保存できませんでした: " + outputPath.string());
        return false;
    }

    Logger::info("画像を保存しました: " + outputPath.string());
    return true;
}

cv::Mat ImageProcessor::transform(const cv::Mat& image) const
{
    // clone：画像データを完全に複製する関数
    return image.clone();
}