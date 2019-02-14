#ifndef TETHER_H
#define TETHER_H

#include "../TeamStrategy.h"
#include<memory>


class Tether: public TeamStrategy{
    private:
        int player_count;
        std::unique_ptr<Cart[]> player_posts;
        double max_post_distance;

        double decay_coefficient;



    public:
        Tether();
        Tether(TeamConfigFile, Pitch*);

        Tether(Tether&);
        Tether(Tether&& t) { *this = std::move(t); }

        Tether& operator=(Tether&);
        Tether& operator=(Tether&&);

        void updateTeam(Team&, Frame);

        void exchange_method(Player*, Frame&);
        double metric(double, double); // distance, control


};

#endif