#include "PitchModel.h"
#include "models/Uniform.h"
#include "models/Xlinear.h"
#include "Pitch.h"

PitchModel* PitchModel::createPitchModel(std::string pm, MatchConfigFile mcf){
    PitchModel* model;

    Pitch pitch_data = Pitch(mcf.pitchX, mcf.pitchY);

    if (pm == "Uniform") {
        model = new Uniform();
    } else if (pm == "Xlinear"){
        model = new Xlinear(pitch_data, mcf);
    }

    return model;
}