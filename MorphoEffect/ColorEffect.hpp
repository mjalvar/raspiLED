/*
*/

#ifndef COLOR_EFFECT_H
#define COLOR_EFFECT_H

#include <vector>
#include <algorithm>
#include <random>

#include "MorphoEffect.hpp"


class ColorEffect: public MorphoEffect{

    private:
        void random(bool IN, bool POS, MorphoColor c);

    public:
        ColorEffect(VirtualDisplay* VD);

        unsigned int step;

        void swap(bool TO_MASK=true);
        void random_in(int th=200);
        void random_out(int th=200);
};

#endif
