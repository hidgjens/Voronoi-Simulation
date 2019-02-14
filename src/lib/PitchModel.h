/*
    Class which models the weighting of the pitch

*/

#ifndef PITCHMODEL_H
#define PITCHMODEL_H

#include "Cart.h"
#include "Frame.h"
#include "cfg/MatchConfigFile.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class PitchModel{
    public:
        PitchModel() {}
        static PitchModel* createPitchModel(std::string, MatchConfigFile);
        virtual double pitchWeight(Cart, Frame) { return 1.0; }

};

#endif