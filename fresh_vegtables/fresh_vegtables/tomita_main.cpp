#include <iostream>
#include <opencv2/opencv.hpp>

//画像ファイル
#define FILE_NAME "/Users/Daiki/Desktop/yasai/vf-0182.jpg"
#define WINDOW_NAME_INPUT "input"
#define WINDOW_NAME_OUTPUT "grayimage"
#define WINDOW_NAME_BINARY "binary"

//二値化閾値
#define BIN_TH (100)
//色の閾値
#define W_TH (50)
//太さの差の閾値
#define CW_TH (12)

//関数のプロトタイプ宣言
//きゅうりの美味しさjudge_return[1:美味しい 0:不味い]
int Judge_Cucumber(cv::Mat cucumber_picture);
//結果表示きゅうりの美味しさjudge
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
    cv::Mat gray_img, bin_img, tmp_img, dst_img,hsv_img;
    double h = 0, cucumber_w = 0, count = 0;
    //輪郭の座標リストの宣言
    std::vector< std::vector< cv::Point > > contours;
    //判定結果
    int judge_h=0, judge_w=1, judge=0;//1:美味しい 0:不味い
    //入力画像を結果画像にコピー
    dst_img = cucumber_picture.clone();
    
    //5. グレースケール化,HSV画像化
    cv::cvtColor(cucumber_picture, gray_img, CV_BGR2GRAY);
    cv::cvtColor(cucumber_picture, bin_img, CV_BGR2GRAY);
    cv::cvtColor(cucumber_picture, hsv_img, CV_BGR2HSV);
    
    cv::Vec3b hsv,g;
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
        judge_h = 1;
        printf("%d\n",judge_h);
    }
    
    //二値画像コピー
    tmp_img = bin_img.clone();
    //輪郭抽出(輪郭内塗りつぶし)
    cv::findContours(tmp_img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    for (int i=0; i<contours.size(); i++) {
        cv::drawContours(tmp_img, contours, i, CV_RGB(255, 255, 255), -1);
    }
    
    cv::cvtColor(tmp_img, tmp_img, CV_GRAY2RGB);
    
    count = 0;
    
    //太さの平均値の算出
    //白い画素の個数を調べる
    for (int y=100; y<cucumber_picture.rows-100; y++) {
        for (int x=0; x<cucumber_picture.cols; x++) {
            
            g=tmp_img.at<cv::Vec3b>(y,x);
            
            if(g[0]==255){
                cucumber_w++;
            }
        }
        count++;
    }
    
    double cw_average = cucumber_w / count;
    
    for (int y=100; y<cucumber_picture.rows-100; y++) {
        cucumber_w = 0;
        for (int x=0; x<cucumber_picture.cols; x++) {
            g=tmp_img.at<cv::Vec3b>(y,x);
            
            if(g[0]==255){
                cucumber_w++;
            }
        }
        //平均値よりも閾値以上の差があれば、まずい
        if(fabs(cucumber_w-cw_average)>CW_TH){
            judge_w = 0;
        }
    }
    printf("%d\n",judge_w);
    
    
    //表示
//    cv::imshow(WINDOW_NAME_INPUT, cucumber_picture);
//    cv::imshow(WINDOW_NAME_BINARY, bin_img);
//    cv::imshow(WINDOW_NAME_OUTPUT, tmp_img);
//    cv::waitKey();
    
    judge = judge_h + judge_w;
    
    
    printf("%d\n",judge);
    
    return judge;//2:とても美味しい 1:美味しい 0:不味い
}

//結果表示きゅうりの美味しさjudge
void Printf_Result_Judge_Cucumber(int result){
    //きゅうりの美味しさ判別
    if(result==2 )
    {
        printf("とてもおいしいきゅうり\n");
    }
    else if(result==1){
        printf("おいしいきゅうり\n");
    }
    else{
        printf("まずいきゅうり\n");
    }
    
}
