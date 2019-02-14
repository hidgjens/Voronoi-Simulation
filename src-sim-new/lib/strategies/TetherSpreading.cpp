#include "TetherSpreading.h"
#include <vector>
#include <algorithm>

TetherSpreading::TetherSpreading() {}

TetherSpreading::TetherSpreading(TetherSpreading& t) : 
player_count(t.player_count),
max_post_distance(t.max_post_distance) {
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }
}

TetherSpreading& TetherSpreading::operator=(TetherSpreading& t) {
    if (&t == this) {
        return *this;
    }

    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;
    players_to_consider = t.players_to_consider;
    samples_to_run = t.samples_to_run;
    edge_coefficient = t.edge_coefficient;
    ally_coefficient = t.ally_coefficient;

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }

    return *this;
}

TetherSpreading& TetherSpreading::operator=(TetherSpreading&& t) {
    if (&t == this) {
        return *this;
    }
    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    pitch_data = t.pitch_data;
    players_to_consider = t.players_to_consider;
    samples_to_run = t.samples_to_run;
    edge_coefficient = t.edge_coefficient;
    ally_coefficient = t.ally_coefficient;
    min_team_distance = t.min_team_distance;
    pitch_data = t.pitch_data;
    description = t.description;

    player_posts = std::move(t.player_posts);
    t.player_count = 0;

    return *this;
    
}

TetherSpreading::TetherSpreading(TeamConfigFile tcf, Pitch* p) :  
player_count(tcf.player_count)
{
    players_to_consider = tcf.players_to_consider;
    samples_to_run = tcf.maxi_samples;
    edge_coefficient = tcf.edge_coefficient;
    ally_coefficient = tcf.ally_coefficient;
    min_team_distance = tcf.min_team_dist;
    pitch_data = p;
    
    // create player posts array
    player_posts = std::make_unique<Cart[]>(player_count);

    int n; // temporary index
    double t_dx, t_dy; // temp x,y

    // need to compute post positions (in units of pitch dims [-0.5, 0.5])
    // bottom row
    for (int i{0}; i < 4; i++ ) { // i = 0,1,2,3 -> n = 1,3,5,7  =>  n = 2i + 1
        // map i to n
        n = 2*i + 1;
        
        // calculate post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.25 - 0.5; // use -0.5 to map [0, 1] to [-0.5, 0.5]

        // store
        player_posts[i] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());

    }
    // middle row
    for (int i{4}; i < 7; i++) { // i = 4,5,6 -> n = 2,4,6  =>  n = 2*(i-3)
        // map to n
        n = 2*(i-3);

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.5 - 0.5;

        // store
        player_posts[i] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
    }
    // top row
    for (int i{7}; i < 11; i++) { // i = 7,8,9,10 -> n = 1,3,5,7    =>    n = 2*(i - 7) + 1
        // map to n
        n = 2 * (i-7) + 1;

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.75 - 0.5;

        // store 
        player_posts[i] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
    }
}

void TetherSpreading::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount(); i++){
        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();


        ///////// Check distance to post
        auto post = player_posts[i];
        if (player_position.dist(post) > max_post_distance) {
            // player has moved too far from their post, move back towards it
            auto dPos = player_position.unitVect2(post) * plyr->getStepSize();
            plyr->changePositionBy(dPos);
            continue;
        }
        ////////


        ////////// CHECK NEAREST PLAYER
        // check if too close to friendly
        auto nearest_position = frame.getNearestAllyPos(plyr->isHomeTeam(), plyr);

        if (player_position.dist(nearest_position) < min_team_distance * plyr->getStepSize()) {
            // player is too close to ally, scatter it
            auto scatter_displacement = nearest_position.unitVect2(player_position); // defined the reverse was round so it points AWAY from "nearest position"
            scatter_displacement *= plyr->getStepSize(); // distance = max step
            plyr->changePositionBy(scatter_displacement);
            continue;
        }
        //////////// DONE

        // iterate through players and apply exchange method individually
        spreadingMethod(plyr, frame);
        // std::cout << plyr.getShirtNum() << std::endl; 
    }
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

bool sort_dist(PlayerInfo, PlayerInfo);
bool sort_samples(std::pair<Cart,double>, std::pair<Cart,double>);

// bool sort_dist(PlayerInfo i, PlayerInfo j) {
    // return i.distance < j.distance;
// }

// bool sort_samples(std::pair<Cart, double> i, std::pair<Cart, double> j) {
    // return(i.second > j.second);
// }

void TetherSpreading::spreadingMethod(Player* plyr, Frame frame) {
    // get info from player
    bool home_team = plyr->isHomeTeam();
    Cart plyr_pos = plyr->getPosition();

    // vector to store all the objects on the pitch to consider 
    std::vector<PlayerInfo> players_on_pitch;

    // need to populate vector with players from each team and the nearby edges

    // temporary variables
    double temp_dist; Cart temp_pos;

    // ally team first
    for (int i{0}; i < frame.getAlliedCount(home_team); i++) {
        if (i != plyr->getShirtNum()){
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
        dx = plyr->getStepSize() * cos(angle);
        dy = plyr->getStepSize() * sin(angle);
        
        test_pos = plyr_pos + Cart(dx, dy);

        // check if position is allowed
        if (fabs(test_pos.xComp()) > pitch_data->getXlim() || fabs(test_pos.yComp()) > pitch_data->getYlim()){
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
        plyr->changePositionTo(sample_results[0].first);
    }

}

double TetherSpreading::distanceToXedge(Cart pos) {
    return pitch_data->getXlim() - fabs(pos.xComp());
}
double TetherSpreading::distanceToYedge(Cart pos) {
    return pitch_data->getYlim() - fabs(pos.yComp());
}

Cart TetherSpreading::XedgePosition(Cart pos) {
    Cart x_edge_pos;

    if (pos.xComp() >= 0) {
        // positive x quadrant
        x_edge_pos = Cart(pitch_data->getXlim(), pos.yComp());
    } else {
        // negative x quadrant
        x_edge_pos = Cart( -1.0 * pitch_data->getXlim(), pos.yComp());
    }

    return x_edge_pos;
}

Cart TetherSpreading::YedgePosition(Cart pos) {
    Cart y_edge_pos;

    if (pos.yComp() >= 0) {
        // positive y quadrant
        y_edge_pos = Cart(pos.xComp(), pitch_data->getYlim());
    } else {
        // negative y quadrant
        y_edge_pos = Cart(pos.xComp(), -1.0 * pitch_data->getYlim());
    }

    return y_edge_pos;

}

double TetherSpreading::computeD(std::vector<PlayerInfo> nearby_players, Cart test_pos) {
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