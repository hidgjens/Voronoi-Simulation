#include "Metric1.h"

Metric1::Metric1() {}
Metric1::Metric1(TeamConfigFile tcf, Pitch* p) {
    decay_coefficient = tcf.decay_coefficient;
    attract_coefficient = tcf.attract_coefficient;
    repel_coefficient = tcf.repel_coefficient; 
    min_team_distance = tcf.min_team_dist;
    pitch_data = p; 

}

void Metric1::updateTeam(Team& team, Frame frame) {
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

        Metric1Method(plyr, frame);
    }
}

void Metric1::Metric1Method(Player* plyr, Frame frame) {
    Cart player_position = plyr->getPosition();
    double player_control = plyr->getControl();
    bool home_team = plyr->isHomeTeam();

    // define opponent metric vector and total metric vector
    Cart opp_metric_vector(0.0, 0.0);
    Cart f_vector(0.0, 0.0);

    // closest player
    int closest_opp; double closest_opp_dist{0};

    // loop through opponent players
    for (int opp_shirt_num{0}; opp_shirt_num < frame.getOpponentCount(home_team); opp_shirt_num++){
        int j = 0;

        // find nearest player with greater spatial control 
        if (player_control < frame.getOpponentControl(home_team, opp_shirt_num)){
            // get distance to opponent
            Cart opp_pos = frame.getOpponentPosition(home_team, opp_shirt_num);
            double opp_dist = opp_pos.dist(player_position);

            // take first player to be closest by default
            if (j == 0){
                closest_opp = opp_shirt_num;
                closest_opp_dist = opp_dist;
                j++;
            }
            // update closest player if new player is closer
            else if (opp_dist < closest_opp_dist){
                closest_opp = opp_shirt_num;
                closest_opp_dist = opp_dist;
            }
        }
    }
    
    // get metric vector corresponding to closest opponent
    // (if no closest opponent with greater area, opp_metric_vector = (0,0))
    if (closest_opp_dist != 0){
        Cart closest_opp_pos = frame.getOpponentPosition(home_team, closest_opp);
        double closest_opp_ctrl = frame.getOpponentControl(home_team, closest_opp);
        opp_metric_vector = Metric1Vector(plyr, closest_opp_pos, closest_opp_ctrl);
    }

    // get closest teammate metric vector
    int closest_ally = frame.getNearestAllyShirtNum(home_team, plyr);
    Cart closest_ally_pos = frame.getNearestAllyPos(home_team, plyr);
    double closest_ally_ctrl = frame.getNearestAllyCtrl(home_team, plyr);
    Cart ally_metric_vector = Metric1Vector(plyr, closest_ally_pos, closest_ally_ctrl) * -1;

    // get total metric vector and step
    f_vector += (ally_metric_vector * repel_coefficient) + (opp_metric_vector * attract_coefficient);
    Cart dPos = f_vector.unitVect() * plyr->getStepSize();

    // move by step
    plyr->changePositionBy(dPos);
}

Cart Metric1::Metric1Vector(Player* plyr, Cart src_pos, double src_control) {
    Cart test_plyr_pos = plyr->getPosition();
    Cart unit_vector = test_plyr_pos.unitVect2(src_pos);
    double distance = test_plyr_pos.dist(src_pos);

    return unit_vector * (src_control * exp( - 1.0 * distance / (decay_coefficient * pitch_data->getPitchLength())));
}