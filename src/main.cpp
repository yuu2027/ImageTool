#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    // 画像ファイルのパスは環境に合わせて変更してください
    cv::Mat bgr = cv::imread("c:/Users/yuu20/opencv/sources/samples/data/apple.jpg");

    if (bgr.empty()) {
        // 画像が読み込めなかった場合の処理
        printf("Error: Image not loaded.\n");
        return -1;
    }

    cv::imshow("Image Display", bgr); // ウィンドウタイトルを追加
    cv::waitKey(0); // キー入力待ち
    cv::destroyAllWindows(); // すべてのウィンドウを閉じる
    return 0;
}