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
//#define FILE_NAME "/Users/x12050xx/Documents/git/fresh_vegtables/4305391-1-つのトマト、白いトップ、孤立した上からの眺め.jpg"
//#define FILE_NAME "/Users/x12050xx/Documents/git/fresh_vegtables/pick-best-tomatoes-15.jpg"
#define FILE_NAME "/Users/x12050xx/Documents/git/fresh_vegtables/xxx.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"
#define WINDOW_NAME_BINARY "binary"


//二値化閾値
#define BIN_TH (100)
//面積閾値
#define AREA_TH (0.1)
//円形度の閾値
#define CIRC_TH (0.50)


//---------------------------------------------------------関数のプロトタイプ宣言
//関数のプロトタイプ宣言
//トマトの美味しさjudge_return[1:美味しい 0:不味い]
int Judge_Tomato(cv::Mat tomato_picture);
//結果表示トマトの美味しさjudge
void Printf_Result_Judge_Tomato(int result);
//色値取得用の関数＿にんじん
void color_input(cv::Mat src_img);
//関数のプロトタイプ宣言
//きゅうりの美味しさ
int Judge_Cucumber(cv::Mat cucumber_picture);
//結果表示
void Printf_Result_Judge_Cucumber(int result);

//---------------------------------------------------------メイン文
//メイン文
int main(int argc, const char * argv[]) {
    
    //カメラ用変数宣言
    cv::VideoCapture cap(0); //ビデオキャプチャ用
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    if (!cap.isOpened()) {
        std::cerr << "Capture is not opened\n" << std::endl;
        exit(0);
    }
    cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    cv::Mat frame;//カメラの毎フレーム格納Mat
    int kamera_break_flag=0;
    
    //動画像の撮影
    while (1) {
        cap >> frame; //キャプチャ
        cv::imshow("Capture", frame);
        
        
        int key = cv::waitKey(30);
        
        //キーボタンで各判定関数へ画像を入れる
        switch (key) {
            case 'q'://終了
                kamera_break_flag=1;
                break;
                
            case 't'://tomato の t
                //トマトの美味しさ判別
                Printf_Result_Judge_Tomato( Judge_Tomato(frame) );
                break;
                
            case 'k'://kyuri の k
                //きゅうりの美味しさ判別
                //きゅうりの美味しさ判別
                Printf_Result_Judge_Cucumber( Judge_Cucumber(frame) );
                break;
                
            case 'n'://ninjin の n
                //にんじんの美味しさ判別
                //人参のおいしいか判断関数
                color_input(frame);
                break;
                
            default:
                break;
        }
        
        
        //終了
        if(kamera_break_flag==1)break;
    }
    printf("おわり\n");
    
    return 0;
}
//##################################################################################佐藤
//---------------------------------------------------------Judge_Tomato
//トマトの美味しさjudge
int Judge_Tomato(cv::Mat tomato_picture){
    
    //変数宣言
    //1. 画像の宣言 (入力画像，グレースケール画像，二値画像，一時的な画像，出力画像,HSV用画像,ヘタの抽出マスク)
    cv::Mat gray_img, bin_img, tmp_img, dst_img,hsv_img,heta_bin_img;
    double L, S, R;//
    //2. 輪郭の座標リストの宣言
    std::vector< std::vector< cv::Point > > contours;
    //判定結果
    int judge=0;//1:美味しい 0:不味い
    //最大面積
    double max_s=tomato_picture.rows*tomato_picture.cols;
    //緑色抽出
    int green_sum=0;
    cv::Vec3b green_hsv;
    double result_hsv_ave[3];
    for(int i=0;i<3;i++)result_hsv_ave[i]=0;
    
    int tmp_flag=0;
    
    //4. 入力画像を結果画像にコピー
    dst_img = tomato_picture.clone();
    
    //5. グレースケール化,HSV画像化
    cv::cvtColor(tomato_picture, gray_img, CV_BGR2GRAY);
    cv::cvtColor(tomato_picture, bin_img, CV_BGR2GRAY);
    cv::cvtColor(tomato_picture, heta_bin_img, CV_BGR2GRAY);
    cv::cvtColor(tomato_picture, hsv_img, CV_BGR2HSV);
    
    //6. 二値化 (固定閾値で実装．閾値: 100)
    cv::Vec3b hsv;
    
    //赤色抽出　緑色抽出
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
            
            //緑色抽出
            if(30<hsv[0] && hsv[0]<130 && 80<hsv[1]){
                heta_bin_img.at<uchar>(y, x)=255;
                green_sum++;
                for(int i=0;i<3;i++){
                    result_hsv_ave[i]+=(double)hsv[i];
                }
            }else{
                heta_bin_img.at<uchar>(y, x)=0;
            }
        }
    }
    
    //二値画像コピー
    tmp_img = bin_img.clone();
    
    tmp_flag=0;
    
    //7. 輪郭抽出
    cv::findContours(tmp_img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    //8. 輪郭の描画
    for (int i=0; i<contours.size(); i++) {
        //輪郭の長さ
        L = cv::arcLength(contours[i], true);
        //面積
        S = cv::contourArea(contours[i]);
        
        //面積が画像の1/3以上なら
        if (max_s * AREA_TH < S) {
            //円形度の計算
            R = 4.0 * M_PI * S / (L * L);
            //printf("--%.2f--\n",R);
            
            //円形度が閾値以上なら
            if (CIRC_TH < R) {
                //輪郭の描画
                cv::drawContours(dst_img, contours, i, CV_RGB(0, 255, 0), 3);
                //丸いので美味しい判定
                tmp_flag=1;//1=美味しい
            }else{
                cv::drawContours(dst_img, contours, i, CV_RGB(0, 0, 255), 3);
                //printf("%f\n",R);
            }
        }
    }
    if(tmp_flag==1){judge++;}
    
    
    //9. 表示
    //cv::imshow(WINDOW_NAME_INPUT, tomato_picture);
    //cv::imshow(WINDOW_NAME_BINARY, bin_img); //二値画像は，findcontour後では使用できない
    //cv::imshow(WINDOW_NAME_OUTPUT, heta_bin_img);//dst_img
    //cv::waitKey();
    
    
    tmp_flag=0;
    //最後にヘタの色値チェック
    if(judge==1){
        for(int i=0;i<3;i++){
            result_hsv_ave[i]=(double)result_hsv_ave[i]/(double)green_sum;
        }
        printf("h:%.1f s:%.1f v:%.1f\n",result_hsv_ave[0],result_hsv_ave[1],result_hsv_ave[2]);
        
        if(55<result_hsv_ave[0] && result_hsv_ave[0]<65){
            tmp_flag=1;
        }else{
            printf("色がダメ\n");
        }
    }
    if(tmp_flag==1){judge++;}
    
    return judge;//1:美味しい 0:不味い
}
//---------------------------------------------------------Printf_Result_Judge_Tomato
//結果表示トマトの美味しさjudge
void Printf_Result_Judge_Tomato(int result){
    //トマトの美味しさ判別
    if(result==1 )
    {
        printf("おいしいトマト\n");
    }else if(result==2){
        printf("すごくおいしいトマト\n");
    }
    else{
        printf("まずいトマト\n");
    }
    
}
//---------------------------------------------------------
int memo(){
    //画像の入力
    //cv::Mat src_img = cv::imread(FILE_NAME, cv::IMREAD_ANYCOLOR);
    cv::Mat src_img = cv::imread(FILE_NAME);
    
    //入力失敗の場合
    if (src_img.empty()) {
        printf("画像が読み込めませんでした\(^q^)/ \n");
        return (-1);
    }
    return 0;
}
//##############################################################################しが
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
    }else{
        printf("まずい人参\n");
    }
    //    printf("%f %f %f\n",r, g, b);//色値の出力
}
//##############################################################################とみた
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
    //cv::imshow(WINDOW_NAME_INPUT, cucumber_picture);
    //cv::imshow(WINDOW_NAME_BINARY, bin_img); //二値画像は，findcontour後では使用できない
    //cv::waitKey();
    
    
    return judge;//1:美味しい 0:不味い
}

//結果表示きゅうりの美味しさjudge
void Printf_Result_Judge_Cucumber(int result){
    //きゅうりの美味しさ判別
    if(result==1 )
    {
        printf("おいしいきゅうり\n");
    }
    else{
        printf("まずいきゅうり\n");
    }
    
}
