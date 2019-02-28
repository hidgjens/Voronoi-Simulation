/*
    Frame Class
        Stores information on the pitch state at a given time
*/

#ifndef FRAME_H
#define FRAME_H

#include<memory>
#include "Cart.h"
#include<iostream>


class Player;

class Frame {

    friend class Match;
    
    private:
        // frame data
        int frameNumber;

        int home_player_count;
        std::unique_ptr<Cart[]> homePositions;
        std::unique_ptr<double[]> homeControl;

        int away_player_count;
        std::unique_ptr<Cart[]> awayPositions;
        std::unique_ptr<double[]> awayControl;

        bool home_possession;

    public:
        // constructors
        Frame();
        Frame(int, int, int);

        Frame(Frame&);
        Frame(Frame&& frame) { *this = std::move(frame); }

        Frame& operator=(Frame&);
        Frame& operator=(Frame&&);

        // accessors and mutators
        void setFrameNumber(int);
        int getFrameNumber() const;

        void setPossession(bool);
        bool getPossession() const;
        void togglePossession();

        void setHomePosition(int, Cart); // (shirt number, position)
        Cart getHomePosition(int);

        void setHomeControl(int, double); // (shirt number, control)
        double getHomeControl(int) const;

        void setAwayPosition(int, Cart);
        Cart getAwayPosition(int);

        void setAwayControl(int, double);
        double getAwayControl(int) const;

        void printFrame() const;

        int getOpponentCount(bool);
        Cart getOpponentPosition(bool, int);
        double getOpponentControl(bool, int);

        int getAlliedCount(bool);
        Cart getAlliedPosition(bool, int);
        double getAlliedControl(bool, int);

        Cart getNearestOpponentPos(bool, Cart);
        double getNearestOpponentDist(bool, Cart);
        double getNearestOpponentCtrl(bool, Cart);
        int getNearestOpponentShirtNum(bool, Cart);

        

        Cart getNearestAllyPos(bool, Player*); // need player here to avoid returning the same player
        double getNearestAllyDist(bool, Player*);
        double getNearestAllyCtrl(bool, Player*);
        int getNearestAllyShirtNum(bool, Player*);


        double getHomeTeamControl();
        double getAwayTeamControl();

        std::string getHomePossStr();
        std::string getAwayPossStr();

};

#endif