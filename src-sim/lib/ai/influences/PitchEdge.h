#ifndef PITCHEDGE_H
#define PITCHEDGE_H

#include "../../vect/Cart.h"

class PitchEdge {
private:
    Cart cntr_pnt_vector;
    Cart direction_vector;
    std::string edge_name;

public:
    PitchEdge();
    PitchEdge(Cart, Cart);
    PitchEdge(Cart, Cart, std::string);

    // accessors and mutators
    void setCentrePoint(Cart);
    Cart getCentrePoint() const;

    void setDirection(Cart);
    Cart getDirection() const;

    void setName(std::string);
    std::string getName() const;

    std::pair<Cart, Cart> getLine() const; // centre point, direction

    double perpendicularDistance(Cart) const;


};

struct PitchBorder {
    public:
    PitchEdge leftEdge;
    PitchEdge rightEdge;
    PitchEdge homeEdge;
    PitchEdge awayEdge;

    Cart pitchDims;

    void configure(){
        leftEdge = PitchEdge(Cart(0.0, 0.5 * pitchDims.yComp()), Cart(1.0, 0.0), "left edge");
        rightEdge = PitchEdge(Cart(0.0, - 0.5 * pitchDims.yComp()), Cart(-1.0, 0.0), "right edge");
        homeEdge = PitchEdge(Cart(-0.5 * pitchDims.xComp(), 0.0), Cart(0.0, 1.0), "home edge");
        awayEdge = PitchEdge(Cart(0.5 * pitchDims.xComp(), 0.0), Cart(0.0, -1.0), "away edge");
    }

    PitchEdge getClosestEdge(Cart pos) {
        double shortest = homeEdge.perpendicularDistance(pos);
        PitchEdge closestedge = homeEdge;

        // check for better
        if (rightEdge.perpendicularDistance(pos) < shortest) {closestedge = rightEdge; shortest = rightEdge.perpendicularDistance(pos); }
        if (leftEdge.perpendicularDistance(pos) < shortest) {closestedge = leftEdge; shortest = leftEdge.perpendicularDistance(pos); }
        if (awayEdge.perpendicularDistance(pos) < shortest) {closestedge = awayEdge; shortest = awayEdge.perpendicularDistance(pos); }

        return closestedge;
    }

    PitchEdge getSecondClosestEdge(Cart pos) {
        // start with home, right, left, away
        double shortest = homeEdge.perpendicularDistance(pos);
        PitchEdge closestedge = homeEdge;
        double secondshortest;
        PitchEdge secondclosestedge;

        if (rightEdge.perpendicularDistance(pos) < shortest) {
            secondclosestedge = closestedge; secondshortest = shortest; // move first to second
            closestedge = rightEdge; shortest = rightEdge.perpendicularDistance(pos); // store new first
        } else {
            // no second yet, so wins it by default
            secondclosestedge = rightEdge; secondshortest = rightEdge.perpendicularDistance(pos);
        }

        if (leftEdge.perpendicularDistance(pos) < shortest) {
            secondclosestedge = closestedge; secondshortest = shortest; // move first to second
            closestedge = leftEdge; shortest = leftEdge.perpendicularDistance(pos); // store new first
        } else {
            // check if it beats second
            if (leftEdge.perpendicularDistance(pos) < secondshortest) {
                secondclosestedge = leftEdge; secondshortest = leftEdge.perpendicularDistance(pos);
            }
        }

        if (awayEdge.perpendicularDistance(pos) < shortest) {
            secondclosestedge = closestedge; secondshortest = shortest; // move first to second
            closestedge = awayEdge; shortest = awayEdge.perpendicularDistance(pos); // store new first
        } else {
            // check if it beats second
            if (awayEdge.perpendicularDistance(pos) < secondshortest) {
                secondclosestedge = awayEdge; secondshortest = awayEdge.perpendicularDistance(pos);
            }
        }

        return(secondclosestedge);
    }

    PitchBorder() : pitchDims(105.0, 68.0) { configure(); }
    PitchBorder(double x, double y) : pitchDims(x,y) { configure(); }
};

#endif