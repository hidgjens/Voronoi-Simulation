#include "FourStep.h"
#include "../Pitch.h"

FourStep::FourStep() { set_wmax(1.0); set_wmin(0.5); }
FourStep::FourStep(double min, double max) { set_wmax(max); set_wmin(min); }

double FourStep::getPitchWeight(Cart p, Pitch* pt) {

    // four regions, 0 -> wmin, 3 -> wmax, 1 - 0 = 2 - 1 = 3 - 2
    
    double xComp = p.xComp();
    bool inner = false; // whether it is in the inner region of the half

    if (!pt->getHomePossession()) {
        xComp *= -1; // x -> -x
    }

    if (std::fabs(xComp) <= pt->getPitchX()/4) {
        inner = true;
    }

    if (xComp > 0) {
        if (inner) {
            return w_min + (w_max - w_min) * 2.0 / 3.0;
        } else {
            return w_max;
        }
    } else {
        if (inner) {
            return w_min + (w_max - w_min) * 1.0 / 3.0;
        } else {
            return w_min;

        }
    }
}

