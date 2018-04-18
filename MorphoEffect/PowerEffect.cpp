

#include "PowerEffect.hpp"


void PowerEffect::run(){
    MorphoColor c;
    c.white();
    for(int br=1;br<31;++br) {
        for(int x=0;x<W;++x)
            for(int y=0;y<H;++y) {
                c.br = br;
                D->pixelBRGB_xy(x,y,c);
            }
        D->show();
        D->sleep(delay);
        printf("PowerEffect: br %0d\n",br);
    }
    D->clear();
}
