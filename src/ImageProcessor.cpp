#include "ImageProcessor.h"
#include "Logger.h"
#include <filesystem>
#include <opencv2/imgcodecs.hpp> // 画像ファイルの読み書き用
#include <opencv2/opencv.hpp> // OpenCV のまとめヘッダ(Mat, resize, Size, INTER_AREAなど)

namespace fs = std::filesystem;

// 入力ファイルを出力先へコピー
bool ImageProcessor::file_copy(const fs::path& inputPath, const fs::path& outputPath)
{
    try {
        // overwrite_existing：既存のファイルがあっても上書きする
        fs::copy_file(inputPath, outputPath, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (...) { // 例外処理．(...)：どんな種類の例外でも受け取る
        return false;
    }
}

// 画像の読み込み
cv::Mat ImageProcessor::load(const fs::path& inputPath) const
{
    // imread：画像ファイルを読み込む関数
    // cv::IMREAD_UNCHANGED：元画像の情報をできるだけそのまま保って読む
    cv::Mat image = cv::imread(inputPath.string(), cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        Logger::error("画像を読み込めませんでした: " + inputPath.string());
    }
    else {
        Logger::info("画像を読み込みました: " + inputPath.string());
    }

    return image;
}

// 画像の保存
bool ImageProcessor::save(const fs::path& outputPath, const cv::Mat& image) const
{
    if (image.empty()) {
        Logger::error("保存対象の画像が空です");
        return false;
    }

    try {
        // parent_path：親フォルダ部分を取り出す
        fs::path parent = outputPath.parent_path();

        if (!parent.empty()) {
            if (fs::exists(parent) && !fs::is_directory(parent)) {
                Logger::error("出力先の親パスがフォルダではありません: " + parent.string());
                return false;
            }

            if (!fs::exists(parent)) {
                fs::create_directories(parent);
            }
        }

        std::string ext = outputPath.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        if (ext.empty()) {
            Logger::error("出力ファイルの拡張子がありません: " + outputPath.string());
            return false;
        }

        cv::Mat outputImage = image;

        // JPEG保存時のチャンネル調整
        if (ext == ".jpg" || ext == ".jpeg") {
            if (image.channels() == 4) {
                cv::cvtColor(image, outputImage, cv::COLOR_BGRA2BGR);
            }
            else if (image.channels() == 1) {
                // 必要なら3ch化してもよい
                // cv::cvtColor(image, outputImage, cv::COLOR_GRAY2BGR);
            }
        }

        bool ok = cv::imwrite(outputPath.string(), outputImage);

        if (ok) {
            Logger::info("画像を保存しました: " + outputPath.string());
        }
        else {
            Logger::error("画像を保存できませんでした: " + outputPath.string());
        }

        return ok;
    }
    catch (const cv::Exception& e) {
        Logger::error("OpenCVエラーで保存に失敗しました: " + std::string(e.what()));
        return false;
    }
    catch (const std::exception& e) {
        Logger::error("例外により保存に失敗しました: " + std::string(e.what()));
        return false;
    }
    catch (...) {
        Logger::error("不明な例外により保存に失敗しました: " + outputPath.string());
        return false;
    }
}

cv::Mat ImageProcessor::transform(const cv::Mat& image, const Config& config)
{
    if (image.empty()) {
        return image;
    }

    cv::Mat result = image.clone();

    // リサイズ変換
    if (config.resizeLongSide > 0) {
        result = resizeKeepAspect(result, config.resizeLongSide);
    }

    // グレースケール変換
    if (config.grayscale) {
        // channels：画像が何チャンネルかを返す関数
        // cv::cvtColor()：カラー画像をグレースケール画像に変換する関数
        if (result.channels() == 3) {
            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
        }
        else if (result.channels() == 4) {
            cv::cvtColor(result, result, cv::COLOR_BGRA2GRAY);
        }
        else if (result.channels() == 1) {
            // すでにグレースケールなので何もしない
        }
        else {
            Logger::error("未対応のチャンネル数です: " + std::to_string(result.channels()));
        }
    }


    return result;
}

// 画像の縦横比を保ったまま、長辺が targetLongSide になるように縮小する処理
cv::Mat ImageProcessor::resizeKeepAspect(const cv::Mat& image, int targetLongSide)
{
    if (image.empty()) {
        return image;
    }

    int width = image.cols; // 画像の横幅
    int height = image.rows; // 画像の高さ
    int longSide = std::max(width, height); // 幅と高さのうち，大きい方を長編として設定

    // リサイズ無効、または拡大になってしまう場合はそのまま返す
    if (targetLongSide <= 0 || longSide <= targetLongSide) {

        Logger::info(
            "resize skipped: " +
            std::to_string(width) + "x" + std::to_string(height)
        );

        return image.clone();
    }

    // 縮小倍率を求める
    // static_cast<double>：doubleに変換
    double scale = static_cast<double>(targetLongSide) / static_cast<double>(longSide);

    // 新しい幅と高さを計算
    // round(..)：少数を四捨五入
    int newWidth = static_cast<int>(std::round(width * scale));
    int newHeight = static_cast<int>(std::round(height * scale));

    // 念のため 1 未満を防ぐ
    newWidth = std::max(1, newWidth);
    newHeight = std::max(1, newHeight);

    cv::Mat resized;

    // 画像をリサイズ
    // resize(..)：OpenCV の画像サイズ変更関数
    // cv::resize(入力画像, 出力画像, 新サイズ, fx, fy, 補間方法)
    // cv::INTER_AREA：もっとも単純．速いが荒くなりやすい
    cv::resize(image, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);

    Logger::info(
        "resize: " +
        std::to_string(width) + "x" + std::to_string(height) +
        " -> " +
        std::to_string(newWidth) + "x" + std::to_string(newHeight)
    );

    return resized;
}