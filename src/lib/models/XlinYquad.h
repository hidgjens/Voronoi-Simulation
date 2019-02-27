#ifndef XLINYQUAD_H
#define XLINYQUAD_H

#include "../PitchModel.h"
#include "../Pitch.h"

class XlinYquad: public PitchModel {
    private:
        double x_min, x_max;
        double y_min, y_max;

    public:
        XlinYquad();
        XlinYquad(Pitch, MatchConfigFile);
        double pitchWeight(Cart, Frame);
};

#endif