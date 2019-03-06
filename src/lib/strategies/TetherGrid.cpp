#include "TetherGrid.h"

TetherGrid::TetherGrid() {}

TetherGrid::TetherGrid(TetherGrid& t) : 
player_count(t.player_count),
max_post_distance(t.max_post_distance),
decay_coefficient(t.decay_coefficient) {
    pitch_data = t.pitch_data;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description; 
    sigma_scale = t.sigma_scale;
    free_agents = t.free_agents;

    home_player_posts = std::make_unique<Cart[]>(player_count- free_agents);
    for (int i{0}; i < player_count- free_agents; i++) {
        home_player_posts[i] = t.home_player_posts[i];
    }
    away_player_posts = std::make_unique<Cart[]>(player_count- free_agents);
    for (int i{0}; i < player_count- free_agents; i++) {
        away_player_posts[i] = t.away_player_posts[i];
    }
}

TetherGrid& TetherGrid::operator=(TetherGrid& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    decay_coefficient = t.decay_coefficient;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;
    sigma_scale = t.sigma_scale;
    free_agents = t.free_agents;

    home_player_posts = std::make_unique<Cart[]>(player_count- free_agents);
    for (int i{0}; i < player_count- free_agents; i++) {
        home_player_posts[i] = t.home_player_posts[i];
    }
    away_player_posts = std::make_unique<Cart[]>(player_count- free_agents);
    for (int i{0}; i < player_count- free_agents; i++) {
        away_player_posts[i] = t.away_player_posts[i];
    }

    return *this;
}

TetherGrid& TetherGrid::operator=(TetherGrid&& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    max_post_distance = t.max_post_distance;
    decay_coefficient = t.decay_coefficient;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;
    sigma_scale = t.sigma_scale;
    free_agents = t.free_agents;

    home_player_posts = std::move(t.home_player_posts);
    away_player_posts = std::move(t.away_player_posts);
    t.player_count = 0;

    return *this;
    
}

TetherGrid::TetherGrid(TeamConfigFile tcf, PitchModel* p) : 
decay_coefficient(tcf.decay_coefficient),
player_count(tcf.player_count),
free_agents(tcf.free_agents),
sigma_scale(tcf.sigma_scale),
max_post_distance(tcf.max_post_distance)
{

    pm = p;
    pitch_data = p->getPitchData();
    
    // create player posts array
    home_player_posts = std::make_unique<Cart[]>(player_count - free_agents);
    away_player_posts = std::make_unique<Cart[]>(player_count - free_agents); // home/away refers to possession

    // going to constuct from H-M-A (home, mid, away) and L-C-R (left, centre, right)

    double x_boundary, y_boundary, xb2, yb2;
    int x_mult, y_mult; // store quadrant
    Cart bound1, bound2, bound3, bound4;

    // first home possession posts

    // H-L (-ve x, +ve y)
    x_mult = -1; y_mult = +1;
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[0] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // H-C (-,0)
    x_mult = -1;
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = p->getHomeGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[1] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // H-R (-,-)
    x_mult = -1;
    y_mult = -1;
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYMINUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[2] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-L (0,+)
    y_mult = +1;
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = p->getHomeGrid().getDXPLUS();
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[3] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-C (0,0)
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = p->getHomeGrid().getDXPLUS();
    yb2 = p->getHomeGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[4] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-R (0,-)
    y_mult = -1;
    x_boundary = p->getHomeGrid().getDXMINUS();
    y_boundary = p->getHomeGrid().getDYMINUS();
    xb2 = p->getHomeGrid().getDXPLUS();
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[5] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // A-L (+,+)
    x_mult = +1;
    y_mult = +1;
    x_boundary = p->getHomeGrid().getDXPLUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[6] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // A-C (+,0)
    x_mult = +1;
    x_boundary = p->getHomeGrid().getDXPLUS();
    y_boundary = p->getHomeGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = p->getHomeGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[7] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // A-R (+,-)
    x_mult = +1;
    y_mult = -1;

    x_boundary = p->getHomeGrid().getDXPLUS();
    y_boundary = p->getHomeGrid().getDYMINUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    home_player_posts[8] = (bound1 + bound2 + bound3 + bound4) * 0.25;


    // now away possession posts
    // H-L (-ve x, +ve y)
    x_mult = -1; y_mult = +1;
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[0] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // H-C (-,0)
    x_mult = -1;
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = p->getHomeGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[1] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // H-R (-,-)
    x_mult = -1;
    y_mult = -1;
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYMINUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[2] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-L (0,+)
    y_mult = +1;
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = p->getAwayGrid().getDXPLUS();
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[3] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-C (0,0)
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = p->getAwayGrid().getDXPLUS();
    yb2 = p->getAwayGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[4] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // M-R (0,-)
    y_mult = -1;
    x_boundary = p->getAwayGrid().getDXMINUS();
    y_boundary = p->getAwayGrid().getDYMINUS();
    xb2 = p->getAwayGrid().getDXPLUS();
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[5] = (bound1 + bound2 + bound3 + bound4) * 0.25; 

    // A-L (+,+)
    x_mult = +1;
    y_mult = +1;
    x_boundary = p->getAwayGrid().getDXPLUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[6] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // A-C (+,0)
    x_mult = +1;
    x_boundary = p->getAwayGrid().getDXPLUS();
    y_boundary = p->getAwayGrid().getDYPLUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = p->getAwayGrid().getDYMINUS();

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[7] = (bound1 + bound2 + bound3 + bound4) * 0.25;

    // A-R (+,-)
    x_mult = +1;
    y_mult = -1;

    x_boundary = p->getAwayGrid().getDXPLUS();
    y_boundary = p->getAwayGrid().getDYMINUS();
    xb2 = pitch_data.getXlim() * x_mult;
    yb2 = pitch_data.getYlim() * y_mult;

    bound1 = Cart(x_boundary, y_boundary);
    bound2 = Cart(xb2, yb2);
    bound3 = Cart(x_boundary, yb2);
    bound4 = Cart(xb2, y_boundary);

    away_player_posts[8] = (bound1 + bound2 + bound3 + bound4) * 0.25;
}

void TetherGrid::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount()  - free_agents; i++){
        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();


        ///////// Check distance to post
        Cart post;
        if (frame.getPossession())
            post = home_player_posts[i];
        else
            post = away_player_posts[i];
        
        if (player_position.dist(post) >= max_post_distance) {
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

    // free agents
    for (int i{player_count - free_agents}; i < player_count; i++) {
        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();
        
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

void TetherGrid::exchange_method(Player* plyr, Frame& frame){
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

double TetherGrid::metric(double dist, double ctrl) {
    // std::cout << "Here " << pitch_data->getXlim() << std::endl;
    return ctrl * exp( - 1.0 * dist / (pitch_data.getPitchLength() * decay_coefficient));
    
}