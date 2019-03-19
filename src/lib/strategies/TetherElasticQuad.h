#ifndef TETHERELASTICQUAD_H
#define TETHERELASTICQUAD_H

#include "../TeamStrategy.h"
#include<memory>


class TetherElasticQuad: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double decay_coefficient;

        double spring_constant;

    public:
        TetherElasticQuad();
        TetherElasticQuad(TeamConfigFile, PitchModel*);

        TetherElasticQuad(TetherElasticQuad&);
        TetherElasticQuad(TetherElasticQuad&& t) { *this = std::move(t); }

        TetherElasticQuad& operator=(TetherElasticQuad&);
        TetherElasticQuad& operator=(TetherElasticQuad&&);

        void updateTeam(Team&, Frame);

        void exchange_method(Player*, Frame&);
        double metric(double, double); // distance, control
        double KR(double); // r


};

#endif