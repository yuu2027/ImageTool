#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <filesystem>
#include <opencv2/core.hpp>

// 画像の読み込み・保存・変換を担当するクラス
class ImageProcessor {
public:
    // ファイル自体をコピー
    bool file_copy(const std::filesystem::path& inputPath,
        const std::filesystem::path& outputPath);

    // 
    cv::Mat load(const std::filesystem::path& inputPath) const;

    bool save(const std::filesystem::path& outputPath, const cv::Mat& image) const;

    cv::Mat transform(const cv::Mat& image) const;
};

#endif