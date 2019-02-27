#include "PitchGrid.h"
#include "PitchModel.h"

double PitchGrid::convert_x_idx_to_coord(int idx) {
    return ((idx + 0.5) - 0.5 * (int)pitch_data.getXdim()) * pitch_data.getXdim() / ((int) pitch_data.getXdim()); 
}

double PitchGrid::convert_y_idx_to_coord(int idx) {
    return ((idx + 0.5) - 0.5 * (int)pitch_data.getYdim()) * pitch_data.getYdim() / ((int) pitch_data.getYdim()); 
}

PitchGrid::PitchGrid(Pitch pitch, PitchModel* pm, bool poss) :
    possession(poss), pitch_data(pitch) {
        computeGrid(pm, poss);
}

void PitchGrid::computeGrid(PitchModel* pm, bool poss) {
    possession = poss;

    double sum_w_rsquared = 0.0, sum_w_xsquared_above = 0.0, sum_w_xsquared_below = 0.0, sum_w_ysquared_above = 0.0, sum_w_ysquared_below = 0.0;

    double v_sum_wx = 0.0, v_sum_wy = 0.0;

    double Z = 0.0, Zx_above = 0.0, Zx_below = 0.0, Zy_above = 0.0, Zy_below = 0.0;

    double temp_x, temp_y, temp_w;

    Frame f; f.setPossession(poss);

    for (int i{0}; i < (int) pitch_data.getXdim(); i++){
        for (int j{0}; i < (int) pitch_data.getYdim(); j++){
            temp_x = convert_x_idx_to_coord(i); temp_y = convert_y_idx_to_coord(j);
            temp_w = pm->pitchWeight(Cart(temp_x, temp_y), f);

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

        temp_w = pm->pitchWeight(Cart(temp_x, temp_y), f);

        if (temp_x > v_sum_wx) {
            sum_w_xsquared_above += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zx_above += temp_w;
        } else {
            sum_w_xsquared_below += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zx_below += temp_w;
        }


    }

    for (int j{0}; j < (int) pitch_data.getYdim(); j++) {
        temp_x = v_sum_wx;
        temp_y = convert_y_idx_to_coord(j);

        temp_w = pm->pitchWeight(Cart(temp_x, temp_y), f);

        if (temp_y > v_sum_wy) {
            sum_w_ysquared_above += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zy_above += temp_w;
        } else {
            sum_w_ysquared_below += (temp_x * temp_x + temp_y * temp_y) * temp_w;
            Zy_below += temp_w;
        }


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


