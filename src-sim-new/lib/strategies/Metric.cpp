#include "Metric.h"

Metric::Metric() {}
Metric::Metric(TeamConfigFile tcf, Pitch* p) {
    decay_coefficient = tcf.decay_coefficient;
    attract_coefficient = tcf.attract_coefficient;
    repel_coefficient = tcf.repel_coefficient; 
    min_team_distance = tcf.min_team_dist;
    pitch_data = p; 

}

void Metric::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i ++ ) {
        metricMethod(team.getPlayer(i), frame);
    }
}

void Metric::metricMethod(Player& plyr, Frame frame) {
    Cart player_position = plyr.getPosition();
    double player_control = plyr.getControl();
    bool home_team = plyr.isHomeTeam();

    Cart temp_src_pos; double temp_src_control;
    Cart f;
    Cart F_vector(0.0, 0.0); // sum

    for (int opp_shirt_num{0}; opp_shirt_num < frame.getOpponentCount(home_team); opp_shirt_num++){
        temp_src_control = frame.getOpponentControl(home_team, opp_shirt_num); // control

        if (temp_src_control > player_control){
            temp_src_pos = frame.getOpponentPosition(home_team, opp_shirt_num);
            f = metricVector(plyr, temp_src_pos, temp_src_control);
            F_vector += f * attract_coefficient;
        }
    }

    for (int ally_shirt_num{0}; ally_shirt_num < frame.getAlliedCount(home_team); ally_shirt_num++){
        if (ally_shirt_num != plyr.getShirtNum())
        {
            temp_src_control = frame.getAlliedControl(home_team, ally_shirt_num); // control

            if (temp_src_control > player_control){
                temp_src_pos = frame.getAlliedPosition(home_team, ally_shirt_num);
                f = metricVector(plyr, temp_src_pos, temp_src_control);
                F_vector += f * repel_coefficient;
            }
        }
    }

    Cart dPos = F_vector.unitVect() * plyr.getStepSize();

    // all players have been checked, move towards highest-value source

    plyr.changePositionBy(dPos);
}

Cart Metric::metricVector(Player& plyr, Cart src_pos, double src_control) {
    Cart test_plyr_pos = plyr.getPosition();
    Cart unit_vector = test_plyr_pos.unitVect2(src_pos);
    double distance = test_plyr_pos.dist(src_pos);

    return unit_vector * (src_control * exp( - 1.0 * distance / (decay_coefficient * pitch_data->getPitchLength())));

}