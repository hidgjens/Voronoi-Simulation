#ifndef TETHERELASTICSPREADING_H
#define TETHERELASTICSPREADING_H

#include "../TeamStrategy.h"
#include<memory>

struct PlayerInfo_TES;

class TetherElasticSpreading: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;


        double spring_constant;

        double edge_coefficient;
        double ally_coefficient;
        int players_to_consider;
        int samples_to_run;

    public:
        TetherElasticSpreading();
        TetherElasticSpreading(TeamConfigFile, PitchModel*);

        TetherElasticSpreading(TetherElasticSpreading&);
        TetherElasticSpreading(TetherElasticSpreading&& t) { *this = std::move(t); }

        TetherElasticSpreading& operator=(TetherElasticSpreading&);
        TetherElasticSpreading& operator=(TetherElasticSpreading&&);

        void updateTeam(Team&, Frame);

                void spreadingMethod(Player*, Frame);

        double distanceToXedge(Cart);
        double distanceToYedge(Cart);

        Cart XedgePosition(Cart);
        Cart YedgePosition(Cart);

        double computeD(std::vector<PlayerInfo_TES>, Cart);

        double KR(double); // r


};

#endif