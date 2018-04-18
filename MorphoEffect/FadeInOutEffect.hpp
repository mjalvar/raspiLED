/*
    perc_start     // FadeIn percentage to start with: 0-100
    perc_end       // FadeIn percentage to end with: 0-100
    use_br         // After 100% continue with brightness

    br_start       // Initial value of the Br: 0-31
    br_end         // Initial value of the Br: 0-31

    perc_start=100 and perc_end=100 and use_br=true
    can be used to control using brightness 
*/

#ifndef FADE_INOUT_EFFECT_H
#define FADE_INOUT_EFFECT_H

#include "MorphoEffect.hpp"

class FadeInOutEffect: public MorphoEffect{
    private:
        void random(bool IN);

    public:
        int perc_start, perc_end;
        bool use_br;
        bool clear;
        int br_start, br_end;
        unsigned int step;

        FadeInOutEffect(VirtualDisplay* VD);
        void fade_in();
        void fade_out();
        void random_in()  { random(true); };
        void random_out() { random(false); };
};

#endif
