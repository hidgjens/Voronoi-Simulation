#include "Pitch.h"
#include <iostream>
#include <string>

Pitch::Pitch() {}
Pitch::Pitch(double x, double y) : x_dim(x), y_dim(y) {
            length = sqrt(x_dim * y_dim);
            x_limit = x_dim / 2.0;
            y_limit = y_dim / 2.0;
    }

void Pitch::setXdimension(double x) { x_dim = x; }
void Pitch::setYdimension(double y) { y_dim = y; }
void Pitch::setMatchName(std::string s) { match_name = s; }
void Pitch::setMatchID(int mid) { match_id = mid; }

double Pitch::getXdim() { return x_dim; }
double Pitch::getYdim() { return y_dim; }
double Pitch::getXlim() { return x_limit; }
double Pitch::getYlim() { return y_limit; }
double Pitch::getPitchLength() { return length; }
std::string Pitch::getMatchName() { return match_name; }
int Pitch::getMatchID() {return match_id; }