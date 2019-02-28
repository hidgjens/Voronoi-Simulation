#include "Pitch.h"
#include <iostream>

Pitch::Pitch() {}
Pitch::Pitch(double x, double y) : x_dim(x), y_dim(y) {
            length = sqrt(x_dim * y_dim);
            x_limit = x_dim / 2.0;
            y_limit = y_dim / 2.0;
    }

void Pitch::setXdimension(double x) { x_dim = x; }
void Pitch::setYdimension(double y) { y_dim = y; }

double Pitch::getXdim() { return x_dim; }
double Pitch::getYdim() { return y_dim; }
double Pitch::getXlim() { return x_limit; }
double Pitch::getYlim() { return y_limit; }
double Pitch::getPitchLength() { return length; }