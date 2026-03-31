#include <iostream>
#include "Config.h"

int main(int argc, char* argv[])
{
    Config config;

    if (!ConfigParser::parse(argc, argv, config)) {
        std::cout << "使い方: imgtool --input <入力フォルダ> --output <出力フォルダ>" << std::endl;
        return 1;
    }

    std::cout << "input: " << config.inputDir << std::endl;
    std::cout << "output: " << config.outputDir << std::endl;

    return 0;
}