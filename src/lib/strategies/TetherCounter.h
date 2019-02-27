#ifndef TETHERCOUNTER_H
#define TETHERCOUNTER_H

#include "../TeamStrategy.h"
#include<memory>


class TetherCounter: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double decay_coefficient;



    public:
        TetherCounter();
        TetherCounter(TeamConfigFile, PitchModel*);

        TetherCounter(TetherCounter&);
        TetherCounter(TetherCounter&& t) { *this = std::move(t); }

        TetherCounter& operator=(TetherCounter&);
        TetherCounter& operator=(TetherCounter&&);

        void updateTeam(Team&, Frame);

        void exchange_method(Player*, Frame&);
        double metric(double, double); // distance, control


};

#endif