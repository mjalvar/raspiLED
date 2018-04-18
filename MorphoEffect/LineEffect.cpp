

#include "LineEffect.hpp"

LineEffect::LineEffect(VirtualDisplay* VD) : MorphoEffect(VD) {
    delay = 10;
    up_down = false;
    line.red();
};


void LineEffect::run() {
    MorphoColor c;

    if( up_down ){
        for(int y=0;y<H;++y) {
            for(int x=0;x<W;++x) {
                c.colorXY(img,x,y);
                if( !c.is_black() ){
                    D->pixelBRGB_xy(x,y,line);
                }
            }
            D->show();
            D->sleep(delay);
            for(int x=0;x<W;++x) {
                c.colorXY(img,x,y);
                D->pixelBRGB_xy(x,y,c);
            }
            if( y==(H-1) )
                D->show();
        }
    }
    else {
        for(int x=0;x<W;++x) {
            for(int y=0;y<H;++y) {
                c.colorXY(img,x,y);
                if( !c.is_black() ){
                    D->pixelBRGB_xy(x,y,line);
                }
            }
            D->show();
            D->sleep(delay);
            for(int y=0;y<H;++y) {
                c.colorXY(img,x,y);
                D->pixelBRGB_xy(x,y,c);
            }
        }
    }

};
