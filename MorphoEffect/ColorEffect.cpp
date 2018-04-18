

#include "ColorEffect.hpp"

ColorEffect::ColorEffect(VirtualDisplay* VD) : MorphoEffect(VD) {
    delay = 10;
    step = 50;
};


void ColorEffect::random_in(int th_value){
    MorphoColor th;
    th.white();
    th.R = th_value;
    random(true,true,th);
    th.white();
    th.B = th_value;
    random(true,true,th);
    th.white();
    th.G = th_value;
    random(true,true,th);
    th.R = th_value;
    th.B = th_value;
    th.G = th_value;
    random(true,false,th);
}


void ColorEffect::random_out(int th_value){
    MorphoColor th;
    th.white();
    th.R = th_value;
    random(false,true,th);
    th.white();
    th.B = th_value;
    random(false,true,th);
    th.white();
    th.G = th_value;
    random(false,true,th);
    th.R = th_value;
    th.B = th_value;
    th.G = th_value;
    random(false,false,th);
}


void ColorEffect::random(bool IN, bool POS, MorphoColor th){
    vector<int> arrX,arrY;
    MorphoColor i;
    delay = 10;
    for(int y=0;y<H;++y) {
        for(int x=0;x<W;++x) {
            i.colorXY(img,x,y);
            if( POS && (i.is_red(th.R) || i.is_green(th.G) || i.is_blue(th.B)) ){
                //printf("adding %0d,%0d,%0d\n",i.R,i.G,i.B);
                arrX.push_back(x);
                arrY.push_back(y);
            }
            if( !POS && !(i.is_red(th.R) || i.is_green(th.G) || i.is_blue(th.B)) ){
                //printf("adding %0d,%0d,%0d\n",i.R,i.G,i.B);
                arrX.push_back(x);
                arrY.push_back(y);
            }
        }
    }

    auto engine = std::default_random_engine{};
    std::vector<int> indices(arrX.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), engine);

    unsigned int count = 0;
    unsigned int Pcount = arrX.size();
    for (auto j : indices) {
        if(IN)
            i.colorXY(img,arrX[j],arrY[j]);
        else
            i.black();
        D->pixelBRGB_xy(arrX[j],arrY[j],i);
        if( count==step || Pcount<step ) {
            D->show();
            D->sleep(delay);
            count = 0;
        }
        ++count;
        --Pcount;
    }
    D->show();
    D->sleep(1000);

};


// Swap between img and mask
// with random pixels
void ColorEffect::swap(bool TO_MASK) {
    MorphoColor i;
    vector<int> arrX,arrY;

    for(int y=0;y<H;++y) {
        for(int x=0;x<W;++x) {
            if( TO_MASK )
                i.colorXY(img,x,y);
            else
                i.colorXY(mask,x,y);
            if( !i.is_black() ){
                D->pixelBRGB_xy(x,y,i);
                arrX.push_back(x);
                arrY.push_back(y);
            }
        }
    }
    D->show();
    D->sleep(1000);

    auto engine = std::default_random_engine{};
    std::vector<int> indices(arrX.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), engine);

    int count = 0;
    unsigned int Pcount = arrX.size();
    for (auto j : indices) {
        if( TO_MASK )
            i.colorXY(mask,arrX[j],arrY[j]);
        else
            i.colorXY(img,arrX[j],arrY[j]);
        D->pixelBRGB_xy(arrX[j],arrY[j],i);
        if( count==10 || Pcount<10 ) {
            D->show();
            D->sleep(delay);
            count = 0;
        }
        ++count;
        --Pcount;
        //printf("step %0d, pending %0d\n",step,Pcount);
    }

};
