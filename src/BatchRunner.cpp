#include "BatchRunner.h"
#include "Logger.h"
#include "FileScanner.h"
#include "ImageProcessor.h"
#include "NameGenerator.h"
#include <filesystem> // ファイルやフォルダの存在確認などをする
#include <string>
#include <iostream>
#include <opencv2/core.hpp>

namespace fs = std::filesystem;

// BatchRunnerクラスのrun関数の本体
bool BatchRunner::run(const Config& config)
{
    fs::path inputDir = config.inputDir;
    fs::path outputDir = config.outputDir;

    // そのパスが存在するか．bool値
    if (!fs::exists(inputDir)) { 
        Logger::error("入力フォルダが存在しません: " + inputDir.string());
        return false;
    }

    // そのパスがフォルダかどうか．bool値
    if (!fs::is_directory(inputDir)) { 
        Logger::error("入力パスがフォルダではありません: " + inputDir.string());
        return false;
    }

    // 出力フォルダが存在するがフォルダではないとき
    if (fs::exists(outputDir) && !fs::is_directory(outputDir)) {
        Logger::error("出力パスがフォルダではありません: " + outputDir.string());
        return false;
    }

    // 出力フォルダが存在しない場合は新しく作成する
    if (!fs::exists(outputDir)) {

        // 必要な親フォルダも含めて作成を試みる
        if (!fs::create_directories(outputDir)) {
            Logger::error("出力フォルダの作成に失敗しました: " + outputDir.string());
            return false;
        }
        Logger::info("出力フォルダを作成しました: " + outputDir.string());
    }
    
    // auto：型の自動推論
    // 画像ファイルのパスの一覧
    auto files = FileScanner::scanImages(inputDir);

    // 画像が0件の場合失敗
    if (files.empty()) {
        Logger::error("画像が見つかりませんでした");
        return false;
    }

    Logger::info("画像数: " + std::to_string(files.size()));

    ImageProcessor processor;

    NameGenerator generator(config, config.outputDir);

    if (config.renameMode == RenameMode::Keep) {
        std::cout << "Keep" << std::endl;
    }
    else {
        std::cout << "Seq" << std::endl;
    }

    for (const auto& file : files) {
        Logger::info("見つかった画像: " + file.string());

        fs::path outputPath = generator.generate(file);

        cv::Mat image = processor.load(file);
        cv::Mat result = processor.transform(image);
        if (!processor.save(outputPath, result)) Logger::error("処理失敗: " + file.string());
    }

    Logger::info("一括処理が完了しました");

    return true;
}
