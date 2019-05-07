#include "Metric3.h"
#include <vector>
#include <algorithm> // std::sort
#include <fstream>
#include <sys/stat.h>


Metric3::Metric3() {}
Metric3::Metric3(TeamConfigFile tcf, PitchModel* p) {
    decay_coefficient = tcf.decay_coefficient;
    attract_coefficient = tcf.attract_coefficient;
    repel_coefficient = tcf.repel_coefficient;
    min_team_distance = tcf.min_team_dist;
    num_nearest_neighbours = tcf.players_to_consider;
    pm = p;
    pitch_data = pm->getPitchData();
}


// data structure for player info
struct MetricPlayerInfo{
    bool ally;
    int shirt_num;
    Cart position;
    double distance;
    double metric;

    MetricPlayerInfo(){}

    MetricPlayerInfo(bool a, int sn, Cart p, double d, double m)
    : ally(a)
    , shirt_num(sn)
    , position(p)
    , distance(d)
    , metric(m)
    {
        //std::cout << "Metric value (in player info constructor):" << std::endl;
        //std::cout << metric << std::endl;
    }
};


void Metric3::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i ++ ) {
        auto plyr = team.getPlayerPtr(i);
        //std::cout << "Player number:" <<std::endl;
        //std::cout << i << std::endl;
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

        Metric3Method(plyr, frame);
    }
}


bool sort_by_metric(MetricPlayerInfo i, MetricPlayerInfo j) {
    return i.metric > j.metric;
}

bool sort_by_distance(MetricPlayerInfo i, MetricPlayerInfo j) {
    return i.distance < j.distance;
}


// want to attract to n (n=2 in config) highest metric opponents
// and repel from closest teammate
void Metric3::Metric3Method(Player* plyr, Frame frame) {
    
    // get player info
    Cart plyr_position = plyr->getPosition();
    double plyr_control = plyr->getControl();
    bool home_team = plyr->isHomeTeam();

    // initialise opponent and allied total metric vectors
    Cart opp_vector(0.0, 0.0); Cart ally_vector(0.0, 0.0);
    
    // define vector to store opponent players
    std::vector<MetricPlayerInfo> players;
    // temp vars
    double temp_dist{0}, temp_ctrl{0}, temp_metric{0}; Cart temp_pos(0.0, 0.0);

    // need to populate vector with opponents
    for (int i{0}; i < frame.getOpponentCount(home_team); i++) {
        
        // get player info
        temp_pos = frame.getOpponentPosition(home_team, i);
        temp_dist = plyr_position.dist(temp_pos);
        temp_ctrl = frame.getOpponentControl(home_team, i);
        //std::cout << "Metric init:" << std::endl;
        //std::cout << temp_metric << std::endl;
        temp_metric = temp_ctrl * exp(-1.0 * temp_dist / (decay_coefficient * pitch_data.getPitchLength()));
        //std::cout << "Metric calc (val given to constructor):" << std::endl;
        //std::cout << temp_metric << std::endl;
        
        // add to list
        players.push_back(MetricPlayerInfo(false, i, temp_pos, temp_dist, temp_metric));
    }
    
    // sort by dist
    std::sort(players.begin(), players.end(), sort_by_distance);
    // now need to sort list by metric
    //std::sort(players.begin(), players.end(), sort_by_metric);

    /*for (auto x: players){
        x.position.print();
        std::cout << x.metric << std::endl;
    }*/

    /*
    // vector of nearest opponents to compare to highest metric
    std::vector<MetricPlayerInfo> nearest_neighbours;
    for (MetricPlayerInfo pi : players){
        if (pi.ally == 0)
            nearest_neighbours.push_back(pi);
    }
    // sort by distance then resize to number of players we need
    std::sort(nearest_neighbours.begin(), nearest_neighbours.end(), sort_by_distance);
    nearest_neighbours.resize(num_nearest_neighbours);

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
        outfile << nearest_neighbours[i].ally << ", " << nearest_neighbours[i].shirt_num << ", " << nearest_neighbours[i].distance << ", ";
        outfile << players[i].ally << ", " << players[i].shirt_num << ", " << players[i].distance << "\n";
        //std::cout << nearest_neighbours[i].ally << ',' << nearest_neighbours[i].shirt_num << ',' << nearest_neighbours[i].distance << '\n' << std::endl;
    }
    */

    // get metric vectors for opponents
    for (int i{0}; i < num_nearest_neighbours; i++){
        MetricPlayerInfo temp_player = players[i];
        opp_vector += plyr_position.unitVect2(temp_player.position) * temp_player.metric; 
        // std::cout << "Opp vector\n:" << std::endl;
        // opp_vector.print();
    }

    // and for closest ally
    temp_pos = frame.getNearestAllyPos(home_team, plyr);
    temp_dist = plyr_position.dist(temp_pos);
    temp_ctrl = frame.getNearestAllyCtrl(home_team, plyr);
    temp_metric = temp_ctrl * exp(-1.0 * temp_dist / (decay_coefficient * pitch_data.getPitchLength()));
    Cart ally_metric_vector = plyr_position.unitVect2(temp_pos) * temp_metric * -1;
    // std::cout << "Ally vector:\n" << std::endl;

    // calculate total metric vector
    Cart f_vector = (opp_vector * attract_coefficient) + (ally_vector * repel_coefficient);
    // std::cout << "Total vector:\n" << std::endl;
    // f_vector.print();

    // move player
    Cart dPos = f_vector.unitVect() * plyr->getStepSize();
    //std::cout << "dPos:\n" << std::endl;
    //dPos.print();
    plyr->changePositionBy(dPos);
}
