#ifndef FOURSTEP_H
#define FOURSTEP_H

#include "../PitchModel.h"


class FourStep: public PitchModel {
    public:
        FourStep();
        FourStep(double, double);

        double getPitchWeight(Cart, Pitch*);
};

#endif