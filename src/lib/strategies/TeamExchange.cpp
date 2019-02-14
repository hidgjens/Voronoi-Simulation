#include "TeamExchange.h"

TeamExchange::TeamExchange() {}
TeamExchange::TeamExchange(TeamConfigFile tcf, Pitch* p) {
    pitch_data = p;
    targets_to_prioritise = tcf.targets_to_prioritise;
    min_team_distance = tcf.min_team_dist;
}

static bool sort_control(std::pair<Cart, double> i, std::pair<Cart, double> j) {
    /* 
        This function is not part the class but is needed to sort 
        std::pairs of (position, control) by the value of control.
    */

    /* 
        NOTE: I am using a bit of trickery here. I am going to order
        the vectors in reverse order to what you might expect. I want
        the last element of the vector to be the first of the 'list',
        so that to get the first item in the list, i can use the pop_back
        function of a vector to remove the last element of the vector.
        For this example, largest control is the last element of the vector
    */
    return (i.second < j.second);
}


static bool sort_distance(std::pair<Player*, double> i, std::pair<Player*, double> j) {
    return (i.second > j.second);
    // this orders the vector so the closest player is last
}

void TeamExchange::updateTeam(Team& team, Frame frame) {
    /*
        Plan: 
        1. Order opponent team from highest control to lowest.
        2. The number of High-Priority Targets (HPTs) is given by targets_to_consider.
        3. Assign the two closest allies to target/attack the first HPT.
            Remove these two players from the selection pool, 
            and remove HPT from list
        4. Repeat step 3 until all HPTs have been assigned 2 attackers.
        5. Assign 1 attacker to the remaining opponents, leaving the opponents
            with the least control untargeted
    */

    bool isHomeTeam = team.isHomeTeam(); // will need this
    int team_player_count = team.getPlayerCount();
    int opponent_player_count = frame.getOpponentCount(isHomeTeam);


    if (team_player_count < 2 * targets_to_prioritise) {
        // this is a problem, as we need two players for each HPT
        // reduce the number of HPT
        targets_to_prioritise = (int) team_player_count / 2;
    }

    // create a vector to store information about opponents (only need position and control)
    std::vector<std::pair<Cart, double>> opponent_players;

    // temporary variables
    Cart temp_cart; double temp_ctrl;

    // fill vector with information from the frame
    for (int i{0}; i < opponent_player_count; i++) {
        // for shirt number i, get control and position
        temp_cart = frame.getOpponentPosition(isHomeTeam, i);
        temp_ctrl = frame.getOpponentControl(isHomeTeam, i);
        // create a pair of the two and store
        opponent_players.push_back(std::make_pair(temp_cart, temp_ctrl));
    }

    // sort vector by control
    std::sort(opponent_players.begin(), opponent_players.end(), sort_control);

    // vector for allied players
    std::vector<std::pair<Player*, double>> attackers;

    for (int i{0}; i < team_player_count; i++) {
        auto plyr = team.getPlayerPtr(i);
        attackers.push_back(std::make_pair(plyr, 0.0));
    }

    // remove targets from the list until all HPT have been dealt with
    // for each target, calculate the distance of the remaining allies to that player,
    // the highest two are removed from the list and made to target that player
    
    // temps
    Player *p, *p1, *p2; Cart c; double d; 
    for (int i{0}; i < targets_to_prioritise; i++) {
        // get player position from vector then remove it
        temp_cart = opponent_players.back().first;
        opponent_players.pop_back();

        // calculate distances for the attackers
        for (int i{0}; i < attackers.size(); i++ ) {
            p = attackers[i].first; c = p->getPosition();
            d = c.dist(temp_cart);
            attackers[i] = std::make_pair(p, d);
        }

        // sort vector by distances
        std::sort(attackers.begin(), attackers.end(), sort_distance);

        // select last two to target player
        p1 = attackers.back().first;
        attackers.pop_back();
        p2 = attackers.back().first;
        attackers.pop_back();
        // these players are now removed from the list, so we can just repeat this step

        // get the players to target opponent
        p1->changePositionTo(temp_cart);
        p2->changePositionTo(temp_cart);

    }

    // all HPTs are dealt with, now the remaining attackers are assigned 1:1 to the highest value remaining targets

    for (int i{0}; i < attackers.size(); i++) {
        // attacking player
        p1 = attackers.back().first;
        attackers.pop_back();

        // target
        temp_cart = opponent_players.back().first;
        opponent_players.pop_back();

        p1->changePositionTo(temp_cart);
    }

}