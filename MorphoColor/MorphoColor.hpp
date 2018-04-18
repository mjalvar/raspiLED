
#ifndef MORPHO_COLOR_H
#define MORPHO_COLOR_H

#ifdef RPI
    #include <bcm2835.h>
#else
    #include <inttypes.h>
#endif
#include "opencv2/opencv.hpp"
#include <string>

using namespace std;

class MorphoColor {
    public:
        uint16_t R,G,B;
        uint16_t br;
        MorphoColor();

        void black();
        void white();
        void red();
        void blue();
        void green();
        void set(uint16_t sR,uint16_t sG, uint16_t sB);

        void colorXY(cv::Mat* img, int x, int y);
        bool is_black(int th=1);
        bool is_red(int th=200);
        bool is_green(int th=200);
        bool is_blue(int th=200);
};

#endif
