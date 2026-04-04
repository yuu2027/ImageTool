#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <filesystem>
#include <opencv2/core.hpp>
#include <Config.h>

// 画像の読み込み・保存・変換を担当するクラス
class ImageProcessor {
public:
    // ファイル自体をコピー
    bool file_copy(const std::filesystem::path& inputPath,
        const std::filesystem::path& outputPath);

    // ファイルの読み取り
    cv::Mat load(const std::filesystem::path& inputPath) const;

    // ファイルの保存
    bool save(const std::filesystem::path& outputPath, const cv::Mat& image) const;

    cv::Mat transform(const cv::Mat& image, const Config& config);

private:
    cv::Mat resizeKeepAspect(const cv::Mat& image, int targetLongSide);
};

#endif