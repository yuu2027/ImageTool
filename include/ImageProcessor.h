#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <filesystem>

// 入力ファイルを出力先に処理して保存する
// 成功したら true、失敗したら false
class ImageProcessor {
public:
    bool process(const std::filesystem::path& inputPath,
        const std::filesystem::path& outputPath);
};

#endif