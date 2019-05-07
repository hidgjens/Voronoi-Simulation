#include "TetherPep26.h"
#include <vector>
#include <algorithm>
#include <iostream>


TetherPep26::TetherPep26() {}

TetherPep26::TetherPep26(TetherPep26& t) :
player_count(t.player_count),
decay_coefficient(t.decay_coefficient),
attract_coefficient(t.attract_coefficient),
repel_coefficient(t.repel_coefficient),
num_nearest_neighbours(t.num_nearest_neighbours) {
    
    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description;

    region_corners_top = std::make_unique<Cart[]>(26);
    region_corners_bottom = std::make_unique<Cart[]>(26);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count);
    
    for (int i{0}; i < player_count; i++) {
        home_player_posts[i] = t.home_player_posts[i];
        away_player_posts[i] = t.away_player_posts[i];
        post_distances[i] = t.post_distances[i];
    }

    for (int i{0}; i < 26; i++) {
        region_corners_top[i] = t.region_corners_top[i];
        region_corners_bottom[i] = t.region_corners_bottom[i];
    }
}


TetherPep26& TetherPep26::operator=(TetherPep26& t) {
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

    region_corners_top = std::make_unique<Cart[]>(26);
    region_corners_bottom = std::make_unique<Cart[]>(26);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count);

    for (int i{0}; i < player_count; i++) {
        home_player_posts[i] = t.home_player_posts[i];
        away_player_posts[i] = t.away_player_posts[i];
        post_distances[i] = t.post_distances[i];
    }

    for (int i{0}; i < 26; i++) {
        region_corners_top[i] = t.region_corners_top[i];
        region_corners_bottom[i] = t.region_corners_bottom[i];
    }

    return *this;
}


TetherPep26& TetherPep26::operator=(TetherPep26&& t) {
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

    region_corners_top = std::move(t.region_corners_top);
    region_corners_bottom = std::move(t.region_corners_bottom);
    home_player_posts = std::move(t.home_player_posts);
    away_player_posts = std::move(t.away_player_posts);
    post_distances = std::move(t.post_distances);
    // to prevent looping through empty arrays in t
    t.player_count = 0;

    return *this;    
}


TetherPep26::TetherPep26(TeamConfigFile tcf, PitchModel* p) :
player_count(tcf.player_count),
decay_coefficient(tcf.decay_coefficient),
attract_coefficient(tcf.attract_coefficient),
repel_coefficient(tcf.repel_coefficient),
num_nearest_neighbours(tcf.players_to_consider) {

    pitch_data = p->getPitchData();
    double pitch_x = pitch_data.getXdim();
    double pitch_y = pitch_data.getYdim();

    region_corners_top = std::make_unique<Cart[]>(26);
    region_corners_bottom = std::make_unique<Cart[]>(26);
    home_player_posts = std::make_unique<Cart[]>(player_count);
    away_player_posts = std::make_unique<Cart[]>(player_count);
    post_distances = std::make_unique<double[]>(player_count); 

    // updated with actual Pep-26 regions
    // want to construct tether regions
    // define points on pitch in terms of ticks
    double xticks[7] = { -52.5, -35.0, -17.5, 0.0, 17.5, 35.0, 52.5 };
    double yticks[6] = { -34.0, -20.4, -6.8, 6.8, 20.4, 34.0 };
    
    // region 0 - bottom corner
    region_corners_bottom[0] = Cart(xticks[0], yticks[0]); region_corners_top[0] = Cart(xticks[1], yticks[1]);
    // region 1 - penalty area
    region_corners_bottom[1] = Cart(xticks[0], yticks[1]); region_corners_top[1] = Cart(xticks[1], yticks[4]);
    // region 2 - top corner
    region_corners_bottom[2] = Cart(xticks[0], yticks[4]); region_corners_top[2] = Cart(xticks[1], yticks[5]);
    // middle sections - lots of iterative nonsense here
    for (int i{0}; i<4; i++) {
        for (int j{0}; j<5; j++) {
            region_corners_bottom[j+3+(5*i)] = Cart(xticks[i+1],yticks[j]);
            region_corners_top[j+3+(5*i)] = Cart(xticks[i+2], yticks[j+1]);
        }
    } 
    // final 3 regions
    region_corners_bottom[23] = Cart(xticks[5], yticks[0]); region_corners_top[23] = Cart(xticks[6], yticks[1]);
    region_corners_bottom[24] = Cart(xticks[5], yticks[1]); region_corners_top[24] = Cart(xticks[6], yticks[4]);
    region_corners_bottom[25] = Cart(xticks[5], yticks[4]); region_corners_top[25] = Cart(xticks[6], yticks[5]);

    /*for (int i{0}; i<26; i++) {
        std::cout << "region corners: region" << i << std::endl;
        region_corners_bottom[i].print();
        region_corners_top[i].print();
    }*/ 


    // we now have the 26 regions, need to give players their regions for home and away possession
    // loop through players
    int home_region_mapper[11]={1,9,11,13,15,17,19,21,23,24,25};
    int away_region_mapper[11]={1,4,6,8,10,12,14,16,18,20,22};
    for (int i{0}; i < player_count; i++) {
        int j = home_region_mapper[i];
        Cart diff = region_corners_top[j] - region_corners_bottom[j];
        home_player_posts[i] = region_corners_bottom[j] + (diff*0.5);

        j = away_region_mapper[i];
        diff = (region_corners_top[j] - region_corners_bottom[j]);
        away_player_posts[i] = region_corners_bottom[j] + (diff*0.5);
    
        // for now set all post distances to a constant (can change this later)
        post_distances[i] = tcf.max_post_distance; 
    }
    /*for (int i{0}; i < player_count; i++) {
        Cart hp = home_player_posts[i]; 
        Cart ap = away_player_posts[i];
        std::cout << "home and away posts:" << std::endl;
        hp.print();
        std::cout << "(region:" << home_region_mapper[i] << ")" << std::endl;
        ap.print();
        std::cout << "(region:" << away_region_mapper[i] << ")" << std::endl;
    }*/
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


bool compare_distances_pep26(PlayerInfo i, PlayerInfo j) {
    return i.distance < j.distance;
}


void TetherPep26::updateTeam(Team& team, Frame frame) {
    // loop through players
    for (int i{0}; i < team.getPlayerCount(); i++) {
        //std::cout << "Home and away player posts\n" << std::endl;
        //home_home_player_posts[i].print();
        //away_home_player_posts[i].print();

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
            std::sort(players.begin(), players.end(), compare_distances_pep26);
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

