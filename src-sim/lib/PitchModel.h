#ifndef PITCHMODEL_H
#define PITCHMODEL_H

#include "vect/Cart.h"

class Pitch;

class PitchModel{

protected:
    double w_min;
    double w_max;

public:
    PitchModel();

    virtual double getPitchWeight(Cart, Pitch*);

    double get_wmin() const;
    void set_wmin(double);

    double get_wmax() const;
    void set_wmax(double);

    static PitchModel* getPitchModel(std::string, double, double);
};

#endif