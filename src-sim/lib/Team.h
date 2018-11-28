#ifndef TEAM_H
#define TEAM_H

#include<iostream>
#include<string>
#include<cmath>
#include<memory>
#include "Player.h"
#include "ai/AI.h"
#include "cfg/TeamConfigFile.h"


class Match;
class Cart;

class Team{
private:
  std::string name;

  int plyrcnt;
  int smartPlyrcnt;
  std::unique_ptr<Player[]> plyrs;

  AI* ATK_smartAI;
  AI* ATK_normAI;
  AI* DEF_smartAI;
  AI* DEF_normAI;

  bool possession;

public:
  // default constructor
  Team();
  Team(TeamConfigFile); // build from config

  // copy and move constructors
  Team(Team&);
  Team(Team&& team) { *this = std::move(team); }

  // copy and move assignments
  Team& operator=(Team&);
  Team& operator=(Team&&);

  // build team
  void buildTeam();
  void numberPlayers(); // give players shirt number

  // accessors and mutators
  int getPlayerCount() const;
  void printPlayers() const;
  Cart getPos(int) const; // get player i position
  double getPlyrCtrl(int, Match&) const;

  void setPossession(bool);
  bool getPossession() const;
  void togglePossession();

  void initRandObjPos(Match&);
  void initCentre(Match&);
  void initUnitPolygon(Match&);

  void updateFrame(Match&);

  const Player getPlayer(int) const; // shirt num
  Player& getPlayerR(int);
  Player* getPlayerPtr(int);

  double minDist(Cart); // minimum distance to point (checks all players)

  void scatterPlayer(int, double, double); // scatter shirt number by angle and radius

  Player* nearestPlayer(Player&); // return pointer to the nearest player to a point

};

#endif
