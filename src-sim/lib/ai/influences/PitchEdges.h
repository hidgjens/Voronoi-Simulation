#ifndef PITCHEDGES_H
#define PITCHEDGES_H

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#include "../../Player.h"
#include "../../Match.h"
#include <vector>


struct PitchEdgeParams{
    public:
        double x_lim = 20.0;
        double y_lim = 20.0;
        double rho_0 = 3.0;
        PitchEdgeParams() {}
        PitchEdgeParams(double x, double y, double r) : x_lim(x), y_lim(y), rho_0(r) {}
} pitch_params;

struct PitchEdge {
    public:
        double norm_angle;
        int zone;
        double lim ;

        PitchEdge(double norm_a, int zn, double l) : norm_angle(norm_a), zone(zn), lim(l) {}
} HomeGoal(-1.0 * M_PI, 4, pitch_params.x_lim), LeftEdge(+0.5 * M_PI, 2, pitch_params.y_lim), AwayGoal(0.0 * M_PI, 6, pitch_params.x_lim), RightEdge(-0.5 * M_PI, 8, pitch_params.y_lim);

int determine_pitch_zone(Player* player, Match* match){
    // get pitch dimensions
    double pitchX = match->getPitchX();
    double pitchY = match->getPitchY();
    // get player position
    Cart player_pos = player->getPos();

    // check for zone 5, easiest one
    if (pitchX/2.0 - std::fabs(player_pos.xComp()) >= pitch_params.x_lim && pitchY/2.0 - std::fabs(player_pos.yComp()) >= pitch_params.y_lim) return 5; // zone 5
    // 2 + 8 work the same, check for those
    if (pitchX/2.0 - std::fabs(player_pos.xComp()) >= pitch_params.x_lim && pitchY/2.0 - std::fabs(player_pos.yComp()) < pitch_params.y_lim) {
        if (player_pos.yComp() < 0.0) return 8; // zone 8
        else return 2; // zone 2
    }
    // 4 or 6
    if (pitchX/2.0 - std::fabs(player_pos.xComp()) < pitch_params.x_lim && pitchY/2.0 - std::fabs(player_pos.yComp()) >= pitch_params.y_lim) {
        if (player_pos.xComp() < 0.0) return 4; // zone 4
        else return 6; // zone 6
    }
    // 1,3,7,9
    if (pitchX/2.0 - std::fabs(player_pos.xComp()) < pitch_params.x_lim && pitchY/2.0 - std::fabs(player_pos.yComp()) < pitch_params.y_lim) {
        if (player_pos.xComp() < 0.0) {
            // 1 and 7
            if (player_pos.yComp() < 0.0) return 7;
            else return 1;
        }
        else {
            // 3 and 9
            if (player_pos.yComp() < 0.0) return 9;
            else return 3;
        }
    }
}

std::pair<double, double> edgeEffect(PitchEdge& edge_, double dist2edge_) {
    return std::make_pair(pitch_params.rho_0 * (1.0 - dist2edge_ / edge_.lim), edge_.norm_angle);
}

std::vector<std::pair<double, double>> calc_edge_terms(Player* player, Match* match) {
    std::vector<std::pair<double, double>> edges;

    // work out which zone the player is in
    int res = determine_pitch_zone(player, match);
    //player->printPos();
    //std::cout << res << "\n";
    
    switch (res){
        case 1: {
            // left home corner
            // calc dist2 home edge and left edge
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(HomeGoal, dist2shortedge));
            edges.push_back(edgeEffect(LeftEdge, dist2longedge));
            break;
        }
        case 2: {
            // left side
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            // add edges
            edges.push_back(edgeEffect(LeftEdge, dist2longedge));
            break;
        }
        case 3: {
            // left away corner
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(AwayGoal, dist2shortedge));
            edges.push_back(edgeEffect(LeftEdge, dist2longedge));
            break;
        }
        case 4: {
            // home edge
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(HomeGoal, dist2shortedge));
            break;
        }
        case 5: {
            // middle, no affect
            edges.push_back(std::make_pair(0.0, 0.0));
            break;
        }
        case 6: {
            // away edge
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(AwayGoal, dist2shortedge));
            break;
        }
        case 7: {
            // home right edge
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(HomeGoal, dist2shortedge));
            edges.push_back(edgeEffect(RightEdge, dist2longedge));
            break;
        }
        case 8: {
            // right edge
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            // add edges
            edges.push_back(edgeEffect(RightEdge, dist2longedge));
            break;
        }
        case 9: {
            // right away corner
            double dist2longedge = match->getPitchY()/2.0 - std::fabs(player->getPosY());
            double dist2shortedge = match->getPitchX()/2.0 - std::fabs(player->getPosX());
            // add edges
            edges.push_back(edgeEffect(AwayGoal, dist2shortedge));
            edges.push_back(edgeEffect(RightEdge, dist2longedge));
            break;
        }
    }
    return edges;
}

#endif