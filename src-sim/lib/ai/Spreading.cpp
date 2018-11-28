#include "Spreading.h"
#include <vector>
#include <algorithm> // std::sort
#include "../Match.h"
#include <cmath>
# define M_PI           3.14159265358979323846  /* pi */
#include "influences/PitchEdges.h"


Spreading::Spreading() : players_to_consider(4), density_samples(18) {}
Spreading::Spreading(int smp) : players_to_consider(5), density_samples(smp) {}
Spreading::Spreading(int smp, int ptc) : players_to_consider(ptc), density_samples(smp) {}

// accessors and mutators

void Spreading::setPlayers_to_consider(int ptc) { players_to_consider = ptc; }
int Spreading::getPlayers_to_consider() const { return players_to_consider; }

void Spreading::setDensity_samples(int smp) { density_samples = smp; }
int Spreading::getDensity_samples() const { return density_samples; }

// angular distribution filter
double Spreading::filter(double theta) { return cos(theta); }

// update

void Spreading::updateFrame(Player& player_, Match& match_) {
    // update player_

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

    // create vector of player ptrs
    std::vector<Player*> closest_players;

    // get the closest players
    for (int i{0}; i < players_to_consider; i++) {
        closest_players.push_back(players_on_pitch[i].first); // store pointer
    }

    // now calculate their thetas
    std::vector<double> player_thetas;
    for (auto x : closest_players) {
        // get player position
        auto temp_pos = x->getPos();
        // calculate unitvector to that player
        auto unit_vect = player_position.unitVect2(temp_pos);
        // calculate theta
        auto theta = atan2(unit_vect.xComp(), unit_vect.yComp());
        
        player_thetas.push_back(theta);
    }

    // create vector to store angle and value in
    std::vector<std::pair<double, double>> angles_density;
    // calc edge effects
    auto edges = calc_edge_terms(&player_, &match_);
    // check density in every direction based on density samples
    for (int i{0}; i < density_samples; i++ ) {
        // calculate direction to check
        double theta_prime = 2 * M_PI * ((double) i / (double) density_samples - 0.5);
        // store density sum from each player
        double density_theta = 0;
        // for each theta compute density contribution
        for (auto theta_i : player_thetas) {
            density_theta += filter(theta_i - theta_prime);
        }
        // add edge effects
        // player_.printPos();
        for (auto weight_direction_pair : edges) {
            // std::cout << "\nVal: " << weight_direction_pair.first << " Theta: " << weight_direction_pair.second << "\n";
            density_theta += weight_direction_pair.first * filter(weight_direction_pair.second - theta_prime);
        }
        angles_density.push_back(std::make_pair(theta_prime, density_theta));

    }
    // sorting lambda fn
    auto sortingDensityFunction = [](std::pair<double, double> i, std::pair<double, double> j) -> bool {
        return( i.second < j.second);
    };

    // sort angles by density, then pick the angle with the smallest density
    std::sort(angles_density.begin(), angles_density.end(), sortingDensityFunction);

    // // prints sorted angles
    // for (auto x : angles_density) {
    //     std::cout << "Theta: " << x.first << "\t dens: " << x.second << "\n";
    // }
    // std::cout << "\n";

    // pick direction
    double direction_to_move = angles_density[0].first;
    double m_step = player_.getMaxStep();
    // move player
    player_.setdPos(Cart(cos(direction_to_move), sin(direction_to_move)) * m_step);
    player_.checkLegalPosition(match_);
    match_.checkCollisions(player_);
}
