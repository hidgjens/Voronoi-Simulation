#ifndef TETHERELASTIC_H
#define TETHERELASTIC_H

#include "../TeamStrategy.h"
#include<memory>


class TetherElastic: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double decay_coefficient;

        double spring_constant;

    public:
        TetherElastic();
        TetherElastic(TeamConfigFile, PitchModel*);

        TetherElastic(TetherElastic&);
        TetherElastic(TetherElastic&& t) { *this = std::move(t); }

        TetherElastic& operator=(TetherElastic&);
        TetherElastic& operator=(TetherElastic&&);

        void updateTeam(Team&, Frame);

        void exchange_method(Player*, Frame&);
        double metric(double, double); // distance, control
        double KR(double); // r


};

#endif