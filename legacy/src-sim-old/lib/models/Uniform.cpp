#include "Uniform.h"
#include "../Pitch.h"

Uniform::Uniform() { set_wmax(1.0); set_wmin(0.0); }
Uniform::Uniform(double min, double max) { set_wmax(max); set_wmin(min); }

double Uniform::getPitchWeight(Cart p, Pitch* pt) { return 1.0; }

