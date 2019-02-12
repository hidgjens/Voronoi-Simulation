#ifndef PLAYER_H
#define PLAYER_H

#include "Cart.h"
#include<iostream>


class Team;

class Player{
    friend class Match;
    private:
        Cart position;
        int shirt_number;
        double current_control;

        double max_step_size = 2.5;

        Team* team;

    public:
        Player();
        Player(int);
        Player(int, Cart);

        int getShirtNum() const;
        void setShirtNum(int);

        double getControl() const;
        void setControl(double);

        double getStepSize() const;
        void setStepSize(double);

        Team* getTeamPtr();
        void setTeamPtr(Team*);

        bool isHomeTeam() const;
        bool isSameTeam(Player&) const;

        Cart getPosition() const;
        void changePositionBy(double, double);
        void changePositionBy(Cart);
        void changePositionTo(double, double);
        void changePositionTo(Cart);

        void setInitialPosition(Cart);

        void printPos() {
            position.print();
        }

        void setZeroControl();
        void addControl(double);
        void normControl(double);

        

        
};

#endif