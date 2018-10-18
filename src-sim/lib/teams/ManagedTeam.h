#ifndef MANAGEDTEAM_H
#define MANAGEDTEAM_H

#include "../Team.h"
#include "../ai/RandomWalk.h"
#include "../ai/AI.h"

class ManagedTeam{
  // probably not the best way to do this, but this an attempt at a more flexible way to build teams of different types. The smart and normal AI are stored in the managed team, the addresses of which are passed to the Team class stored within this managed team. Then there is one Managed Team object which contains everything a team needs, which can pass a reference to the team it is storing to a Match class
private:
  std::string name;
  int plyrCnt;
  int smartCnt;
  AI normAI;
  AI smartAI;
  Team team;

public:
  // constructors
  ManagedTeam();
  ManagedTeam(std::string); // team name
  ManagedTeam(std::string, int); // team name, players
  ManagedTeam(std::string, int, int); // team name, players, smart

  void buildTeam(std::string, int, int);
  Team& getTeam();
  void setTeam(Team);

  // accessor and mutators
  virtual void setNormAI(AI*);
  virtual void setSmartAI(AI*);
  virtual AI* getNormAI();
  virtual AI* getSmartAI();

  void setPlyrCnt(int);
  int getPlyrCnt() const;
  void setSmartCnt(int);
  int getSmartCnt() const;

  void setName(std::string);
  std::string getName() const;
};

#endif
