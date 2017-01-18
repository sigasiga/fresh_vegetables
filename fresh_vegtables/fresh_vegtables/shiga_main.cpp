//
//  main.cpp
//  fresh_vegtables
//
//  Created by shiga on 2017/01/11.
//  Copyright (c) 2017年 shiga. All rights reserved.
//

//メモ
//    for (y=0; y<src_img.rows; y++) {
//        for (x=0; x<src_img.cols; x++) {
//            //画素値の取得
//            cv::Vec3b s = src_img.at<cv::Vec3b>(y, x);
//            if(s[0]<100 && s[1]<160 && s[1]>50 && s[2]>150){
//                s[0] = 0; //B
//                s[1] = 0; //G
//                s[2] = 0; //R
//                count++;
//            }
//            dst_img.at<cv::Vec3b>(y, x) = s;
//        }
//    }


#include <iostream>
#include <opencv2/opencv.hpp>

//画像ファイル (小さめが良い)
#define FILE_NAME "/Users/shiga/Desktop/yasai/人参/13102924_common.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"

//------------------------------------------
//色値取得用の関数
void color_input(cv::Mat src_img){
    
    //変数の宣言
    int x, y;
    int count=0;
    int s_r=0,s_g=0,s_b=0;//色値記録用変数
    double r=0,g=0,b=0;//色値平均計算用変数
    
    //画像の走査
    for (y=0; y<src_img.rows; y++) {
        for (x=0; x<src_img.cols; x++) {
            //画素値の取得
            cv::Vec3b s = src_img.at<cv::Vec3b>(y, x);
            if(s[0]<100 && s[1]<160 && s[1]>50 && s[2]>150){//オレンジの色値を加算
                s_b = s_b + s[0]; //B
                s_g = s_g + s[1]; //G
                s_r = s_r + s[2]; //R
                count++;//オレンジ画素の計算
            }
        }
    }
    
    //オレンジっぽい色のRGBの色値の平均を出す
    r = (double)s_r / (double)count;
    g = (double)s_g / (double)count;
    b = (double)s_b / (double)count;
    
    if(b<60 && g<120 && g>80 && r>200){//濃いオレンジであった場合
        printf("おいしい人参\n");
    }
    //    printf("%f %f %f\n",r, g, b);//色値の出力
}
//------------------------------------------

int main(int argc, const char * argv[]) {
    
    //画像の入力
    cv::Mat src_img = cv::imread(FILE_NAME, cv::IMREAD_ANYCOLOR);
    if (src_img.empty()) { //入力失敗の場
        return (-1);
    }
    
    //人参のおいしいか判断関数
    color_input(src_img);
    
    //画像の走査
    // OpenCV関数
    cv::imshow(WINDOW_NAME_INPUT, src_img);//画像の表示
    cv::waitKey(); //キー入力待ち (止める)
    return 0;
}




