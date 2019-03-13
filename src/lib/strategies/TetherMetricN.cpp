#include "TetherMetricN.h"
#include<vector>
#include <algorithm> // for std::sort
#include<iostream>

TetherMetricN::TetherMetricN() {}

TetherMetricN::TetherMetricN(TetherMetricN& t) : 
player_count(t.player_count),
max_post_distance(t.max_post_distance),
decay_coefficient(t.decay_coefficient) {
    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description; 

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }
}

TetherMetricN& TetherMetricN::operator=(TetherMetricN& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    decay_coefficient = t.decay_coefficient;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }

    return *this;
}

TetherMetricN& TetherMetricN::operator=(TetherMetricN&& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    decay_coefficient = t.decay_coefficient;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;

    player_posts = std::move(t.player_posts);
    t.player_count = 0;

    return *this;
    
}

TetherMetricN::TetherMetricN(TeamConfigFile tcf, PitchModel* p) : 
decay_coefficient(tcf.decay_coefficient),
attract_coefficient(tcf.attract_coefficient),
repel_coefficient(tcf.repel_coefficient),
player_count(tcf.player_count),
max_post_distance(tcf.max_post_distance),
num_nearest_neighbours(tcf.players_to_consider)

{
    pm = p;
    pitch_data = p->getPitchData();
    
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
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());

    }
    // middle row
    for (int i{4}; i < 7; i++) { // i = 4,5,6 -> n = 2,4,6  =>  n = 2*(i-3)
        // map to n
        n = 2*(i-3);

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.5 - 0.5;

        // store
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());
    }
    // top row
    for (int i{7}; i < 11; i++) { // i = 7,8,9,10 -> n = 1,3,5,7    =>    n = 2*(i - 7) + 1
        // map to n
        n = 2 * (i-7) + 1;

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.75 - 0.5;

        // store 
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());
    }
}

void TetherMetricN::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount(); i++){
        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();


        ///////// Check distance to post
        auto post = player_posts[i];
        if (player_position.dist(post) >= max_post_distance) {
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

        // iterate through players and apply MetricN method individually
        MetricNMethod(plyr, frame);
        //std::cout << "Player considered for metric method:" << std::endl;
        //std::cout << plyr->getShirtNum() << std::endl; 
    }
}


// data structure for player info
struct PlayerInfo{
    bool ally;
    int shirt_num;
    Cart position;
    double distance;

    PlayerInfo(){}

    PlayerInfo(bool a, int sn, Cart p, double d)
    : ally(a)
    , shirt_num(sn)
    , position(p)
    , distance(d)
    {}
};


bool sort_by_distance(PlayerInfo i, PlayerInfo j) {
    return i.distance < j.distance;
}


void TetherMetricN::MetricNMethod(Player* plyr, Frame& frame){
    // get player info
    Cart plyr_position = plyr->getPosition();
    double plyr_control = plyr->getControl();
    bool home_team = plyr->isHomeTeam();

    // initialise opponent and allied total metric vectors
    Cart opp_vector(0.0, 0.0); Cart ally_vector(0.0, 0.0);

    // vector to store all players on pitch
    std::vector<PlayerInfo> players;

    // need to populate vector with players from each team
    // temporary variables
    double temp_dist, temp_ctrl; Cart temp_pos;

    // ally team first
    for (int i{0}; i < frame.getAlliedCount(home_team); i++) {
        if (i != plyr->getShirtNum()){
            // calculate info about player
            temp_pos = frame.getAlliedPosition(home_team, i);
            temp_dist = plyr_position.dist(temp_pos);
            // add to list
            players.push_back(PlayerInfo(true, i, temp_pos, temp_dist));
        }
        else {
            continue; // same player, ignore
        }
    }

    // and now opponent team
    for (int i{0}; i < frame.getOpponentCount(home_team); i++) {
        // get player info
        temp_pos = frame.getOpponentPosition(home_team, i);
        temp_dist = plyr_position.dist(temp_pos);
        // add to list
        players.push_back(PlayerInfo(false, i, temp_pos, temp_dist));
    }

    // now need to sort list by distance
    std::sort(players.begin(), players.end(), sort_by_distance);
    //std::cout << "Made it up to choosing n nearest neighbours" << std::endl;
    
    // find metric vectors for n nearest neighbours
    for (int i{0}; i < num_nearest_neighbours; i++){
        //std::cout << "up to temp player" << std::endl;
        PlayerInfo temp_player = players[i];
        //std::cout << "temp player made" << std::endl;
        //std::cout << "temp player pos:" << std::endl;
        //temp_player.position.print();
        
        // for allies
        if (temp_player.ally == true){
            Cart temp_pos = temp_player.position;
            double temp_ctrl = frame.getAlliedControl(home_team, temp_player.shirt_num);
            ally_vector += MetricNVector(plyr, temp_pos, temp_ctrl) * -1;
            //std::cout << "Ally vector:" << std::endl;
            //ally_vector.print();
        }

        // and for opponents
        else if (temp_player.ally == false){
            Cart temp_pos = temp_player.position;
            double temp_ctrl = frame.getOpponentControl(home_team, temp_player.shirt_num);
            opp_vector += MetricNVector(plyr, temp_pos, temp_ctrl); 
            //std::cout << "Opponent vector:" << std::endl;
            //opp_vector.print();
        }
    }

    // calculate total metric vector
    Cart f_vector = (opp_vector * attract_coefficient) + (ally_vector * repel_coefficient);
    //f_vector.print();

    // move player
    Cart dPos = f_vector.unitVect() * plyr->getStepSize();
    plyr->changePositionBy(dPos);
}


Cart TetherMetricN::MetricNVector(Player* plyr, Cart src_pos, double src_control) {
    Cart test_plyr_pos = plyr->getPosition();
    Cart unit_vector = test_plyr_pos.unitVect2(src_pos);
    double distance = test_plyr_pos.dist(src_pos);
    double pitch_length = pitch_data.getPitchLength();
    //std::cout << "Pitch length" << std::endl;
    //std::cout << pitch_length << std::endl;

    Cart metric_n_vector =  unit_vector * (src_control * exp( - 1.0 * distance / (decay_coefficient * pitch_data.getPitchLength())));
    return metric_n_vector;
}