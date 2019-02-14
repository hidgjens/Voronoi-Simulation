#ifndef LINEARX_H
#define LINEARX_H

#include "../PitchModel.h"


class LinearX: public PitchModel {
    public:
        LinearX();
        LinearX(double, double);

        double getPitchWeight(Cart, Pitch*);
};

#endif