#ifndef TETHERGRID_H
#define TETHERGRID_H

#include "../TeamStrategy.h"
#include<memory>


class TetherGrid: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> home_player_posts;
        std::unique_ptr<Cart[]> away_player_posts;
        double max_post_distance;

        double decay_coefficient;

        double sigma_scale;
        int free_agents;



    public:
        TetherGrid();
        TetherGrid(TeamConfigFile, PitchModel*);

        TetherGrid(TetherGrid&);
        TetherGrid(TetherGrid&& t) { *this = std::move(t); }

        TetherGrid& operator=(TetherGrid&);
        TetherGrid& operator=(TetherGrid&&);

        void updateTeam(Team&, Frame);

        void exchange_method(Player*, Frame&);
        double metric(double, double); // distance, control


};

#endif