#include "Spreading.h"
#include <vector>
#include <algorithm> // std::sort

Spreading::Spreading() {}
Spreading::Spreading(TeamConfigFile tcf) {
    players_to_consider = tcf.players_to_consider;
    samples_to_run = tcf.maxi_samples;
    edge_coefficient = tcf.edge_coefficient;
    ally_coefficient = tcf.ally_coefficient;
}

// data structure for player information
struct PlayerInfo{
    bool ally;
    int shirt_num;
    Cart position;
    double distance;

    bool edge = false;
    bool y_edge = false;


    PlayerInfo(){}

    PlayerInfo(bool a, int sn, Cart p, double d)
    : ally(a)
    , shirt_num(sn)
    , position(p)
    , distance(d)
    {}

};

bool sort_dist(PlayerInfo i, PlayerInfo j) {
    return i.distance < j.distance;
}

bool sort_samples(std::pair<Cart, double> i, std::pair<Cart, double> j) {
    return(i.second > j.second);
}

void Spreading::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i++) {
        spreadingMethod(team.getPlayer(i), frame);
    }
}

void Spreading::spreadingMethod(Player& plyr, Frame frame) {
    // get info from player
    bool home_team = plyr.isHomeTeam();
    Cart plyr_pos = plyr.getPosition();

    // vector to store all the objects on the pitch to consider 
    std::vector<PlayerInfo> players_on_pitch;

    // need to populate vector with players from each team and the nearby edges

    // temporary variables
    double temp_dist; Cart temp_pos;

    // ally team first
    for (int i{0}; i < frame.getAlliedCount(home_team); i++) {
        if (i != plyr.getShirtNum()){
            // calculate info about player
            temp_pos = frame.getAlliedPosition(home_team, i);
            temp_dist = plyr_pos.dist(temp_pos);
            // add to list
            players_on_pitch.push_back(PlayerInfo(true, i, temp_pos, temp_dist));
        } else {
            continue; // same player
        }
    }

    // opponent team
    for (int i{0}; i < frame.getOpponentCount(home_team); i++) {
        temp_pos = frame.getOpponentPosition(home_team, i);
        temp_dist = plyr_pos.dist(temp_pos);
        // add to list
        players_on_pitch.push_back(PlayerInfo(false, i, temp_pos, temp_dist));
    }


    // construct "player info" objects for the walls

    auto x_edge = PlayerInfo(true, 0, XedgePosition(plyr_pos), distanceToXedge(plyr_pos));
    auto y_edge = PlayerInfo(true, 0, YedgePosition(plyr_pos), distanceToYedge(plyr_pos));
    x_edge.edge = true; y_edge.edge = true; y_edge.y_edge = true;

    // add to vector
    players_on_pitch.push_back(x_edge); players_on_pitch.push_back(y_edge);

    // sort vector by distances (smallest first)
    std::sort(players_on_pitch.begin(), players_on_pitch.end(), sort_dist);

    // create a new vector to store the closest of these players
    std::vector<PlayerInfo> closest_players;

    // add the closest players
    for (int i{0}; i < players_to_consider; i++){
        closest_players.push_back(players_on_pitch[i]);
    }

    /* 
        We now have a vector with the nearest objects.
        The objects store the positions, distances, and type
        so now we can maximise a function summed over these
    */

    /*
        Going to maximise the sum of distances (D) to these object.
        Maximising will be done by sampling the area around the player.
        For simplicity, consider only the positions at distance R = max_step,
        and therefore shall sample regular intervals on a circle.
    */

    // need a vector to store the positions we test and the value of D found
    std::vector<std::pair<Cart, double>> sample_results;

    // store initial value for comparison
    double initial_D = computeD(closest_players, plyr_pos);

    // temporary variables
    double angle, dx, dy, tempD; Cart test_pos;

    // run samples, and compute D. Store the results
    for (int i{0}; i < samples_to_run; i++) {
        angle = (((double) i )/ samples_to_run) * 2 * M_PI;
        dx = plyr.getStepSize() * cos(angle);
        dy = plyr.getStepSize() * sin(angle);
        
        test_pos = plyr_pos + Cart(dx, dy);

        // check if position is allowed
        if (fabs(test_pos.xComp()) > pitch_data->x_limit || fabs(test_pos.yComp()) > pitch_data->y_limit){
            // out of range
            continue;
        }

        tempD = computeD(closest_players, test_pos);
        
        // store
        sample_results.push_back(std::make_pair(test_pos, tempD));
    }

    // sort results
    std::sort(sample_results.begin(), sample_results.end(), sort_samples);

    // check if best position improves D, if so move there
    if (sample_results[0].second > initial_D) {
        plyr.changePositionTo(sample_results[0].first);
    }

}

double Spreading::distanceToXedge(Cart pos) {
    return pitch_data->x_limit - fabs(pos.xComp());
}
double Spreading::distanceToYedge(Cart pos) {
    return pitch_data->y_limit - fabs(pos.yComp());
}

Cart Spreading::XedgePosition(Cart pos) {
    Cart x_edge_pos;

    if (pos.xComp() >= 0) {
        // positive x quadrant
        x_edge_pos = Cart(pitch_data->x_limit, pos.yComp());
    } else {
        // negative x quadrant
        x_edge_pos = Cart( -1.0 * pitch_data->x_limit, pos.yComp());
    }

    return x_edge_pos;
}

Cart Spreading::YedgePosition(Cart pos) {
    Cart y_edge_pos;

    if (pos.yComp() >= 0) {
        // positive y quadrant
        y_edge_pos = Cart(pos.xComp(), pitch_data->y_limit);
    } else {
        // negative y quadrant
        y_edge_pos = Cart(pos.xComp(), -1.0 * pitch_data->y_limit);
    }

    return y_edge_pos;

}

double Spreading::computeD(std::vector<PlayerInfo> nearby_players, Cart test_pos) {
    double D = 0.0;
    
    // iterate through edges, calculate distances and sum
    for (auto obj_info : nearby_players) {
        // identify object
        if (obj_info.edge) {
            // this is an edge
            // the position of an edge must be recalculated
            if (obj_info.y_edge)
                // y edge
                D += edge_coefficient * YedgePosition(test_pos).dist(test_pos);
            else
                // x edge
                D += edge_coefficient * XedgePosition(test_pos).dist(test_pos);
            
        } else if (obj_info.ally) {
            // object is an ally
            D += ally_coefficient * obj_info.position.dist(test_pos);
        } else {
            // this is an opponent
            D += obj_info.position.dist(test_pos);
        }
    }
    return D;
}