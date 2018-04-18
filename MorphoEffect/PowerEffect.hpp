

#ifndef POWER_EFFECT_H
#define POWER_EFFECT_H

#include "MorphoEffect.hpp"


class PowerEffect: public MorphoEffect{

    public: 
        PowerEffect(VirtualDisplay* VD) : MorphoEffect(VD) {};
        void run();
};

#endif
