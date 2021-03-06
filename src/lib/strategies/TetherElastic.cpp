#include "TetherElastic.h"

TetherElastic::TetherElastic() {}

TetherElastic::TetherElastic(TetherElastic& t) : 
player_count(t.player_count),
max_post_distance(t.max_post_distance),
decay_coefficient(t.decay_coefficient) {
    pitch_data = t.pitch_data;
    spring_constant = t.spring_constant;
    pm = t.pm;
    min_team_distance = t.min_team_distance;
    description = t.description; 

    player_posts = std::make_unique<Cart[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        player_posts[i] = t.player_posts[i];
    }
}

TetherElastic& TetherElastic::operator=(TetherElastic& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    spring_constant = t.spring_constant;
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

TetherElastic& TetherElastic::operator=(TetherElastic&& t) {
    if (&t == this) {
        return *this;
    }
    pm = t.pm;
    player_count = t.player_count;
    spring_constant = t.spring_constant; 
    max_post_distance = t.max_post_distance;
    decay_coefficient = t.decay_coefficient;
    pitch_data = t.pitch_data;
    min_team_distance = t.min_team_distance;
    description = t.description;

    player_posts = std::move(t.player_posts);
    t.player_count = 0;

    return *this;
    
}

TetherElastic::TetherElastic(TeamConfigFile tcf, PitchModel* p) : 
decay_coefficient(tcf.decay_coefficient),
player_count(tcf.player_count),
max_post_distance(tcf.max_post_distance)
{
    pm = p;
    pitch_data = p->getPitchData();
    //std::cout << "max_post " << max_post_distance << std::endl;
    if (max_post_distance < 1.0) {
        max_post_distance = 1.0; // prevent divide by zero error, pitch is discretised to 1m^2 anyway
    }
    spring_constant = 2.5 / max_post_distance;
    //std::cout << "spring " << spring_constant << std::endl;

    
    // create player posts array
    player_posts = std::make_unique<Cart[]>(player_count);

    int n; // temporary index
    double t_dx, t_dy; // temp x,y

    // need to compute post positions (in units of pitch dims [-0.5, 0.5])
    // bottom row
    for (int i{0}; i < 4; i++ ) { // i = 0,1,2,3 -> n = 1,3,5,7  =>  n = 2i + 1
        // map i to n
        n = 2*i + 1;
        
        // calculate post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.25 - 0.5; // use -0.5 to map [0, 1] to [-0.5, 0.5]

        // store
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());

    }
    // middle row
    for (int i{4}; i < 7; i++) { // i = 4,5,6 -> n = 2,4,6  =>  n = 2*(i-3)
        // map to n
        n = 2*(i-3);

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.5 - 0.5;

        // store
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());
    }
    // top row
    for (int i{7}; i < 11; i++) { // i = 7,8,9,10 -> n = 1,3,5,7    =>    n = 2*(i - 7) + 1
        // map to n
        n = 2 * (i-7) + 1;

        // get post position
        t_dx = n * 0.125 - 0.5; t_dy = 0.75 - 0.5;

        // store 
        player_posts[i] = Cart(t_dx * pitch_data.getXdim(), t_dy * pitch_data.getYdim());
    }
}

void TetherElastic::updateTeam(Team& team, Frame frame){
    
    // std::cout << "Update," << std::flush;
    for (int i{0}; i < team.getPlayerCount(); i++){
        // get player
        auto plyr = team.getPlayerPtr(i);
        auto player_position = plyr->getPosition();


        ///////// Check distance to post
        auto post = player_posts[i];
        auto dist2post = post.dist(player_position);
        if (dist2post != 0.0){
            //std::cout << "TetherElasticUpdate" << std::endl;
            //std::cout << "Dist2Post " << dist2post << std::endl;

            auto unit_vect_to_post = player_position.unitVect2(post); // unit vector towards post
            //unit_vect_to_post.print();

            auto kr = KR(dist2post);
            //std::cout << "KR " << kr << std::endl;

            auto dPos = unit_vect_to_post * kr;
            //dPos.print();

            plyr->changePositionBy(dPos);
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

void TetherElastic::exchange_method(Player* plyr, Frame& frame){
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

double TetherElastic::metric(double dist, double ctrl) {
    // std::cout << "Here " << pitch_data->getXlim() << std::endl;
    return ctrl * exp( - 1.0 * dist / (pitch_data.getPitchLength() * decay_coefficient));
    
}

double TetherElastic::KR(double r) { return spring_constant * r; }