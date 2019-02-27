#ifndef PITCHGRID_H
#define PITCHGRID_H

/*
    A class to store the grid boundaries for tether strategies
*/

#include "Cart.h"
#include "Pitch.h"

class PitchModel;

class PitchGrid{
    private:
        bool possession; // which value of possession this grid corresponds to
        double convert_x_idx_to_coord(int);
        double convert_y_idx_to_coord(int);
        Pitch pitch_data;
        Cart mean_position;
        double dx_plus, dx_minus, dy_plus, dy_minus;

    public:
        PitchGrid() {}
        PitchGrid(Pitch, PitchModel*, bool); // bool = possession
        // accessors/mutators
        bool getPossession() const { return possession; }
        void setPossession(bool p) { possession = p; }
        Pitch getPitchData() const { return pitch_data; }
        void setPitchData(Pitch p) { pitch_data = p; }

        Cart getMeanPosition() const { return mean_position; }
        double getDXPLUS() const { return dx_plus; }
        double getDXMINUS() const { return dx_minus; }
        double getDYPLUS() const { return dy_plus; }
        double getDYMINUS() const { return dy_minus; }

        void computeGrid(PitchModel*, bool); // bool = possession

};

#endif