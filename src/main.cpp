#include "BatchRunner.h"
#include "Config.h"
#include "Logger.h"
#include "ImageProcessor.h"
#include "NameGenerator.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    Config config;

    if (!ConfigParser::parse(argc, argv, config)) {
        Logger::error("使い方: imgtool --input <入力フォルダ> --output <出力フォルダ>");
        return 1;
    }

    BatchRunner runner;

    if (!runner.run(config)) {
        Logger::error("処理に失敗しました");
        return 1;
    }
    
    return 0;
}