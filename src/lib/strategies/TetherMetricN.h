#ifndef TetherMetricN_H
#define TetherMetricN_H

#include "../TeamStrategy.h"
#include<memory>


class TetherMetricN: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;
        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

    public:
        TetherMetricN();
        TetherMetricN(TeamConfigFile, PitchModel*);

        TetherMetricN(TetherMetricN&);
        TetherMetricN(TetherMetricN&& t) { *this = std::move(t); }

        TetherMetricN& operator=(TetherMetricN&);
        TetherMetricN& operator=(TetherMetricN&&);

        void updateTeam(Team&, Frame);

        void MetricNMethod(Player*, Frame&);

        Cart MetricNVector(Player*, Cart, double); // test player, source position, source control

};

#endif