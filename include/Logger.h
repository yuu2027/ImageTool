#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// アプリケーション内で情報ログやエラーログを出力するためのクラス
class Logger {
public:
    // 通常の処理状況を示すログを出力する
    static void info(const std::string& message);
    // エラー内容を示すログを出力する
    static void error(const std::string& message);
};

#endif