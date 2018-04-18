

#include "MorphoColor.hpp"

MorphoColor::MorphoColor() {
    br = 1;
    black();
}


bool MorphoColor::is_black(int th) {
    if( R<th && G<th && B<th) return true;
    else return false;
}


bool MorphoColor::is_red(int th) {
    if( R>th ) return true;
    else return false;
}


bool MorphoColor::is_green(int th) {
    if( G>th ) return true;
    else return false;
}


bool MorphoColor::is_blue(int th) {
    if( B>th ) return true;
    else return false;
}


void MorphoColor::colorXY(cv::Mat* img, int x, int y) {
    cv::Vec3b img_color = img->at<cv::Vec3b>(y,x);
    B = img_color.val[0];
    G = img_color.val[1];
    R = img_color.val[2];
}

void MorphoColor::black(){
    R = 0;
    G = 0;
    B = 0;
}

void MorphoColor::white(){
    R = 255;
    G = 255;
    B = 255;
}

void MorphoColor::red(){
    R = 255;
    G = 0;
    B = 0;
}

void MorphoColor::green(){
    R = 0;
    G = 255;
    B = 0;
}

void MorphoColor::blue(){
    R = 0;
    G = 0;
    B = 255;
}

void MorphoColor::set(uint16_t sR, uint16_t sG, uint16_t sB){
    R = sR;
    G = sG;
    B = sB;
}

