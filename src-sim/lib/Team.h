#ifndef TEAM_H
#define TEAM_H

#include<iostream>
#include<string>
#include<cmath>
#include<memory>
#include "Player.h"
#include "ai/RandomWalk.h"
#include "ai/AI.h"


class Match;
class Cart;

class Team{
private:
  std::string name;

  int plyrcnt;
  int smartPlyrcnt;
  std::unique_ptr<Player[]> plyrs;

  AI* smartAI;
  AI* normAI;

public:
  // default constructor
  Team();
  // parameterised constructor
  Team(int); // player count
  Team(std::string); // team name
  Team(std::string, int); // name, count
  Team(std::string, int, AI&); // name, count, AI
  Team(std::string, int, AI&, int, AI&); // name, team count, default AI, smrt count, smart AI
  Team(std::string, int, AI*, int, AI*); // name, team count, default AI, smrt count, smart AI
  // Team(std::string, int, AI&, int, RandomWalk&); // name, team count, default AI, smrt count, smart AI

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

  void initRandObjPos(Match&);
  void initCentre(Match&);
  void initUnitPolygon(Match&);

  void updateFrame(Match&);

  const Player getPlayer(int) const; // shirt num
  Player& getPlayerR(int);

  double minDist(Cart); // minimum distance to point (checks all players)

  void scatterPlayer(int, double, double); // scatter shirt number by angle and radius

};

#endif
