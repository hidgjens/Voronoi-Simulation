#include "PitchModel.h"
#include "models/Uniform.h"
#include "models/LinearX.h"


PitchModel::PitchModel(){}

double PitchModel::getPitchWeight(Cart, Pitch*) { return 1.0; }

double PitchModel::get_wmin() const { return w_min; }
void PitchModel::set_wmin(double min) { w_min = min; }

double PitchModel::get_wmax() const { return w_max; }
void PitchModel::set_wmax(double max) { w_max = max; }

PitchModel* PitchModel::getPitchModel(std::string mdl) {
    // factory method 
    PitchModel* return_mdl;
    if (mdl == "Uniform") { return_mdl = new Uniform(); }
    else
    if (mdl == "LinearX") { return_mdl = new LinearX(); }
    // add more here

    // else
    else
    {throw std::invalid_argument("Unknown pitch model " + mdl + "\n"); }

    return return_mdl;
    
}
