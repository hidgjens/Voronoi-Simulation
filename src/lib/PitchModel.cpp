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

// all this is used for model distribution

double PitchModel::convert_x_idx_to_coord(int idx) {
    return ((idx + 0.5) - 0.5 * (int)pitch_data.getXdim()) * pitch_data.getXdim() / ((int) pitch_data.getXdim()); 
}

double PitchModel::convert_y_idx_to_coord(int idx) {
    return ((idx + 0.5) - 0.5 * (int)pitch_data.getYdim()) * pitch_data.getYdim() / ((int) pitch_data.getYdim()); 
}

void PitchModel::compute_model_dist(){
    double sum_w_rsquared = 0.0, sum_w_xsquared_above = 0.0, sum_w_xsquared_below = 0.0, sum_w_ysquared_above = 0.0, sum_w_ysquared_below = 0.0;

    double v_sum_wx = 0.0, v_sum_wy = 0.0;

    double Z = 0.0, Zx_above = 0.0, Zx_below = 0.0, Zy_above = 0.0, Zy_below = 0.0;

    double temp_x, temp_y, temp_w;

    Frame f; f.setPossession(true);

    for (int i{0}; i < (int) pitch_data.getXdim(); i++){
        for (int j{0}; i < (int) pitch_data.getYdim(); j++){
            temp_x = convert_x_idx_to_coord(i); temp_y = convert_y_idx_to_coord(j);
            temp_w = pitchWeight(Cart(temp_x, temp_y), f);

            Z += temp_w;
            v_sum_wx += temp_x * temp_w;
            v_sum_wy += temp_y * temp_w;
            sum_w_rsquared += (temp_x * temp_x + temp_y * temp_y) * temp_w;
        }
    }

    sum_w_rsquared /= Z; v_sum_wx /= Z; v_sum_wy /= Z;

    mean_position = Cart(v_sum_wx, v_sum_wy);

    for (int i{0}; i < (int) pitch_data.getXdim(); i++) {
        temp_x = convert_x_idx_to_coord(i);
        temp_y = v_sum_wy;

        temp_w = pitchWeight(Cart(temp_x, temp_y), f);

        if (temp_x > v_sum_wx) {
            sum_w_ysquared_above += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zy_above += temp_w;
        } else {
            sum_w_ysquared_below += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zy_below += temp_w;
        }

        if (Zy_above == 0) Zy_above = 1.0;
        if (Zy_below == 0) Zy_below = 1.0;
        if (Zx_above == 0) Zx_above = 1.0;
        if (Zx_below == 0) Zx_below = 1.0;

        sum_w_xsquared_above /= Zx_above; sum_w_xsquared_below /= Zx_below;
        sum_w_ysquared_above /= Zy_above; sum_w_ysquared_below /= Zy_below;

        dx_plus = sqrt(sum_w_xsquared_above - (v_sum_wx * v_sum_wx + v_sum_wy * v_sum_wy));
        dx_minus = sqrt(sum_w_xsquared_below - (v_sum_wx * v_sum_wx + v_sum_wy * v_sum_wy));

        dy_plus = sqrt(sum_w_ysquared_above - (v_sum_wx * v_sum_wx + v_sum_wy * v_sum_wy));
        dy_minus = sqrt(sum_w_ysquared_below - (v_sum_wx * v_sum_wx + v_sum_wy * v_sum_wy));
        
    }

    for (int j{0}; j < (int) pitch_data.getYdim(); j++) {

    }


}