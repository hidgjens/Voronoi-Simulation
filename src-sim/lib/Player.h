#ifndef PLAYER_H
#define PLAYER_H

#include "PitchObject.h"
#include "ai/AI.h"

class Team;

class Player: public PitchObject {
private:
  // player info
  int shirtNum;
  std::string team;
  bool smart;
  double maxStep;
  bool* possession; // ptr to teams possession bool (saves updating for individuals)
  Team* team_players;

public:
  // default constructor
  Player();
  // parameterised constructors
  Player(int); // shirtNum
  Player(int, std::string); // shirtNum, team
  Player(int, std::string, bool); // shirtNum, team, AI

  // accessors and mutators
  int getShirtNum() const;
  void setShirtNum(int);

  std::string getTeam() const;
  void setTeam(std::string);

  double getMaxStep() const;
  void setMaxStep(double);

  void setTeamPtr(Team*);
  Team* getTeamPtr();

  bool getSmart() const;
  std::string getSmartStr() const;
  void setSmart(bool);

  double getCtrl(Match&) const;
  // double getCtrl2(Match&) const;

  void setIPos(Cart);
  void setIdPos(Cart);

  void linkPossession(bool*);
  bool getPossession() const;
  std::string getPossession_txt() const;

  void printDetails();


};

#endif
