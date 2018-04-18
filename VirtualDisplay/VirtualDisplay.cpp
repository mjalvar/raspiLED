

#include "VirtualDisplay.hpp"

VirtualDisplay::VirtualDisplay() {
    W = Constants::MORPHO_DISPLAY_W;
    H = Constants::MORPHO_DISPLAY_H;
}

void VirtualDisplay::pixelRGB_xy(int x, int y, MorphoColor c){
    printf("Virtual class pixelRGB_xy\n");
}

void VirtualDisplay::sleep(int ms){

}

