#ifndef UNIFORM_H
#define UNIFORM_H

#include "../PitchModel.h"

class Uniform: public PitchModel{
    public:
        Uniform();
        Uniform(Pitch, MatchConfigFile);
        
        double pitchWeight(Cart, Frame);


};

#endif