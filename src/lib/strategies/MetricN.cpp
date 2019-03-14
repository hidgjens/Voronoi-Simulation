#include "MetricN.h"
#include <vector>
#include <algorithm> // std::sort
#include <fstream>
#include <sys/stat.h>

MetricN::MetricN() {}
MetricN::MetricN(TeamConfigFile tcf, PitchModel* p) {
    decay_coefficient = tcf.decay_coefficient;
    attract_coefficient = tcf.attract_coefficient;
    repel_coefficient = tcf.repel_coefficient;
    num_nearest_neighbours = tcf.players_to_consider; 
    min_team_distance = tcf.min_team_dist;
    pm = p;
    pitch_data = pm->getPitchData();
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


void MetricN::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i ++ ) {
        auto plyr = team.getPlayerPtr(i);

        ////////// CHECK NEAREST PLAYER
        // check if too close to friendly
        auto player_position = plyr->getPosition();
        auto nearest_position = frame.getNearestAllyPos(plyr->isHomeTeam(), plyr);

        if (player_position.dist(nearest_position) < min_team_distance * plyr->getStepSize()) {
            // player is too close to ally, scatter it
            auto scatter_displacement = nearest_position.unitVect2(player_position); // defined the reverse was round so it points AWAY from "nearest position"
            scatter_displacement *= plyr->getStepSize(); // distance = max step
            plyr->changePositionBy(scatter_displacement);
            return;
        }
        //////////// DONE

        MetricNMethod(plyr, frame);
    }
}



bool sort_by_dist(PlayerInfo i, PlayerInfo j) {
    return i.distance < j.distance;
}


void MetricN::MetricNMethod(Player* plyr, Frame frame) {
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
    std::sort(players.begin(), players.end(), sort_by_dist);

    // take n nearest neighbours
    std::vector<PlayerInfo> nearest_neighbours;

    for (int i{0}; i < num_nearest_neighbours; i++){
        nearest_neighbours.push_back(players[i]);
    }
    
    // print out status of each player considered to file
    std::ofstream outfile;
    std::string filepath = "data_files/csvs/" + pitch_data.getMatchName();
    std::string filename = std::to_string(pitch_data.getMatchID()) + ".met";
    std::string outpath = filepath + "/" + filename;
    
    // check if folder exists
    struct stat st;
    if(!stat(filepath.c_str(), &st) == 0){
        // directory doesn't exist, make the folder
        system(("mkdir " + filepath).c_str());
    }
    
    // open file
    outfile.open(outpath, std::ios::app);
    
    // write to file
    // format is ally(bool), shirtnum(int), distance(double)
    for (int i{0}; i < num_nearest_neighbours; i++){
        outfile << nearest_neighbours[i].ally << ", " << nearest_neighbours[i].shirt_num << ", " << nearest_neighbours[i].distance << '\n'; 
        //std::cout << nearest_neighbours[i].ally << ',' << nearest_neighbours[i].shirt_num << ',' << nearest_neighbours[i].distance << '\n' << std::endl;
    }


    // get metric vectors
    for (int i{0}; i < num_nearest_neighbours; i++){
        PlayerInfo temp_player = nearest_neighbours[i];

        //temp_player.position.print();
        // for allies
        if (temp_player.ally == true){
            Cart temp_pos = temp_player.position;
            double temp_ctrl = frame.getAlliedControl(home_team, temp_player.shirt_num);
            ally_vector += MetricNVector(plyr, temp_pos, temp_ctrl) * -1;
            //ally_vector.print();
        }

        // and for opponents
        else if (temp_player.ally == false){
            Cart temp_pos = temp_player.position;
            double temp_ctrl = frame.getOpponentControl(home_team, temp_player.shirt_num);
            opp_vector += MetricNVector(plyr, temp_pos, temp_ctrl); 
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


Cart MetricN::MetricNVector(Player* plyr, Cart src_pos, double src_control) {
    Cart test_plyr_pos = plyr->getPosition();
    Cart unit_vector = test_plyr_pos.unitVect2(src_pos);
    double distance = test_plyr_pos.dist(src_pos);
    double pitch_length = pitch_data.getPitchLength();
    
    Cart metric_n_vector =  unit_vector * (src_control * exp( - 1.0 * distance / (decay_coefficient * pitch_data.getPitchLength())));
    return metric_n_vector;
}