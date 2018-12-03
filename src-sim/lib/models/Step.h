#ifndef STEP_H
#define STEP_H

#include "../PitchModel.h"


class Step: public PitchModel {
    public:
        Step();
        Step(double, double);

        double getPitchWeight(Cart, Pitch*);
};

#endif