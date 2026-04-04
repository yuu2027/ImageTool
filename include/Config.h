#ifndef CONFIG_H 
#define CONFIG_H 

#include <string> // 文字列を扱うためのライブラリ
#include <filesystem>

enum class RenameMode {
    Keep,
    Seq
};

struct Config { // アプリケーションの実行設定を保持する構造体
    std::string inputDir; // 入力フォルダのパスを保存するための変数
    std::string outputDir; // 出力フォルダのパスを保存するための変数

    RenameMode renameMode = RenameMode::Keep;
    std::string prefix;

    int resizeLongSide = 0; // 0ならリサイズしない
};

class ConfigParser { // コマンドライン引数を解析して Config を構築するクラス
public:
    // 引数を解析して config に結果を書き込む
    // 成功時は true、引数不足や不正な形式なら false を返す
    static bool parse(int argc, char* argv[], Config& config);
};

#endif