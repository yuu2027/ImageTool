#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include "Config.h"
#include <filesystem>
#include <string>
#include <unordered_set> // 値を重複なしで保存するための入れ物

namespace fs = std::filesystem;

// 出力ファイル名を決めるクラス
class NameGenerator {
public:
    // config の設定と出力先フォルダを受け取る
    NameGenerator(const Config& config, const std::filesystem::path& outputDir);

    // 入力ファイルに対して、出力先のフルパスを生成する
    std::filesystem::path generate(const std::filesystem::path& inputFile);

private:
    std::filesystem::path outputDir_; // 出力フォルダ
    RenameMode renameMode_;           // 命名モード（keep / seq）
    std::string prefix_;              // keep のときに付ける接頭辞
    int sequence_;                    // seq のときの連番カウンタ
    std::string format_;              // 拡張子

    // 今回の実行中に使用した名前を記録する
    std::unordered_set<std::string> reservedNames_;

    // 元ファイル名を使う方式の名前を作る
    std::string makeKeepName(const std::filesystem::path& inputFile) const;

    // 連番方式の名前を作る
    std::string makeSeqName(const std::filesystem::path& inputFile);

    // 同名衝突を避ける
    std::string avoidCollision(const std::string& fileName);

    // 出力拡張子を返す
    std::string getOutputExtension(const fs::path& inputFile) const;
};

#endif