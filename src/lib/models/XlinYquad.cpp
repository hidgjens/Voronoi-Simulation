#include "XlinYquad.h"

XlinYquad::XlinYquad() {}

XlinYquad::XlinYquad(Pitch pitch, MatchConfigFile mcf) {
    pitch_data = pitch;
    x_min = mcf.x_min;
    x_max = mcf.x_max; 
    y_min = mcf.y_min;
    y_max = mcf.y_max;
}

double XlinYquad::pitchWeight(Cart pos, Frame frame) {
    double x_ = pos.xComp();
    double y_ = pos.yComp();
    // reflect coordinates if away possession
    if (!frame.getPossession()) {
        x_ *= -1.0;
        y_ *= -1.0;
    }
    //                  v rescaling variable to [0,1]

    double A_y = 4 * (y_max - y_min) / (pitch_data.getYdim() * pitch_data.getYdim());

    double x_contr = x_min + ((x_ / pitch_data.getXdim()) + 0.5) * (x_max - x_min);
    double y_contr = - A_y * ( y_ - pitch_data.getYdim() / 2.0) * ( y_ + pitch_data.getYdim() / 2.0) + y_min;

    return x_contr * y_contr;
}