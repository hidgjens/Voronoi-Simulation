#ifndef PLAYER_H
#define PLAYER_H

#include "PitchObject.h"
#include "ai/AI.h"

class Player: public PitchObject {
private:
  // player info
  int shirtNum;
  std::string team;
  bool smart;
  double maxStep;

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

  bool getSmart() const;
  std::string getSmartStr() const;
  void setSmart(bool);

  double getCtrl(Match&) const;

  void setIPos(Cart);
  void setIdPos(Cart);


};

#endif
