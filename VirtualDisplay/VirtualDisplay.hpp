
#ifndef VIRTUAL_DISPLAY_H
#define VIRTUAL_DISPLAY_H

#include <stdio.h>
#include "Constants.hpp"
#include "MorphoColor.hpp"
 
class VirtualDisplay {
    protected:
        int W, H;

    public:
        VirtualDisplay();
        virtual void pixelRGB_xy(int x, int y, MorphoColor c);
        virtual void sleep(int ms);
};

#endif
