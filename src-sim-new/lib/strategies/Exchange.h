#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "../TeamStrategy.h"

class Player;
class TeamConfigFile;

class Exchange: public TeamStrategy{
    private:
        double decay_coefficient;

    public:
        Exchange();
        Exchange(TeamConfigFile, Pitch*);

        void updateTeam(Team&, Frame);

        void exchange_method(Player&, Frame&);
        double metric(double, double); // distance, control


};

#endif