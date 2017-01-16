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
#define FILE_NAME "/Users/x12050xx/Documents/git/fresh_vegtables/4305391-1-つのトマト、白いトップ、孤立した上からの眺め.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"
#define WINDOW_NAME_BINARY "binary"


//二値化閾値
#define BIN_TH (100)
//面積閾値
#define AREA_TH (0.3)
//円形度の閾値
#define CIRC_TH (0.8)


//---------------------------------------------------------関数のプロトタイプ宣言
//関数のプロトタイプ宣言
//トマトの美味しさjudge_return[1:美味しい 0:不味い]
int Judge_Tomato(cv::Mat tomato_picture);
//結果表示トマトの美味しさjudge
void Printf_Result_Judge_Tomato(int result);

//メイン文
int main(int argc, const char * argv[]) {
    
    //画像の入力
    //cv::Mat src_img = cv::imread(FILE_NAME, cv::IMREAD_ANYCOLOR);
    cv::Mat src_img = cv::imread(FILE_NAME);
    
    //入力失敗の場合
    if (src_img.empty()) {
        printf("画像が読み込めませんでした\(^q^)/ \n");
        return (-1);
    }
    
    //トマトの美味しさ判別
    Printf_Result_Judge_Tomato( Judge_Tomato(src_img) );
    
    
    return 0;
}
//---------------------------------------------------------Judge_Tomato
//トマトの美味しさjudge
int Judge_Tomato(cv::Mat tomato_picture){
    
    //変数宣言
    //1. 画像の宣言 (入力画像，グレースケール画像，二値画像，一時的な画像，出力画像,HSV用画像)
    cv::Mat gray_img, bin_img, tmp_img, dst_img,hsv_img;
    double L, S, R;//
    //2. 輪郭の座標リストの宣言
    std::vector< std::vector< cv::Point > > contours;
    //判定結果
    int judge=0;//1:美味しい 0:不味い
    //最大面積
    double max_s=tomato_picture.rows*tomato_picture.cols;
    
    //4. 入力画像を結果画像にコピー
    dst_img = tomato_picture.clone();
    
    //5. グレースケール化,HSV画像化
    cv::cvtColor(tomato_picture, gray_img, CV_BGR2GRAY);
    cv::cvtColor(tomato_picture, bin_img, CV_BGR2GRAY);
    cv::cvtColor(tomato_picture, hsv_img, CV_BGR2HSV);
    
    //6. 二値化 (固定閾値で実装．閾値: 100)
    cv::Vec3b hsv;
    //画像の走査
    for (int y=0; y<tomato_picture.rows; y++) {
        for (int x=0; x<tomato_picture.cols; x++) {
            
            hsv=hsv_img.at<cv::Vec3b>(y,x);
            
            //赤の色相，ちょい高めの彩度でトマトの果肉部分のみを抽出
            if(0<hsv[0] && hsv[0]<30 && 100<hsv[1]){
                bin_img.at<uchar>(y, x)=255;
            }else{
                bin_img.at<uchar>(y, x)=0;
            }
        }
    }
    
    //二値画像コピー
    tmp_img = bin_img.clone();
    
    //7. 輪郭抽出
    cv::findContours(tmp_img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    
    //8. 輪郭の描画
    for (int i=0; i<contours.size(); i++) {
        //輪郭の長さ
        L = cv::arcLength(contours[i], true);
        //面積
        S = cv::contourArea(contours[i]);
        
        //面積が画像の1/3以上なら
        if (max_s*AREA_TH < S) {
            //円形度の計算
            R = 4.0 * M_PI * S / (L * L);
            
            //円形度が閾値以上なら
            if (CIRC_TH < R) {
                //輪郭の描画
                cv::drawContours(dst_img, contours, i, CV_RGB(0, 255, 0), 3);
                //丸いので美味しい判定
                judge=1;//1=美味しい
            }else{
                cv::drawContours(dst_img, contours, i, CV_RGB(0, 0, 255), 3);
                printf("%f\n",R);
            }
            
        }
        
    }
    
    //9. 表示
    cv::imshow(WINDOW_NAME_INPUT, tomato_picture);
    cv::imshow(WINDOW_NAME_BINARY, bin_img); //二値画像は，findcontour後では使用できない
    cv::imshow(WINDOW_NAME_OUTPUT, dst_img);
    cv::waitKey();
    
    
    return judge;//1:美味しい 0:不味い
}
//---------------------------------------------------------Printf_Result_Judge_Tomato
//結果表示トマトの美味しさjudge
void Printf_Result_Judge_Tomato(int result){
    //トマトの美味しさ判別
    if(result==1 )
    {
        printf("おいしい\n");
    }
    else{
        printf("まずい\n");
    }
    
}
//---------------------------------------------------------
int temp(cv::Mat tomato_picture){
    
    int judge;//1:美味しい 0:不味い
    
    //出力画像のメモリ確保
    cv::Mat gray_img = cv::Mat(tomato_picture.size(), CV_8UC1);
    
    //画像の走査
    for (int y=0; y<tomato_picture.rows; y++) {
        for (int x=0; x<tomato_picture.cols; x++) {
            //画素値の取得
            cv::Vec3b s = tomato_picture.at<cv::Vec3b>(y, x);
            uchar val = 0.11448*s[0] //B
            + 0.58661*s[1] //G
            + 0.29891*s[2];//R
            gray_img.at<uchar>(y, x) = val;
        }
    }
    // OpenCV関数
    cv::cvtColor(tomato_picture, gray_img, cv::COLOR_BGR2GRAY);
    
    cv::imshow(WINDOW_NAME_INPUT, tomato_picture);//画像の表示
    cv::imshow(WINDOW_NAME_OUTPUT, gray_img);//画像の表示
    cv::waitKey(); //キー入力待ち (止める)
    
    return judge;
}
