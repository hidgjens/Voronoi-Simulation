#include "TetherCounter.h"

TetherCounter::TetherCounter() { }

TetherCounter::TetherCounter(TetherCounter& t) : 
player_count(t.player_count),
max_post_distance(t.max_post_distance),
decay_coefficient(t.decay_coefficient) {
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description; 

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }
}

TetherCounter& TetherCounter::operator=(TetherCounter& t) {
    if (&t == this) {
        return *this;
    }

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

TetherCounter& TetherCounter::operator=(TetherCounter&& t) {
    if (&t == this) {
        return *this;
    }
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

TetherCounter::TetherCounter(TeamConfigFile tcf, Pitch* p) : 
decay_coefficient(tcf.decay_coefficient),
player_count(tcf.player_count)
{
    pitch_data = p;
    
    // create player posts array
    player_posts = std::make_unique<Cart[]>(player_count);

    int n; int m; int j{0}; // temporary indices
    double t_dx, t_dy; // temp x,y

    // need to compute post positions (in units of pitch dims [-0.5, 0.5])
    for (int i{0}; i<4; i++) { // i = 0,1,2,3 -> n = 1,3,5,7 => n = 2*i + 1
        // map to n
        n = 2*i + 1;

        // generate y component of position
        t_dy = n*0.125 - 0.5;

        for (int i{0}; i<2; i++) { // i = 0,1 -> m = 2,6 (if n = 1,7), m = 3,5 (if n = 3,5) 
            // generate x components 
            if (n == 1 || n == 7) {
                m = 2; t_dx = m*0.125 - 0.5;

                // store position
                player_posts[j] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
                j++;

                m = 6; t_dx = m*0.125 - 0.5;

                // store position
                player_posts[j] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
                j++;
            }

            if (n == 3 || n == 5) {
                m = 3; t_dx = m*0.125 - 0.5;

                // store position
                player_posts[j] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
                j++;

                m = 5; t_dx = m*0.125 - 0.5;

                // store position
                player_posts[j] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
                j++;
            }
        }
    }
    
    // and now the extra cases where n = 4
    n = 4; t_dy = n*0.125 - 0.5;
    for (int i{0}; i<2; i++) { // i = 0,1 -> m = 1,7 => m = 6*i + 1
        // map i to m
        m = 6*i + 1; t_dx = m*0.125 - 0.5;

        // store position
        player_posts[j] = Cart(t_dx * pitch_data->getXdim(), t_dy * pitch_data->getYdim());
        j++;
    }

    // free agent has a post in the centre that is ignored
    player_posts[j] = Cart(0,0); 
}

void TetherCounter::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount(); i++){

        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();


        ///////// Check distance to post (if in first 10 players)
        auto post = player_posts[i];
        if (player_position.dist(post) > max_post_distance && i != team.getPlayerCount()-1) {
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
        exchange_method(plyr, frame);
        // std::cout << plyr.getShirtNum() << std::endl; 
    }
}

void TetherCounter::exchange_method(Player* plyr, Frame& frame){
    // std::cout << "EXCHANGEMETHOD" << std::flush;
    // get player's position and use it to calculate exchange effect
    Cart player_position = plyr->getPosition();
    // std::cout << "2EXCHANGEMETHOD" << std::flush;

    double player_control = plyr->getControl();
    // std::cout << "3EXCHANGEMETHOD" << std::flush;

    bool home_team = plyr->isHomeTeam();
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
    plyr->changePositionTo(highest_metric_src);
 
}

double TetherCounter::metric(double dist, double ctrl) {
    // std::cout << "Here " << pitch_data->getXlim() << std::endl;
    return ctrl * exp( - 1.0 * dist / (pitch_data->getPitchLength() * decay_coefficient));
    
}