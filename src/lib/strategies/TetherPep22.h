#ifndef TETHERPEP22_H
#define TETHERPEP22_H

#include "../TeamStrategy.h"
#include<memory>

class TetherPep22: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> corners1;
        std::unique_ptr<Cart[]> corners2;
        std::unique_ptr<Cart[]> home_player_posts;
        std::unique_ptr<Cart[]> away_player_posts;
        std::unique_ptr<double[]> post_distances;
        std::unique_ptr<double[]> spring_constants;

        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

    public:
        TetherPep22();
        TetherPep22(TeamConfigFile, PitchModel*);

        TetherPep22(TetherPep22&);
        TetherPep22(TetherPep22&& t) { *this = std::move(t); }

        TetherPep22& operator=(TetherPep22&);
        TetherPep22& operator=(TetherPep22&&);

        void updateTeam(Team&, Frame);

        void TetherPepMethod(Player*, Frame&);

        Cart GetMetricVector(Player*, Cart, double); // test player, source position, source control
};

#endif