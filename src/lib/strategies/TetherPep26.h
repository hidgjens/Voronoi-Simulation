#ifndef TETHERPEP26_H
#define TETHERPEP26_H

#include "../TeamStrategy.h"
#include<memory>

class TetherPep26: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> region_corners_top;
        std::unique_ptr<Cart[]> region_corners_bottom;
        std::unique_ptr<Cart[]> home_player_posts;
        std::unique_ptr<Cart[]> away_player_posts;
        std::unique_ptr<double[]> post_distances;

        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

    public:
        TetherPep26();
        TetherPep26(TeamConfigFile, PitchModel*);

        TetherPep26(TetherPep26&);
        TetherPep26(TetherPep26&& t) { *this = std::move(t); }

        TetherPep26& operator=(TetherPep26&);
        TetherPep26& operator=(TetherPep26&&);

        void updateTeam(Team&, Frame);

        void TetherPepMethod(Player*, Frame&);

        Cart GetMetricVector(Player*, Cart, double); // test player, source position, source control
};

#endif