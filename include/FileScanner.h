#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <filesystem>
#include <vector>

class FileScanner {
public:
    static std::vector<std::filesystem::path> scanImages(const std::filesystem::path& inputDir);
};

#endif