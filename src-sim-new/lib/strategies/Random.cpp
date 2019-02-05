#include "Random.h"

Random::Random() {}

void Random::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i++) {
        randomWalk(team.getPlayer(i));
    }
}

void Random::randomWalk(Player& player) {
    double rand_direction = ((double) rand() / RAND_MAX) * 2 * M_PI;
    double step_size = player.getStepSize();

    double dx = step_size * cos(rand_direction);
    double dy = step_size * sin(rand_direction);

    Cart dPos(dx, dy);

    player.changePositionBy(dPos); 
}