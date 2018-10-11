/*
Basic scenario for testing. Home team has 1 smart playing using the RandomWalk AI, while everyone else uses the default (stationary) AI
*/

#ifndef SCENARIO_H
#define SCENARIO_H

#include<iostream>
#include<chrono>
#include "../Match.h"

class Scenario {
private:
  std::string filename;
  int homeCnt;
  int awayCnt;
  int frames;
  double pitchX;
  double pitchY;
  Team homeTeam;
  Team awayTeam;
  // set the AI for each team
  AI hNormAI;// home normal
  RandomWalk hSmartAI; // home smart
  AI aAI; // away AI

  bool legacy;

protected:
  void setFilename(std::string);
  std::string getFilename() const;

  void setHomeCnt(int);
  int getHomeCnt() const;
  void setAwayCnt(int);
  int getAwayCnt() const;

  void setFrames(int);
  int getFrames() const;

  void setPitchDims(double, double);
  double getPitchX() const;
  double getPitchY() const;

  void setHomeTeam(Team);
  Team& getHomeTeam();
  void setAwayTeam(Team);
  Team& getAwayTeam();

  void setLegacy(bool);
  bool getLegacy() const;

  virtual Match buildMatch();


public:
  Scenario();
  Scenario(std::string);
  Scenario(int, std::string);
  Scenario(int, std::string, bool);

  virtual void start(int, bool); // num frames, start num = 0
  virtual void start(int, int, bool); // num frames, start num, legacy
};

#endif
