#ifndef UNIFORM_H
#define UNIFORM_H

#include "../PitchModel.h"

class Pitch;



class Uniform: public PitchModel {
    public:
        Uniform();
        Uniform(double, double);

        double getPitchWeight(Cart, Pitch*);
};

#endif