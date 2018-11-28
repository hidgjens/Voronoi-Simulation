#include "Team.h"
#include<cmath>
#include "Match.h"
# define M_PI           3.14159265358979323846  /* pi */

// Team::Team() : plyrcnt(11) , name("Team") {
//   buildTeam();
// }

// Team::Team(int plyrs) : plyrcnt(plyrs) , name("Team") {
//   buildTeam();
// }

// Team::Team(std::string nm) : plyrcnt(11) , name(nm) {
//   buildTeam();
// }

// Team::Team(std::string nm, int plyrs) : plyrcnt(plyrs), name(nm) {
//   buildTeam();
// }

// Team::Team(std::string nm, int plyrs, AI& nAI) : name(nm), plyrcnt(plyrs), smartPlyrcnt(0) {
//   smartAI = &nAI;
//   normAI = &nAI;
//   buildTeam();
// }

// Team::Team(std::string nm, int plyrs, AI& nAI, int splyrs, AI& smAI) : name(nm), plyrcnt(plyrs), normAI(&nAI), smartPlyrcnt(splyrs), smartAI(&smAI) {
//   buildTeam();
// }

// Team::Team(std::string nm, int plyrs, AI* nAI, int splyrs, AI* smAI) : name(nm), plyrcnt(plyrs), normAI(nAI), smartPlyrcnt(splyrs), smartAI(smAI) {
//   buildTeam();
// }

// Team::Team(std::string nm, int plyrs, AI& nAI, int splyrs, RandomWalk& smAI) : name(nm), plyrcnt(plyrs), normAI(nAI), smartPlyrcnt(splyrs), smartAI(smAI) {
//   std::cout << splyrs << smAI.getDesc() << " spec AI\n";
//   buildTeam();
// }

Team::Team() {}

Team::Team(TeamConfigFile tcf_) 
: name(tcf_.getTeamName())
, plyrcnt(tcf_.getPlayerCount())
, smartPlyrcnt(tcf_.getSmartCount())
{
  std::cout << "Team " + name + "\n";
  // need to get AI from the TeamConfigFile
  ATK_smartAI = AI::makeAI(tcf_.getAtkSmartAI());
  ATK_normAI = AI::makeAI(tcf_.getAtkNormAI());
  DEF_smartAI = AI::makeAI(tcf_.getDefSmartAI());
  DEF_normAI = AI::makeAI(tcf_.getDefNormAI());
  // build team
  buildTeam();
}

// copy constructor
Team::Team(Team& team) : plyrcnt(team.plyrcnt) , name(team.name) , smartPlyrcnt(team.smartPlyrcnt) , ATK_smartAI(team.ATK_smartAI) , ATK_normAI(team.ATK_normAI), DEF_smartAI(team.DEF_smartAI), DEF_normAI(team.DEF_normAI) {
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

  name = team.name;
  plyrcnt = team.plyrcnt;
  smartPlyrcnt = team.smartPlyrcnt;
  ATK_smartAI = team.ATK_smartAI;
  ATK_normAI = team.ATK_normAI;
  DEF_smartAI = team.DEF_smartAI;
  DEF_normAI = team.DEF_normAI;

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
  ATK_smartAI = team.ATK_smartAI;
  ATK_normAI = team.ATK_normAI;
  DEF_smartAI = team.DEF_smartAI;
  DEF_normAI = team.DEF_normAI;
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
      plyrs[i].setTeamPtr(this);
      plyrs[i].linkPossession(&possession);
    } else {
      // normal player
      plyrs[i] = Player(i + 1, name, false);
      plyrs[i].setTeamPtr(this);
      plyrs[i].linkPossession(&possession);

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

void Team::setPossession(bool possess_) { possession = possess_; }
bool Team::getPossession() const { return possession; }
void Team::togglePossession() { possession = !possession; }


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
      if (possession)
        ATK_smartAI->updateFrame(plyrs[i], ptch);
      else
        DEF_smartAI->updateFrame(plyrs[i], ptch);
    } else {
      // normal player
      if (possession)
        ATK_normAI->updateFrame(plyrs[i], ptch);
      else 
        DEF_normAI->updateFrame(plyrs[i], ptch);
    }

  }
}

const Player Team::getPlayer(int shirt_num) const {
  return plyrs[shirt_num - 1];
}
Player& Team::getPlayerR(int shirt_num){
 return plyrs[shirt_num - 1];
}
Player* Team::getPlayerPtr(int shirt_num){
  return &plyrs[shirt_num - 1 ];
}

double Team::minDist(Cart ps) {
  double mindist = plyrs[0].getPos().dist(ps); // return variable;
  
  double tempdist;
  for (int i{1}; i < plyrcnt; i++){ // not counting from 0, since mindist is plyr 0 by default
    tempdist = plyrs[i].getPos().dist(ps);
    if (tempdist < mindist) {
      // current player is closer, update, mindist
      mindist = tempdist;
    }
  }
  // mindist is the distance of the closest player
  return mindist;
}

void Team::scatterPlayer(int shirt_num, double r, double theta){
  plyrs[shirt_num - 1].setdPos(r * cos(theta), r * sin(theta));
}

Player* Team::nearestPlayer(Player& targetPlayer) {
  Cart pos = targetPlayer.getPos();
  double mindist = -1.0;
  Player* closestPlayer;
  double testdist;
  Player* testPlayer;
  Cart temp_player_pos;

  for (int i{0}; i < plyrcnt; i++){
    testPlayer = &plyrs[i];
    if (testPlayer == &targetPlayer) { //testPlayer->printDetails(); targetPlayer.printDetails(); 
      continue; } // skip if the same

    temp_player_pos = testPlayer->getPos();
    testdist = pos.dist(temp_player_pos);
    if (testdist < mindist || mindist < 0) { // < 0 is default meaning unset
      mindist = testdist;
      closestPlayer = testPlayer;
    }
  }

  return closestPlayer;
  
}