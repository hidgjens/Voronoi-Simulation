#ifndef TETHERELASTICMETRICN_H
#define TETHERELASTICMETRICN_H

#include "../TeamStrategy.h"
#include<memory>


class TetherElasticMetricN: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

        double spring_constant;

    public:
        TetherElasticMetricN();
        TetherElasticMetricN(TeamConfigFile, PitchModel*);

        TetherElasticMetricN(TetherElasticMetricN&);
        TetherElasticMetricN(TetherElasticMetricN&& t) { *this = std::move(t); }

        TetherElasticMetricN& operator=(TetherElasticMetricN&);
        TetherElasticMetricN& operator=(TetherElasticMetricN&&);

        void updateTeam(Team&, Frame);

        void MetricNMethod(Player*, Frame&);

        Cart MetricNVector(Player*, Cart, double); // test player, source position, source control

        double KR(double); // r


};

#endif