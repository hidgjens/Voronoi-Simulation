#include "Xlinear.h"

Xlinear::Xlinear() {}

Xlinear::Xlinear(Pitch pitch, MatchConfigFile mcf) {
    pitch_data = pitch;
    x_min = mcf.x_min;
    x_max = mcf.x_max; 
}

double Xlinear::pitchWeight(Cart pos, Frame frame) {
    double x_ = pos.xComp();
    //                  v rescaling variable to [0,1]
    return x_min + ((x_ / pitch_data.x_dim) + 0.5) * (x_max - x_min);
}