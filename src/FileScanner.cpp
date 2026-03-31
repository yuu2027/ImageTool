#include "FileScanner.h"
#include <algorithm>
#include <string>

namespace fs = std::filesystem;

// 画像ファイルか判別
static bool isImageFile(const fs::path& path)
{
    // 拡張子を持つか
    if (!path.has_extension()) {
        return false;
    }

    std::string ext = path.extension().string(); // 拡張子を文字列で渡す
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); // 拡張子を小文字に変換

    return ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".eps";
}

// 画像ファイルの一覧を返す関数
std::vector<fs::path> FileScanner::scanImages(const fs::path& inputDir)
{
    std::vector<fs::path> files;

    for (const auto& entry : fs::directory_iterator(inputDir)) {

        // 普通のファイルかつ画像拡張子を持つか
        if (entry.is_regular_file() && isImageFile(entry.path())) {
            files.push_back(entry.path());
        }
    }

    return files;
}