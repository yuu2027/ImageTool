#include "ImageProcessor.h"
#include <filesystem>

namespace fs = std::filesystem;

bool ImageProcessor::process(const fs::path& inputPath, const fs::path& outputPath)
{
    try {
        // 入力ファイルを出力先へコピー
        // overwrite_existing：既存のファイルがあっても上書きする
        fs::copy_file(inputPath, outputPath, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (...) { // 例外処理．(...)：どんな種類の例外でも受け取る
        return false;
    }
}