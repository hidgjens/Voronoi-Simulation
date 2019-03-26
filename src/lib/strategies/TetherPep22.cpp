#include "TetherPep22.h"
#include <vector>
#include <algorithm>

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
    corners1[2] = Cart(0, -16.5); corners2[2] = Cart(16.5, -pitch_y/2);

    // split remaining area into 8 equal regions for simplicity
    for (int i{0}; i<8; i++) {  
        if (i<4) {
            corners1[i+3] = Cart(16.5, (-pitch_y/2) + (i * pitch_y/4));
            corners2[i+3] = Cart((16.5/2) + (pitch_x/4), (-pitch_y/2) + ((i+1) * pitch_y/4));
        }

        else {
            corners1[i+3] = Cart((16.5/2) + (pitch_x/4), (-pitch_y/2) + (i * pitch_y/4));
            corners2[i+3] = Cart(pitch_x/2, (-pitch_y/2) + ((i+1) * pitch_y/4));
        }
    }

    // loop through players
    for (int i{0}; i < player_count; i++) {
        // transform x-coordinate so x=0 at centre, not home goal-line
        corners1[i].setX(corners1[i].xComp() - (pitch_x/2));
        corners2[i].setX(corners2[i].xComp() - (pitch_x/2));
    
        // player posts are midpoints of the line connecting the diagonal corners
        Cart diff = corners2[i] - corners1[i];
        home_player_posts[i] = corners1[i] + (diff * (1/2));    

        // can now construct away player posts by inverting x co-ordinates
        // swap player indices so that "keeper" does not traverse whole pitch
        int j = player_count - (i+1);
        away_player_posts[j] = home_player_posts[i];
        away_player_posts[j].setX(away_player_posts[j].xComp() * -1);

        // set each player's max post distance to half the diagonal length of the region
        // (ie circle intersecting the 4 corners of the region)
        post_distances[i] = diff.mod() * (1/2);

        // and calculate corresponding spring constant
        spring_constants[i] = 2.5 / post_distances[i];
    }
}

void TetherPep22::updateTeam(Team& team, Frame frame) {

    // loop through players
    for (int i{0}; i < team.getPlayerCount(); i++) {
        // get player info
        auto plyr = team.getPlayerPtr(i);
        auto plyr_position = plyr->getPosition();

        // Check distance to post
        Cart post;
        if (frame.getPossession())
            post = home_player_posts[i];
        else
            post = away_player_posts[i];
        
        double dist2post = plyr_position.dist(post);

        if (dist2post > post_distances[i]) {
            // player is outside tether region, move back at full speed
            auto dPos = plyr_position.unitVect2(post) * plyr->getStepSize();
            plyr->changePositionBy(dPos);
            continue;
        }        

    



    }



}

