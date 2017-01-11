//
//  main.cpp
//  fresh_vegtables
//
//  Created by shiga on 2017/01/11.
//  Copyright (c) 2017年 shiga. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

//画像ファイル (小さめが良い)
#define FILE_NAME "/Users/x12050xx/Pictures/71fc11a0.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"

int main(int argc, const char * argv[]) {
    int x, y;
    
    //画像の入力
    cv::Mat src_img = cv::imread(FILE_NAME, cv::IMREAD_ANYCOLOR);
    if (src_img.empty()) { //入力失敗の場
        return (-1);
    }
    
    //出力画像のメモリ確保
    cv::Mat gray_img = cv::Mat(src_img.size(), CV_8UC1);
    
    //画像の走査
    for (y=0; y<src_img.rows; y++) {
        for (x=0; x<src_img.cols; x++) {
            //画素値の取得
            cv::Vec3b s = src_img.at<cv::Vec3b>(y, x);
            uchar val = 0.11448*s[0] //B
            + 0.58661*s[1] //G
            + 0.29891*s[2];//R
            gray_img.at<uchar>(y, x) = val;
        }
    }
    // OpenCV関数
    cv::cvtColor(src_img, gray_img, cv::COLOR_BGR2GRAY);
    
    cv::imshow(WINDOW_NAME_INPUT, src_img);//画像の表示
    cv::imshow(WINDOW_NAME_OUTPUT, gray_img);//画像の表示
    cv::waitKey(); //キー入力待ち (止める)
    return 0;
}