#ifndef XLINEAR_H
#define XLINEAR_H

#include "../PitchModel.h"
#include "../Pitch.h"

class Xlinear: public PitchModel {
    private:
        double x_min, x_max;

    public:
        Xlinear();
        Xlinear(Pitch, MatchConfigFile);
        double pitchWeight(Cart, Frame);
};

#endif