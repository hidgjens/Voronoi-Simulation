#include "Team.h"
#include "TeamStrategy.h"
#include "Pitch.h"

Team::Team() { 
    players = std::make_unique<Player[]>(11);

}

Team::Team(TeamConfigFile tcf_, bool home, Pitch* pitch)
: tcf(tcf_)
, player_count(tcf_.player_count)
, home_team(home) {
    // create strategy
    teamstrat = TeamStrategy::CreateStrat(pitch, tcf);
    pitch_data = pitch;
    // std::cout << &pitch_data << "\t" << &pitch << std::endl;
    // std::cout << pitch_data->getXlim() << "\t" << pitch_data->getYlim() << "\t" << pitch_data->getPitchLength() << std::endl;
    

    players = std::make_unique<Player[]>(player_count);
    //std::cout << "\nmaking players\n"<< std::flush;
    for (int i{0}; i < player_count; i++){
        // create players and give random positions
        players[i] = Player(i);
        players[i].setTeamPtr(this);
        double randx = (((double) rand() / RAND_MAX) - 0.5) * pitch_data->getXdim();
        double randy = (((double) rand() / RAND_MAX) - 0.5) * pitch_data->getYdim();
        players[i].setInitialPosition(Cart(randx, randy));
        players[i].setZeroControl();
        
    }
    //std::cout << "\ncomplete\n"<< std::flush;
}


// copy and move stuff

Team::Team(Team& team) : tcf(team.tcf) , player_count(team.player_count), home_team(team.home_team), teamstrat(team.teamstrat), pitch_data(team.pitch_data)
{
    players = std::make_unique<Player[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        players[i] = team.players[i];
        players[i].setTeamPtr(this);

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
    pitch_data = team.pitch_data;

    players = std::make_unique<Player[]>(player_count);
    for (int i{0}; i < player_count; i++) {
        players[i] = team.players[i];
        players[i].setTeamPtr(this);

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
    pitch_data = team.pitch_data;

    players = std::move(team.players);
    for (int i{0}; i < player_count; i++) {
        players[i].setTeamPtr(this);

    }
    team.player_count = 0;

    return *this;
}

Player& Team::getPlayer(int i) { return players[i]; }
Player* Team::getPlayerPtr(int i) { return &players[i]; }

bool Team::isHomeTeam() const { return home_team; }
int Team::getPlayerCount() const { return player_count; }

void Team::update(Frame frame) { teamstrat->updateTeam(*this, frame); }

void Team::addControl(int i, double dc) { players[i].addControl(dc); }
void Team::setZeroControl(int i) { players[i].setZeroControl(); }
double Team::getControl(int i) { return players[i].getControl(); }
void Team::normControl(double norm) { 
    for (int i{0}; i< player_count; i++){
        players[i].normControl(norm);
    }
}

Pitch* Team::getPitchData() {
    return pitch_data;
}

double Team::xlim() {
    // std::cout << pitch_data->getXlim() << std::endl;
    return pitch_data->getXlim();
}

double Team::ylim() {
    // std::cout << pitch_data->getYlim() << std::endl;

    return pitch_data->getYlim();
}