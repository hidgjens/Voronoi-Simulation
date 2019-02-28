#include "PitchModel.h"
#include "models/Uniform.h"
#include "models/Xlinear.h"
#include "models/XlinYquad.h"
#include "Pitch.h"

PitchModel* PitchModel::createPitchModel(std::string pm, MatchConfigFile mcf){
    PitchModel* model;

    Pitch pitch_data = Pitch(mcf.pitchX, mcf.pitchY);
    // std::cout << "PM " << pitch_data.getXlim() << "\t" << pitch_data.getYlim() << "\t" << pitch_data.getPitchLength() <<  "\t" << &pitch_data << std::endl;


    if (pm == "Uniform") {
        model = new Uniform(pitch_data, mcf);
    } else if (pm == "Xlinear"){
        model = new Xlinear(pitch_data, mcf);
    } else if (pm == "XlinYquad") {
        model = new XlinYquad(pitch_data, mcf);
    }

    return model;
}

void PitchModel::compute_model_dist(){
    home_possession_grid = PitchGrid(pitch_data, this, true);
    away_possession_grid = PitchGrid(pitch_data, this, false);
}