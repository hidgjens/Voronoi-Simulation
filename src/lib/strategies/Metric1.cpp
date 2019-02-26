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

    // total metric vector
    Cart f_vector(0.0, 0.0);

    // get closest opponent metric vector
    int closest_opp = frame.getNearestOpponentShirtNum(home_team, player_position);   
    Cart closest_opp_pos = frame.getNearestOpponentPos(home_team, player_position);
    double closest_opp_ctrl = frame.getOpponentControl(home_team, closest_opp);
    Cart opp_metric_vector = Metric1Vector(plyr, closest_opp_pos, closest_opp_ctrl);

    // get closest teammate metric vector
    int closest_ally = frame.getNearestAllyShirtNum(home_team, plyr);
    Cart closest_ally_pos = frame.getNearestAllyPos(home_team, plyr);
    double closest_ally_ctrl = frame.getNearestAllyCtrl(home_team, plyr);
    Cart ally_metric_vector = Metric1Vector(plyr, closest_ally_pos, closest_ally_ctrl);

    // get total metric vector and step
    f_vector += (ally_metric_vector + opp_metric_vector) * attract_coefficient;
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