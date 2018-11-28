#include "DSquared.h"
#include <vector>
#include <algorithm> // std::sort
#include "../Match.h"
#include <cmath>
#include "influences/PitchEdge.h"

# define M_PI           3.14159265358979323846  /* pi */


DSquared::DSquared() : players_to_consider(5), density_samples(36) {}
DSquared::DSquared(int smp) : players_to_consider(5), density_samples(smp) {}
DSquared::DSquared(int smp, int ptc) : players_to_consider(ptc), density_samples(smp) {}

// accessors and mutators

void DSquared::setPlayers_to_consider(int ptc) { players_to_consider = ptc; }
int DSquared::getPlayers_to_consider() const { return players_to_consider; }

void DSquared::setDensity_samples(int smp) { density_samples = smp; }
int DSquared::getDensity_samples() const { return density_samples; }

// update

void DSquared::updateFrame(Player& player_, Match& match_) {
    // update player_
    // get pitchborder
    PitchBorder* pb = new PitchBorder(match_.getPitchX(), match_.getPitchY());
    // get player information
    auto player_position = player_.getPos();

    // get teams
    auto homeTeam = match_.getHomeTeam();
    auto awayTeam = match_.getAwayTeam();
    
    // create vector to store player ptrs and distances
    std::vector<std::pair<Player*, double>> players_on_pitch;

    // iterate through home players
    for (int i{0}; i < homeTeam.getPlayerCount(); i++){
        Player* temp_player = homeTeam.getPlayerPtr(i + 1); // POINTER!!
        if (player_.getTeam() == temp_player->getTeam() && player_.getShirtNum() == temp_player->getShirtNum()) continue; // same player so skip
        else {
            // otherwise compute distance and store in vector
            auto temp_pos = temp_player->getPos();
            double dist = player_position.dist(temp_pos);

            players_on_pitch.push_back(std::make_pair(temp_player, dist));
        }
    }
    // iterate through away players
    for (int i{0}; i < awayTeam.getPlayerCount(); i++){
        Player* temp_player = awayTeam.getPlayerPtr(i + 1); // POINTER!!
        if (player_.getTeam() == temp_player->getTeam() && player_.getShirtNum() == temp_player->getShirtNum()) continue; // same player so skip
        else {
            // otherwise compute distance and store in vector
            auto temp_pos = temp_player->getPos();
            double dist = player_position.dist(temp_pos);

            players_on_pitch.push_back(std::make_pair(temp_player, dist));
        }
    }
    // sorting function
    auto sortingDistFunction = [](std::pair<Player*, double> i, std::pair<Player*, double> j) -> bool {
        return( i.second < j.second);
    };
    // sort list based on distance
    std::sort(players_on_pitch.begin(), players_on_pitch.end(), sortingDistFunction);

    // // prints sorted list
    // for (auto x : players_on_pitch) {
    //     std::cout << "Player: " << x.first << "\t dist: " << x.second << "\n";
    // }
    // std::cout << "\n";

    // get distance to nearest edge
    int nearby_edges = 0;

    auto fce = pb->getClosestEdge(player_position);
    double first_edge_dist = fce.perpendicularDistance(player_position);
    auto sce = pb->getSecondClosestEdge(player_position);
    double second_edge_dist = sce.perpendicularDistance(player_position);


    if (players_on_pitch[players_to_consider - 1].second > second_edge_dist) {
        if (players_on_pitch[players_to_consider - 2].second > first_edge_dist) {
            nearby_edges = 2;
        }
        else {
            nearby_edges = 1;
        }
        
    } else
    if (players_on_pitch[players_to_consider - 1].second > first_edge_dist) {
        nearby_edges = 1;
    }
    int mod_ptc = players_to_consider - nearby_edges;

    // create vector of player ptrs
    std::vector<Player*> closest_players;
    // get the closest players
    for (int i{0}; i < mod_ptc; i++) {
        closest_players.push_back(players_on_pitch[i].first); // store pointer
    }

    auto calc_dsquared = [closest_players, pb](Cart pos, int edge_) -> double {
        double dsquared = 0.0;
        for (auto player : closest_players) {
            dsquared += player->getPos().dist2(pos);
        }
        if (edge_ = 1){    // get edge effect
            auto ce = pb->getClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(ce.perpendicularDistance(pos), 2.0);
        } else
        if (edge_ = 2){
            auto ce = pb->getClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(ce.perpendicularDistance(pos), 2.0);
            auto sce = pb->getSecondClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(sce.perpendicularDistance(pos), 2.0);
        }
        return dsquared;
    };

    auto calc_d = [closest_players, pb](Cart pos, int edge_) -> double {
        double dsquared = 0.0;
        for (auto player : closest_players) {
            dsquared += pow(player->getPos().dist(pos), 0.5);
        }
        if (edge_ = 1) {
            auto ce = pb->getClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(ce.perpendicularDistance(pos), 0.5);
        } else
        if (edge_ = 2) {   // get edge effect
            auto ce = pb->getClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(ce.perpendicularDistance(pos), 0.5);
            auto sce = pb->getSecondClosestEdge(pos);
            //std::cout<<ce.getName() + "\t"; pos.print();
            dsquared += pow(sce.perpendicularDistance(pos), 0.5);
        }
        return dsquared;
    };

    // create vector to store angle and value in
    std::vector<std::pair<double, double>> angles_ds;
    // remember current dsquared
    double dsquared_ini = calc_d(player_position, nearby_edges);

    // calc edge effects
    
    // check density in every direction based on density samples
    for (int i{0}; i < density_samples; i++ ) {
        // calculate direction to check
        double theta_prime = 2 * M_PI * ((double) i / (double) density_samples - 0.5);
        // store density sum from each player

        auto temp_pos = player_.getPos() + Cart(player_.getMaxStep() * cos(theta_prime), player_.getMaxStep() * sin(theta_prime));

        angles_ds.push_back(std::make_pair(theta_prime, calc_d(temp_pos, nearby_edges)));

    }
    // sorting lambda fn
    auto sortingDensityFunction = [](std::pair<double, double> i, std::pair<double, double> j) -> bool {
        return( i.second > j.second);
    };

    // sort angles by density, then pick the angle with the smallest density
    std::sort(angles_ds.begin(), angles_ds.end(), sortingDensityFunction);

    // // prints sorted angles
    // for (auto x : angles_ds) {
    //     std::cout << "Theta: " << x.first << "\t dens: " << x.second << "\n";
    // }
    // std::cout << "\n";

    if (angles_ds[0].second >= dsquared_ini) // equals sign for added thrill
    {   // pick direction
        double direction_to_move = angles_ds[0].first;
        double m_step = player_.getMaxStep();
        // move player
        player_.setdPos(Cart(cos(direction_to_move), sin(direction_to_move)) * m_step);
        player_.checkLegalPosition(match_);
        match_.checkCollisions(player_);
               
    }
    delete pb;
}
