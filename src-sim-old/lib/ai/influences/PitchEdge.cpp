#include "PitchEdge.h"

PitchEdge::PitchEdge()
    {}
PitchEdge::PitchEdge(Cart cntr_vect, Cart dir_vect) 
    :   cntr_pnt_vector(cntr_vect)
    ,   direction_vector(dir_vect)
    {}
PitchEdge::PitchEdge(Cart cntr_vect, Cart dir_vect, std::string nm)
    :   cntr_pnt_vector(cntr_vect)
    ,   direction_vector(dir_vect)
    ,   edge_name(nm)
    {}

// accessors and mutators

void PitchEdge::setCentrePoint(Cart new_vect) { cntr_pnt_vector = new_vect; }
Cart PitchEdge::getCentrePoint() const { return cntr_pnt_vector; }

void PitchEdge::setDirection(Cart new_vect) { direction_vector = new_vect; }
Cart PitchEdge::getDirection() const { return direction_vector; }

void PitchEdge::setName(std::string nm) { edge_name = nm; }
std::string PitchEdge::getName() const { return edge_name; }

std::pair<Cart, Cart> PitchEdge::getLine() const { return std::make_pair(cntr_pnt_vector, direction_vector); }

double PitchEdge::perpendicularDistance(Cart pos) const {
/* use fact that |A x B| = |A||B|sin(theta_AB)
    and d = |A|sin(theta_AB) if A = P->C (player position P, centre point C, and direction_vector, B).
    Therefore can use d = |(P->C) x B|/|B| (for convenience, we have already set |B| = 1)

    let PC = P->C
        |PC x B| = |(PC_x, PC_y, 0) x (B_x, B_y, 0)|
            = |det[[PC_x, PC_y], [B_x, B_y]]|
            = |PC_x * B_y - PC_y * B_x|
            = |(C_x - P_x) * B_y - (C_y - P_x) * B_x|

    So provided B is a unit vector (which it is) then
        d = |(C_x - P_x) * B_y - (C_y - P_y) * B_x|
    which is easy to calculate, and B and C are easy to define.

*/
    return std::fabs((cntr_pnt_vector.xComp() - pos.xComp()) * direction_vector.yComp() - (cntr_pnt_vector.yComp() - pos.yComp()) * direction_vector.xComp());

}