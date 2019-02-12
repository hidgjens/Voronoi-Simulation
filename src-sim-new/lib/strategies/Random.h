#ifndef RANDOM_H
#define RANDOM_H

#include "../TeamStrategy.h"

class Random: public TeamStrategy{
    public:
        Random();

        void updateTeam(Team&, Frame);

        void randomWalk(Player&);

};

#endif