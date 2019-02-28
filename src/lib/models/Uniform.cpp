#include "Uniform.h"

Uniform::Uniform() {}
Uniform::Uniform(Pitch p, MatchConfigFile mcf){
    pitch_data = p;
}

double Uniform::pitchWeight(Cart pos, Frame frame) {
    // uniform
    return 1.0;
}