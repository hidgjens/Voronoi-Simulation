/*
    Class which models the weighting of the pitch

*/

#ifndef PITCHMODEL_H
#define PITCHMODEL_H

#include "Cart.h"
#include "Frame.h"
#include "Pitch.h"
#include "PitchGrid.h"
#include "cfg/MatchConfigFile.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class PitchModel{
    protected:

        Pitch pitch_data;
        PitchGrid home_possession_grid, away_possession_grid;

    public:
        PitchModel() {}
        static PitchModel* createPitchModel(std::string, MatchConfigFile);
        virtual double pitchWeight(Cart, Frame) { return 1.0; }

        void compute_model_dist();

        PitchGrid getHomeGrid() const { return home_possession_grid; }
        PitchGrid getAwayGrid() const { return away_possession_grid; }
        Pitch getPitchData() { return pitch_data; }

};

#endif