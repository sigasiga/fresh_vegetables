//
//  main.cpp
//  fresh_vegtables
//
//  Created by shiga on 2017/01/11.
//  Copyright (c) 2017年 shiga. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

//画像ファイル
#define FILE_NAME "/Users/Daiki/Desktop/yasai/20121228113455_50dd054fbd6e4.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"
#define WINDOW_NAME_BINARY "binary"

//二値化閾値
#define BIN_TH (100)

//関数のプロトタイプ宣言
//きゅうりの美味しさ
int Judge_Cucumber(cv::Mat cucumber_picture);
//結果表示
void Printf_Result_Judge_Cucumber(int result);

//メイン文
int main(int argc, const char * argv[]) {
    
    //画像の入力
    cv::Mat src_img = cv::imread(FILE_NAME);
    
    //入力失敗の場合
    if (src_img.empty()) {
        printf("画像が読み込めませんでした\n");
        return (-1);
    }
    
    //きゅうりの美味しさ判別
    Printf_Result_Judge_Cucumber( Judge_Cucumber(src_img) );
    
    
    return 0;
}

//きゅうりの美味しさjudge
int Judge_Cucumber(cv::Mat cucumber_picture){
    
    //変数宣言
    //画像の宣言
    cv::Mat gray_img, bin_img, dst_img,hsv_img;
    //平均算出用関数（色相、画素数）
    double h = 0, count = 0;
    //輪郭の座標リストの宣言
    std::vector< std::vector< cv::Point > > contours;
    //判定結果
    int judge=0;//1:美味しい 0:不味い
    //入力画像を結果画像にコピー
    dst_img = cucumber_picture.clone();
    
    //5. グレースケール化,HSV画像化
    cv::cvtColor(cucumber_picture, gray_img, CV_BGR2GRAY);
    cv::cvtColor(cucumber_picture, bin_img, CV_BGR2GRAY);
    cv::cvtColor(cucumber_picture, hsv_img, CV_BGR2HSV);
    
    //二値化
    cv::Vec3b hsv;
    //画像の走査
    for (int y=0; y<cucumber_picture.rows; y++) {
        for (int x=0; x<cucumber_picture.cols; x++) {
            
            hsv=hsv_img.at<cv::Vec3b>(y,x);
            
            //きゅうり表面部分（黄色から緑）
            if(30<hsv[0] && hsv[0]<80 && hsv[1]>20){
                bin_img.at<uchar>(y, x)=255;
                //色値平均を出すための色値保存
                h+=hsv[0];
                count++;
            }else{
                bin_img.at<uchar>(y, x)=0;
            }
        }
    }
    
    
    //きゅうりの表面と判断された部分の色値の平均算出
    double h_average = h/count;
    printf("%f\n",h_average);
    //黄色に近くなければ美味しい
    if (h_average > 50) {
        judge = 1;
    }
    
    
    //9. 表示
    cv::imshow(WINDOW_NAME_INPUT, cucumber_picture);
    cv::imshow(WINDOW_NAME_BINARY, bin_img); //二値画像は，findcontour後では使用できない
    cv::waitKey();
    
    
    return judge;//1:美味しい 0:不味い
}

//結果表示きゅうりの美味しさjudge
void Printf_Result_Judge_Cucumber(int result){
    //きゅうりの美味しさ判別
    if(result==1 )
    {
        printf("おいしい\n");
    }
    else{
        printf("まずい\n");
    }
    
}
