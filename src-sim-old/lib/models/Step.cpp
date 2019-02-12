#include "Step.h"
#include "../Pitch.h"

Step::Step() { set_wmax(1.0); set_wmin(0.5); }
Step::Step(double min, double max) { set_wmax(max); set_wmin(min); }

double Step::getPitchWeight(Cart p, Pitch* pt) {
    
    double xComp = p.xComp();

    if (!pt->getHomePossession()) {
        xComp *= -1; // x -> -x
    }

    if (xComp > 0) {
        return w_max;
    } else {
        return w_min;
    }
}

