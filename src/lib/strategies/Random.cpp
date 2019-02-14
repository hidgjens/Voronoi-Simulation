#include "Random.h"

Random::Random() { }
Random::Random(Pitch* p) { pitch_data = p;  }

void Random::updateTeam(Team& team, Frame frame) {
    for (int i{0}; i < team.getPlayerCount(); i++) {
        auto plyr = team.getPlayerPtr(i);
        randomWalk(plyr);
    }
}

void Random::randomWalk(Player* player) {
    double rand_direction = ((double) rand() / RAND_MAX) * 2 * M_PI;
    double step_size = player->getStepSize();

    double dx = step_size * cos(rand_direction);
    double dy = step_size * sin(rand_direction);
    // std::cout << dx << " " << dy << std::endl;

    Cart dPos(dx, dy);

    player->changePositionBy(dPos); 
}