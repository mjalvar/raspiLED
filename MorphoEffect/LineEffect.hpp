/*
*/

#ifndef LINE_EFFECT_H
#define LINE_EFFECT_H

#include "MorphoEffect.hpp"

class LineEffect: public MorphoEffect{

    public:
        LineEffect(VirtualDisplay* VD);
        
        bool up_down;
        MorphoColor line;
        void run();
};

#endif
