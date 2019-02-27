/*
    Class which models the weighting of the pitch

*/

#ifndef PITCHMODEL_H
#define PITCHMODEL_H

#include "Cart.h"
#include "Frame.h"
#include "Pitch.h"
#include "cfg/MatchConfigFile.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class PitchModel{
    protected:
        double convert_x_idx_to_coord(int);
        double convert_y_idx_to_coord(int);
        Pitch pitch_data;
        Cart mean_position;
        double dx_plus, dx_minus, dy_plus, dy_minus;

    public:
        PitchModel() {}
        static PitchModel* createPitchModel(std::string, MatchConfigFile);
        virtual double pitchWeight(Cart, Frame) { return 1.0; }

        void compute_model_dist();

};

#endif