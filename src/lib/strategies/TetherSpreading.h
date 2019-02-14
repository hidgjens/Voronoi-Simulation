#ifndef TETHERSPREADING_H
#define TETHERSPREADING_H

#include "../TeamStrategy.h"
#include<memory>
#include<vector>

struct PlayerInfo;



class TetherSpreading: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double edge_coefficient;
        double ally_coefficient;
        int players_to_consider;
        int samples_to_run;



    public:
        TetherSpreading();
        TetherSpreading(TeamConfigFile, Pitch*);

        TetherSpreading(TetherSpreading&);
        TetherSpreading(TetherSpreading&& t) { *this = std::move(t); }

        TetherSpreading& operator=(TetherSpreading&);
        TetherSpreading& operator=(TetherSpreading&&);

        void updateTeam(Team&, Frame);

        void spreadingMethod(Player*, Frame);

        double distanceToXedge(Cart);
        double distanceToYedge(Cart);

        Cart XedgePosition(Cart);
        Cart YedgePosition(Cart);

        double computeD(std::vector<PlayerInfo>, Cart);


};

#endif