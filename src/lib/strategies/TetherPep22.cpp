#include "TetherPep22.h"
#include <vector>
#include <algorithm>
#include <iostream>


TetherPep22::TetherPep22() {}

TetherPep22::TetherPep22(TetherPep22& t) :
player_count(t.player_count),
decay_coefficient(t.decay_coefficient),
attract_coefficient(t.attract_coefficient),
repel_coefficient(t.repel_coefficient),
num_nearest_neighbours(t.num_nearest_neighbours) {
    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description;


    corners1 = std::make_unique<Cart[]>(player_count);
    corners2 = std::make_unique<Cart[]>(player_count);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count);
    spring_constants = std::make_unique<double[]>(player_count);
    
    for (int i{0}; i < player_count; i++) {
        corners1[i] = t.corners1[i];
        corners2[i] = t.corners2[i];
        home_player_posts[i] = t.home_player_posts[i];
        away_player_posts[i] = t.away_player_posts[i];
        post_distances[i] = t.post_distances[i];
        spring_constants[i] = t.spring_constants[i];
    }
}


TetherPep22& TetherPep22::operator=(TetherPep22& t) {
    if (&t == this) {
        return *this;
    }
    
    player_count = t.player_count;
    decay_coefficient = t.decay_coefficient;
    repel_coefficient = t.repel_coefficient;
    attract_coefficient = t.attract_coefficient;
    num_nearest_neighbours = t.num_nearest_neighbours;

    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description;

    corners1 = std::make_unique<Cart[]>(player_count);
    corners2 = std::make_unique<Cart[]>(player_count);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count);
    spring_constants = std::make_unique<double[]>(player_count);

    for (int i{0}; i < player_count; i++) {
        corners1[i] = t.corners1[i];
        corners2[i] = t.corners2[i];
        home_player_posts[i] = t.home_player_posts[i];
        away_player_posts[i] = t.away_player_posts[i];
        post_distances[i] = t.post_distances[i];
        spring_constants[i] = t.spring_constants[i];
    }

    return *this;
}


TetherPep22& TetherPep22::operator=(TetherPep22&& t) {
    if (&t == this) {
        return *this;
    }
    
    player_count = t.player_count;
    decay_coefficient = t.decay_coefficient;
    repel_coefficient = t.repel_coefficient;
    attract_coefficient = t.attract_coefficient;
    num_nearest_neighbours = t.num_nearest_neighbours;

    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description;

    corners1 = std::move(t.corners1);
    corners2 = std::move(t.corners2);
    home_player_posts = std::move(t.home_player_posts);
    away_player_posts = std::move(t.away_player_posts);
    post_distances = std::move(t.post_distances);
    spring_constants = std::move(t.spring_constants);
    // to prevent looping through empty arrays in t
    t.player_count = 0;

    return *this;    
}


TetherPep22::TetherPep22(TeamConfigFile tcf, PitchModel* p) :
player_count(tcf.player_count),
decay_coefficient(tcf.decay_coefficient),
attract_coefficient(tcf.attract_coefficient),
repel_coefficient(tcf.repel_coefficient),
num_nearest_neighbours(tcf.players_to_consider) {

    pitch_data = p->getPitchData();
    double pitch_x = pitch_data.getXdim();
    double pitch_y = pitch_data.getYdim();

    corners1 = std::make_unique<Cart[]>(player_count);
    corners2 = std::make_unique<Cart[]>(player_count);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count);
    spring_constants = std::make_unique<double[]>(player_count); 

    // want to construct tether regions - get diagonal corners for each region
    // penalty area
    corners1[0] = Cart(0, -16.5); corners2[0] = Cart(16.5, 16.5);

    // own corners
    corners1[1] = Cart(0, 16.5); corners2[1] = Cart(16.5, pitch_y/2);
    corners1[2] = Cart(0, -pitch_y/2); corners2[2] = Cart(16.5, -16.5);

    // split remaining area into 8 equal regions for simplicity
    for (int i{0}; i<8; i++) {  
        if (i<4) {
            corners1[i+3] = Cart(16.5, (-pitch_y/2) + (i * pitch_y/4));
            corners2[i+3] = Cart((16.5/2) + (pitch_x/4), (-pitch_y/2) + ((i+1) * pitch_y/4));
        }

        else {
            corners1[i+3] = Cart((16.5/2) + (pitch_x/4), (-pitch_y/2) + ((i-4) * pitch_y/4));
            corners2[i+3] = Cart(pitch_x/2, (-pitch_y/2) + ((i-3) * pitch_y/4));
        }
    }

    // loop through players
    for (int i{0}; i < player_count; i++) {
        // transform x-coordinate so x=0 at centre, not home goal-line
        corners1[i].setX(corners1[i].xComp() - (pitch_x/2));
        corners2[i].setX(corners2[i].xComp() - (pitch_x/2));
    
        // player posts are midpoints of the line connecting the diagonal corners
        Cart diff = (corners2[i] - corners1[i]);
        home_player_posts[i] = corners1[i] + (diff * 0.5);

        // can now construct away player posts
        // add pitchx/2 for all players except "keeper"
        away_player_posts[i] = home_player_posts[i];
        if (i != 0) { 
            away_player_posts[i].setX(home_player_posts[i].xComp() + (pitch_x/2));
        }
        // set each player's max post distance to half the diagonal length of the region
        // (ie circle intersecting the 4 corners of the region)
        post_distances[i] = diff.mod() * 0.25;
        
        //std::cout << "Post distance:" << std::endl;
        //std::cout << post_distances[i] << "\n" << std::endl;
        // and calculate corresponding spring constant
        spring_constants[i] = 2.5 / post_distances[i];
    }
}


struct PlayerInfo{
    bool ally;
    int shirt_num;
    Cart position;
    double distance;

    PlayerInfo(){}

    PlayerInfo(bool a, int sn, Cart p, double d) :
    ally(a),
    shirt_num(sn),
    position(p),
    distance(d)
    {}
};


bool compare_distances(PlayerInfo i, PlayerInfo j) {
    return i.distance < j.distance;
}


void TetherPep22::updateTeam(Team& team, Frame frame) {
    // loop through players
    for (int i{0}; i < team.getPlayerCount(); i++) {
        //std::cout << "Home and away player posts\n" << std::endl;
        //home_player_posts[i].print();
        //away_player_posts[i].print();

        // get player info
        auto plyr = team.getPlayerPtr(i);
        bool allied = plyr->isHomeTeam();
        auto plyr_position = plyr->getPosition();

        // check if too close to friendly
        auto nearest_ally_pos = frame.getNearestAllyPos(allied, plyr);
        //std::cout << "scatter block:" << std::endl;
        if (plyr_position.dist(nearest_ally_pos) < min_team_distance * plyr->getStepSize()) {
            // player is too close to ally, scatter it
            auto scatter_displacement = nearest_ally_pos.unitVect2(plyr_position); // defined the reverse was round so it points AWAY from "nearest position"
            scatter_displacement *= plyr->getStepSize(); // distance = max step
            plyr->changePositionBy(scatter_displacement);
            continue;
        }
        //std::cout << "no prang" << std::endl;

        //std::cout << "check post distance" << std::endl;
        // Check distance to post
        // if in possession, AWAY half is weighted more -> use AWAY posts
        Cart post;
        if (frame.getPossession())
            post = away_player_posts[i];
        else
            post = home_player_posts[i];
        double dist2post = plyr_position.dist(post);
        
        //std::cout << "no prang" << std::endl;
        //std::cout << i << std::endl;
        //std::cout << post_distances[i] << std::endl;        
        if (dist2post > post_distances[i]) {
            //std::cout << "outisde tether region" << std::endl;
            // player is outside tether region, move back at full speed
            auto dPos = plyr_position.unitVect2(post) * plyr->getStepSize();
            plyr->changePositionBy(dPos);
            //std::cout << "no prang" << std::endl;
            continue;
        }        

        else if (dist2post <= post_distances[i]) {
            //std::cout << "inside tether region" << std::endl;
            // player is inside region, add elastic contribution
            double dist_ratio = plyr_position.dist(post) / post_distances[i];
            Cart elastic_contribution = plyr_position.unitVect2(post) * plyr->getStepSize() * dist_ratio;

            // now calculate metricN contribution
            // list of all players
            std::vector<PlayerInfo> players;
            //std::cout << "defined players list" << std::endl;
            // populate with opponents
            Cart temp_plyr_pos; double temp_plyr_dist;
            for (int i{0}; i < frame.getOpponentCount(allied); i++){
                temp_plyr_pos = frame.getOpponentPosition(allied, i);
                temp_plyr_dist = plyr_position.dist(temp_plyr_pos);
                players.push_back(PlayerInfo(allied, i, temp_plyr_pos, temp_plyr_dist));
            }
            
            // and allies
            for (int i{0}; i < team.getPlayerCount(); i++) {
                if (i != plyr->getShirtNum()) {
                    temp_plyr_pos = frame.getAlliedPosition(allied, i);
                    temp_plyr_dist = plyr_position.dist(temp_plyr_pos);
                    players.push_back(PlayerInfo(allied, i, temp_plyr_pos, temp_plyr_dist));    
                }
            }

            // now sort and take nearest neighbours
            std::sort(players.begin(), players.end(), compare_distances);
            //players.resize(num_nearest_neighbours);
            //players.resize(3);
            //std::cout << "resize fine" << std::endl;

            // get metric contribution for each player
            Cart metric_contribution(0,0);
            //for (auto p : players) {}
            for (int i{0}; i < num_nearest_neighbours; i++) {
                //std::cout << "into loop" << std::endl;
                PlayerInfo p = players[i];
                //p.position.print();
                //std::cout << "\n" << p.ally << "\n" << p.shirt_num << "\n" << p.distance << std::endl; 
                double multiplier{1.0}; double temp_ctrl;
                
                if (p.ally == true) {
                    multiplier *= -1.0 * repel_coefficient;
                    temp_ctrl = frame.getAlliedControl(allied, p.shirt_num);
                }
                else if (p.ally == false) {
                    multiplier *= attract_coefficient;
                    temp_ctrl = frame.getOpponentControl(allied, p.shirt_num);
                }
                //std::cout << pitch_data.getPitchLength() << std::endl;
                //std::cout << multiplier << std::endl;
                //std::cout << temp_ctrl << std::endl;
                double metric_amp = multiplier * temp_ctrl * exp(-1.0 * p.distance / (pitch_data.getPitchLength() * decay_coefficient)); 
                //std::cout << "Metric amplitude:" << metric_amp << std::endl;
                metric_contribution += plyr_position.unitVect2(p.position) * metric_amp;           
                //std::cout << "loop code fine" << std::endl;
            }
            
            // normalise metric contribution then add elastic contribution, then renormalise step
            metric_contribution = metric_contribution.unitVect() * plyr->getStepSize();
            Cart total_vect = metric_contribution + elastic_contribution;
            plyr->changePositionBy(total_vect.unitVect() * plyr->getStepSize());
        }
    }
}

