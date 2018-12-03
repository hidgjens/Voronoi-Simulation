#include "PitchModel.h"
#include "models/Uniform.h"
#include "models/LinearX.h"
#include "models/Step.h"
#include "models/FourStep.h"


PitchModel::PitchModel(){}

double PitchModel::getPitchWeight(Cart, Pitch*) { return 1.0; }

double PitchModel::get_wmin() const { return w_min; }
void PitchModel::set_wmin(double min) { w_min = min; }

double PitchModel::get_wmax() const { return w_max; }
void PitchModel::set_wmax(double max) { w_max = max; }

PitchModel* PitchModel::getPitchModel(std::string mdl, double min, double max) {
    // factory method 
    PitchModel* return_mdl;
    if (mdl == "Uniform") { return_mdl = new Uniform(); }
    else
    if (mdl == "LinearX") { return_mdl = new LinearX(); }
    else
    if (mdl == "Step") { return_mdl = new Step(); }
    else
    if (mdl == "FourStep") { return_mdl = new FourStep(); }
    // add more here

    // else
    else
    {throw std::invalid_argument("Unknown pitch model " + mdl + "\n"); }

    return_mdl->set_wmin(min); return_mdl->set_wmax(max);
    return return_mdl;
    
}
