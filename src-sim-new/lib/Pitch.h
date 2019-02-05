#ifndef PITCH_H
#define PITCH_H

#include<cmath>

struct Pitch {
    /*
        Pitch data structure, for use by AI to understand the pitch boundaries.
        - x_dim and y_dim are pitch dimensions, x is length between home and away edges.
        - length is the characteristic pitch length, =sqrt(x*y)
        - x_lim, y_lim are half of pitch dimensions st the mod value of a coordinate
            should not be larger than these limits. Included for convenience
    */

    double x_dim, y_dim, length, x_limit, y_limit;

    Pitch() {}

    Pitch(double x, double y) : x_dim(x) , y_dim(y) {
        length = sqrt(x_dim * y_dim);
        x_limit = x_dim / 2.0;
        y_limit = y_dim / 2.0;
    }   

};

#endif