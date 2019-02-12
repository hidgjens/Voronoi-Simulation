#include "Exchange.h"
#include "../Pitch.h"


Exchange::Exchange() : decay_coefficient(1) {}
Exchange::Exchange(TeamConfigFile tcf_, Pitch* p) {
    // load values from config file
    decay_coefficient = tcf_.decay_coefficient;
    min_team_distance = tcf_.min_team_dist;
    pitch_data = p; 
    
}

void Exchange::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount(); i++){
        // get player
        auto plyr = team.getPlayer(i);


        ////////// CHECK NEAREST PLAYER
        // check if too close to friendly
        auto player_position = plyr.getPosition();
        auto nearest_position = frame.getNearestAllyPos(plyr.isHomeTeam(), plyr);

        if (player_position.dist(nearest_position) == min_team_distance * plyr.getStepSize()) {
            // player is too close to ally, scatter it
            auto scatter_displacement = nearest_position.unitVect2(player_position); // defined the reverse was round so it points AWAY from "nearest position"
            scatter_displacement *= plyr.getStepSize(); // distance = max step
            plyr.changePositionBy(scatter_displacement);
            return;
        }
        //////////// DONE


        // iterate through players and apply exchange method individually
        exchange_method(plyr, frame);
        // std::cout << plyr.getShirtNum() << std::endl;

        
    }

}

void Exchange::exchange_method(Player& plyr, Frame& frame){
    // std::cout << "EXCHANGEMETHOD" << std::flush;
    // get player's position and use it to calculate exchange effect
    Cart player_position = plyr.getPosition();
    // std::cout << "2EXCHANGEMETHOD" << std::flush;

    double player_control = plyr.getControl();
    // std::cout << "3EXCHANGEMETHOD" << std::flush;

    bool home_team = plyr.isHomeTeam();
    // std::cout << "5EXCHANGEMETHOD" << std::flush;


    double temp_metric, temp_control, temp_dist; Cart temp_pos;
    double highest_metric = -1.0; Cart highest_metric_src = player_position; // store the highest metric value and the source position. Use -ve and player as default so that if nothing changes we know and can tell the player to stay still

    for (int opp_shirt_num{0}; opp_shirt_num < frame.getOpponentCount(home_team); opp_shirt_num++){
        // std::cout << ",ctrl," << std::flush;
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
    // std::cout << plyr.getShirtNum() << std::endl;
    plyr.changePositionTo(highest_metric_src);
 
}

double Exchange::metric(double dist, double ctrl) {
    // std::cout << "Here " << pitch_data->getXlim() << std::endl;
    return ctrl * exp( - 1.0 * dist / (pitch_data->getPitchLength() * decay_coefficient));
    
}