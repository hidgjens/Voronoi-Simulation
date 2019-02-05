#include "Exchange.h"

Exchange::Exchange() : decay_coefficient(1) {}
Exchange::Exchange(TeamConfigFile tcf_) {
    // load values from config file
    decay_coefficient = tcf_.decay_coefficient;
    
}

void Exchange::updateTeam(Team& team, Frame frame){
    for (int i{0}; i < team.getPlayerCount(); i++){
        // iterate through players and apply exchange method individually
        exchange_method(team.getPlayer(i), frame);
    }

}

void Exchange::exchange_method(Player& plyr, Frame frame){
    // get player's position and use it to calculate exchange effect
    Cart player_position = plyr.getPosition();
    double player_control = plyr.getControl();
    bool home_team = plyr.isHomeTeam();

    double temp_metric, temp_control, temp_dist; Cart temp_pos;
    double highest_metric = -1.0; Cart highest_metric_src = player_position; // store the highest metric value and the source position. Use -ve and player as default so that if nothing changes we know and can tell the player to stay still

    for (int opp_shirt_num{0}; opp_shirt_num < frame.getOpponentCount(home_team); opp_shirt_num++){
        temp_control = frame.getOpponentControl(home_team, opp_shirt_num); // control

        if (temp_control > player_control){
            // considered opponent has more control. Calculate their metric
            temp_pos = frame.getOpponentPosition(home_team, opp_shirt_num); // position
            temp_dist = temp_pos.dist(player_position); // distance to player
            temp_metric = metric(temp_dist, temp_control); //temp_control * exp(- temp_dist / ( pitch_data->length * decay_coefficient));
            
            // check if better than current metric
            if (temp_metric > highest_metric) {
                highest_metric = temp_metric;
                highest_metric_src = temp_pos;
            }
        }
    }

    // all players have been checked, move towards highest-value source

    plyr.changePositionTo(highest_metric_src);
 
}

double Exchange::metric(double dist, double ctrl) {
    return ctrl * exp( - 1.0 * dist / (pitch_data->length * decay_coefficient));
    
}