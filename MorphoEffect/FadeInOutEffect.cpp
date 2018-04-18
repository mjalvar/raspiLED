

#include "FadeInOutEffect.hpp"

FadeInOutEffect::FadeInOutEffect(VirtualDisplay* VD) : MorphoEffect(VD) {
    delay = 100;
    perc_start = 0;
    perc_end = 100;
    use_br = false;
    br_start = 1;
    br_end = 31;
    step = 20;
    clear = true;
};


void FadeInOutEffect::random(bool IN=true){
    vector<int> arrX,arrY;
    MorphoColor i;

    delay = 10;
    for(int y=0;y<H;++y) {
        for(int x=0;x<W;++x) {
            i.colorXY(img,x,y);
            if( !i.is_black() ){
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

};


void FadeInOutEffect::fade_in(){
    MorphoColor c;

    // color percentage, br=1
    for(int p=perc_start;p<=perc_end;p+=10) {
        if(clear) D->clear();
        for(int x=0;x<W;++x) {
            for(int y=0;y<H;++y) {
                cv::Vec3b img_color = img->at<cv::Vec3b>(y,x);
                c.B = img_color.val[0];
                c.G = img_color.val[1];
                c.R = img_color.val[2];
                if( !c.is_black(10) ) {
                    c.B = img_color.val[0]*p/100;
                    c.G = img_color.val[1]*p/100;
                    c.R = img_color.val[2]*p/100;
                    D->pixelRGB_xy(x,y,c);
                }
            }
        }
        D->show();
        D->sleep(delay);
    }
    // brightness
    if( use_br ){
        for(int br=br_start;br<=br_end;br++) {
            if(clear) D->clear();
            for(int x=0;x<W;++x) {
                for(int y=0;y<H;++y) {
                    cv::Vec3b img_color = img->at<cv::Vec3b>(y,x);
                    c.B = img_color.val[0];
                    c.G = img_color.val[1];
                    c.R = img_color.val[2];
                    c.br = br;
                    if( !c.is_black(10) )
                        D->pixelRGB_xy(x,y,c);
                }
            }
            D->show();
            D->sleep(delay);
        }
    }
}


void FadeInOutEffect::fade_out(){
    MorphoColor c;

    // brightness
    if( use_br ){
        for(int br=br_end;br>=br_start;br--) {
            if(clear) D->clear();
            for(int x=0;x<W;++x) {
                for(int y=0;y<H;++y) {
                    cv::Vec3b img_color = img->at<cv::Vec3b>(y,x);
                    c.B = img_color.val[0];
                    c.G = img_color.val[1];
                    c.R = img_color.val[2];
                    c.br = br;
                    if( !c.is_black(10) )
                        D->pixelBRGB_xy(x,y,c);
                }
            }
            D->show();
            D->sleep(delay);
        }
    }
    // color percentage, br=1
    for(int p=perc_end;p>=perc_start;p-=10) {
        if(clear) D->clear();
        for(int x=0;x<W;++x) {
            for(int y=0;y<H;++y) {
                cv::Vec3b img_color = img->at<cv::Vec3b>(y,x);
                c.B = img_color.val[0];
                c.G = img_color.val[1];
                c.R = img_color.val[2];
                if( !c.is_black(10) ) {
                    c.B = img_color.val[0]*p/100;
                    c.G = img_color.val[1]*p/100;
                    c.R = img_color.val[2]*p/100;
                    c.br = 1;
                    D->pixelBRGB_xy(x,y,c);
                }
            }
        }
        D->show();
        D->sleep(delay);
    }
}
