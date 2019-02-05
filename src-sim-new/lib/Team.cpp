#include "Team.h"
#include "TeamStrategy.h"
#include "Pitch.h"

Team::Team() {}

Team::Team(TeamConfigFile tcf_, bool home, Pitch& pitch)
: tcf(tcf_)
, player_count(tcf_.player_count)
, home_team(home) {
    // create strategy
    teamstrat = TeamStrategy::CreateStrat(&pitch, tcf);

    players = std::make_unique<Player[]>(player_count);
    std::cout << "making players\n";
    for (int i{0}; i < player_count; i++){
        // create players and give random positions
        players[i] = Player(i);
        double randx = (((double) rand() / RAND_MAX) - 0.5) * pitch.x_dim;
        double randy = (((double) rand() / RAND_MAX) - 0.5) * pitch.y_dim;
        players[i].setInitialPosition(Cart(randx, randy));
    }
}


// copy and move stuff

Team::Team(Team& team) : tcf(team.tcf) , player_count(team.player_count), home_team(team.home_team), teamstrat(team.teamstrat)
{
    players = std::make_unique<Player[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        players[i] = team.players[i];
    }
}

// copy assignment
Team& Team::operator=(Team& team) {
    if (&team == this) {
        return *this;
    }

    tcf = team.tcf;
    player_count = team.player_count;
    home_team = team.home_team;
    teamstrat = team.teamstrat;

    players = std::make_unique<Player[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        players[i] = team.players[i];
    }

    return *this;

}

Team& Team::operator=(Team&& team){
    if (&team == this) {
        return *this;
    }

    tcf = team.tcf;
    player_count = team.player_count;
    home_team = team.home_team;
    teamstrat = team.teamstrat;

    players = std::move(team.players);
    team.player_count = 0;

    return *this;
}

Player& Team::getPlayer(int i) { return players[i]; }
bool Team::isHomeTeam() const { return home_team; }
int Team::getPlayerCount() const { return player_count; }

void Team::update(Frame frame) { teamstrat->updateTeam(*this, frame); }