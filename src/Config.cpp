#include "Config.h" // ヘッダーファイルの読み込み
#include <string> 

// コマンドライン引数を解析し、入力フォルダと出力フォルダを config に設定する
// 戻り値:
//   true  -> inputDir と outputDir の両方が設定できた
//   false -> どちらかが設定できなかった
bool ConfigParser::parse(int argc, char* argv[], Config& config)
{
    // argv[0] は通常実行ファイル名なので、実際の引数は argv[1] から確認する
    // ++iは実行前に1を足す　i++は実行後に1を足す
    for (int i = 1; i < argc; ++i) { 
        std::string arg = argv[i];
        // i + 1 < argc で、次の引数が存在することを確認している
        if (arg == "--input" && i + 1 < argc) { 
            config.inputDir = argv[++i];
        }
        else if (arg == "--output" && i + 1 < argc) {
            config.outputDir = argv[++i];
        }
    }
    // inputDir と outputDir の両方が空でなければ解析成功とみなす
    return !config.inputDir.empty() && !config.outputDir.empty();
}