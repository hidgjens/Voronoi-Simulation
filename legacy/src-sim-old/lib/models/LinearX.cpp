#include "LinearX.h"
#include "../Pitch.h"

LinearX::LinearX() { set_wmax(1.0); set_wmin(0.5); }
LinearX::LinearX(double min, double max) { set_wmax(max); set_wmin(min); }

double LinearX::getPitchWeight(Cart p, Pitch* pt) {
    double pos_x = p.xComp();

    if (!pt->getHomePossession()) { // not home possession, inverse x coords ( x -> -x )
        pos_x *= -1.0;
    }

    double val = get_wmin() + (pos_x/pt->getPitchX() + 0.5) * (get_wmax() - get_wmin()); // pos_x -> -p_x/2, val -> w_min; pox_x -> +p_x/2, val -> w_max;

    return val;
}

