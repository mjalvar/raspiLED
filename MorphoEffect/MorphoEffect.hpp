
#ifndef MORPHO_EFFECT_H
#define MORPHO_EFFECT_H

#include <stdio.h>
#include <string>
#include "opencv2/opencv.hpp"
#include "VirtualDisplay.hpp"
#ifdef MORPHO_GTK
    #include "GtkDisplay.hpp"
#else
    #include "LedDisplay.hpp"
#endif
#include "Constants.hpp"

using namespace std;


class MorphoEffect{
    protected:
        #ifdef MORPHO_GTK
            GtkDisplay* D;
        #else
            LedDisplay* D;
        #endif
        int W, H;
        cv::Mat *img, *mask;
        MorphoColor color;


    public:
        int delay;
        MorphoEffect(VirtualDisplay* VD);

        void set_img(cv::Mat& img_ref);
        void set_mask(cv::Mat& img_ref);
        virtual void run(){};
};

#endif
