#include "Team.h"
#include<cmath>
#include "Match.h"
# define M_PI           3.14159265358979323846  /* pi */

Team::Team() : plyrcnt(11) , name("Team") {
  buildTeam();
}

Team::Team(int plyrs) : plyrcnt(plyrs) , name("Team") {
  buildTeam();
}

Team::Team(std::string nm) : plyrcnt(11) , name(nm) {
  buildTeam();
}

Team::Team(std::string nm, int plyrs) : plyrcnt(plyrs), name(nm) {
  buildTeam();
}

Team::Team(std::string nm, int plyrs, AI& nAI) : name(nm), plyrcnt(plyrs), smartPlyrcnt(0) {
  smartAI = &nAI;
  normAI = &nAI;
  buildTeam();
}

Team::Team(std::string nm, int plyrs, AI& nAI, int splyrs, AI& smAI) : name(nm), plyrcnt(plyrs), normAI(&nAI), smartPlyrcnt(splyrs), smartAI(&smAI) {
  buildTeam();
}

Team::Team(std::string nm, int plyrs, AI* nAI, int splyrs, AI* smAI) : name(nm), plyrcnt(plyrs), normAI(nAI), smartPlyrcnt(splyrs), smartAI(smAI) {
  buildTeam();
}

// Team::Team(std::string nm, int plyrs, AI& nAI, int splyrs, RandomWalk& smAI) : name(nm), plyrcnt(plyrs), normAI(nAI), smartPlyrcnt(splyrs), smartAI(smAI) {
//   std::cout << splyrs << smAI.getDesc() << " spec AI\n";
//   buildTeam();
// }

// copy constructor
Team::Team(Team& team) : plyrcnt(team.plyrcnt) , name(team.name) , smartPlyrcnt(team.smartPlyrcnt) , smartAI(team.smartAI) , normAI(team.normAI) {
  plyrs = std::make_unique<Player[]>(plyrcnt);
  for (int i{0}; i < plyrcnt; i++){
    plyrs[i] = team.plyrs[i];
  }
}

// deep copy assignment
Team& Team::operator=(Team& team) {
  if (&team == this) { // no self assignment
    return *this;
  }

  plyrcnt = team.plyrcnt;
  name = team.name;
  smartPlyrcnt = team.smartPlyrcnt;
  smartAI = team.smartAI;
  normAI = team.normAI;

  plyrs = std::make_unique<Player[]>(plyrcnt);
  for (int i{0}; i < plyrcnt; i++){
    plyrs[i] = team.plyrs[i];
  }
  return *this;
}

// move assignment
Team& Team::operator=(Team&& team) {
  if (&team == this) {
    return *this;
  }
  plyrcnt = team.plyrcnt;
  name = team.name;
  smartPlyrcnt = team.smartPlyrcnt;
  smartAI = team.smartAI;
  normAI = team.normAI;

  plyrs = std::move(team.plyrs);

  team.plyrcnt = 0;
  team.smartPlyrcnt = 0;

  return *this;
}

void Team::buildTeam(){
  plyrs = std::make_unique<Player[]>(plyrcnt);
  for (int i{0}; i < plyrcnt; i++){
    if (i < smartPlyrcnt){
      // smart player
      plyrs[i] = Player(i + 1, name, true);
    } else {
      // normal player
      plyrs[i] = Player(i + 1, name, false);
    }
  }
}

int Team::getPlayerCount() const {
  return plyrcnt;
}
void Team::printPlayers() const {
  std::cout << name + "\n";
  for (int i{0}; i < plyrcnt; i++){
    std::cout << "\t" << plyrs[i].getName() << ": ";
    plyrs[i].printPos();
    std::cout << "\n";
  }
}
Cart Team::getPos(int shirt_num) const {
  return plyrs[shirt_num - 1].getPos();
}
double Team::getPlyrCtrl(int shirt_num, Match& match) const {
  return plyrs[shirt_num - 1].getCtrl(match);
}


void Team::initRandObjPos(Match& ptch) {
  for (int i{0}; i < plyrcnt; i++) {
    plyrs[i].randPos(ptch);
  }
}
void Team::initCentre(Match& ptch) {
  for (int i{0}; i < plyrcnt; i++) {
    plyrs[i].setPos(0.0, 0.0);
  }
}
void Team::initUnitPolygon(Match& ptch) {
  double dTheta{2 * M_PI / plyrcnt};

  for (int i{0}; i < plyrcnt; i++) {
    plyrs[i].setPos(0.5 * ptch.getPitchX() * cos(dTheta * i), 0.5 * ptch.getPitchY() * sin(dTheta * i));
  }
}

void Team::updateFrame(Match& ptch) {
  //std::cout << "smartPlyrcnt " << smartPlyrcnt << "\n";
  //std::cout << plyrcnt;
  for (int i{0}; i < plyrcnt; i++) {
    //std::cout << i;
    if (i < smartPlyrcnt){
      // smart player
      //std::cout << "smart";

      smartAI->updateFrame(plyrs[i], ptch);
    } else {
      // normal player
      normAI->updateFrame(plyrs[i], ptch);
    }

  }
}

 const Player Team::getPlayer(int shirt_num) const {
  return plyrs[shirt_num - 1];
}
Player& Team::getPlayerR(int shirt_num){
 return plyrs[shirt_num - 1];
}

void Team::scatterPlayer(int shirt_num, double r, double theta){
  plyrs[shirt_num - 1].setdPos(r * cos(theta), r * sin(theta));
}
