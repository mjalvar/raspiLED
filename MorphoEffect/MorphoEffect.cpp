

#include "MorphoEffect.hpp"

MorphoEffect::MorphoEffect(VirtualDisplay* VD) {
    W = Constants::MORPHO_DISPLAY_W;
    H = Constants::MORPHO_DISPLAY_H;
    #ifdef MORPHO_GTK
        D = (GtkDisplay*) VD;
    #else
        D = (LedDisplay*) VD;
    #endif
    delay = 1000;
}


void MorphoEffect::set_img(cv::Mat& img_ref){
    img = &img_ref;
}


void MorphoEffect::set_mask(cv::Mat& img_ref){
    mask = &img_ref;
}

