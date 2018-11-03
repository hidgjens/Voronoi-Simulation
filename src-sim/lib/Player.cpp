#include "Player.h"
#include "Match.h"

// default constructor
Player::Player()
  : shirtNum(0)
  , team("Neutral")
  , smart(false)
  , maxStep(2.5)
{
  setName(team + "Player" + std::to_string(shirtNum));
  setPos(0.0, 0.0);
}

Player::Player(int sh_num)
  : shirtNum(sh_num)
  , team("Neutral")
  , smart(false)
  , maxStep(2.5)
{
  setName(team + "Player" + std::to_string(shirtNum));
  setPos(0.0, 0.0);
}
Player::Player(int sh_num, std::string tm)
  : shirtNum(sh_num)
  , team(tm)
  , smart(false)
  , maxStep(2.5)
{
  setName(team + "Player" + std::to_string(shirtNum));
  setPos(0.0, 0.0);
}
Player::Player(int sh_num, std::string tm, bool smrt)
  : shirtNum(sh_num)
  , team(tm)
  , smart(smrt)
  , maxStep(2.5)
{
  setName(team + "Player" + std::to_string(shirtNum));
  setPos(0.0, 0.0);
}

// accessors and mutators

int Player::getShirtNum() const{
  return shirtNum;
}
void Player::setShirtNum(int new_sh){
  shirtNum = new_sh;
}

std::string Player::getTeam() const{
  return team;
}
void Player::setTeam(std::string new_tm){
  team = new_tm;
}

double Player::getMaxStep() const {
  return maxStep;
}
void Player::setMaxStep(double mxs) {
  maxStep = mxs;
}
bool Player::getSmart() const {
  return smart;
}
std::string Player::getSmartStr() const {
  if (smart){
    return "True";
  } else {
    return "False";
  }
}
void Player::setSmart(bool smrt) {
  smart = smrt;
}

double Player::getCtrl(Match& match) const {
  if (team == "Home") {
    //std::cout << "I am a HOME player\n";
    return match.getCurrentPitch().getHomePlyrCtrl(shirtNum);
  } else
  if (team == "Away") {
    //std::cout << "I am a AWAY player\n";
    return match.getCurrentPitch().getAwayPlyrCtrl(shirtNum);
  }
}
double Player::getCtrl2(Match& match) const {
  if (team == "Home") {
    //std::cout << "I am a HOME player\n";
    return match.getCurrentPitch().getHomePlyrCtrl2(shirtNum);
  } else
  if (team == "Away") {
    //std::cout << "I am a AWAY player\n";
    return match.getCurrentPitch().getAwayPlyrCtrl2(shirtNum);
  }
}

void Player::setIPos(Cart idealpos){
  // set ideal position, player will move in direction of this position at max step unless the position is reached
  if (getPos().dist(idealpos) > maxStep){
    setdPos(getPos().unitVect2(idealpos) * maxStep);
  } else {
    setdPos(idealpos - getPos());
  }
}
void Player::setIdPos(Cart idealpos){
  // set ideal dPosition, player will move in direction of this position at max step unless the position is reached
  if (idealpos.mod() > maxStep){
    setdPos(idealpos.unitVect() * maxStep);
  } else {
    setdPos(idealpos);
  }
}
