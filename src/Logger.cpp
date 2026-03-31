#include "Logger.h"
#include <iostream>

// Loggerクラスの実装ファイル
// info は標準出力、error は標準エラー出力へ表示する

// 通常ログは標準出力に出す
void Logger::info(const std::string& message)
{
    std::cout << "[INFO] " << message << std::endl;
}

// エラーログは標準エラー出力に出す
void Logger::error(const std::string& message)
{
    std::cerr << "[ERROR] " << message << std::endl;
}