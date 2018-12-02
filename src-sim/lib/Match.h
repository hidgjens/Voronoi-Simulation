#ifndef MATCH_H
#define MATCH_H

#include<iostream>
#include<string>
#include<memory>
#include "Pitch.h"
#include "Team.h"
#include "ai/AI.h"
#include "cfg/MatchConfigFile.h"
#include "cfg/TeamConfigFile.h"
#include "Ball.h"

class PitchModel;


class Match{
  friend class Pitch;
private:
  int matchID;
  int frames;
  int currentframe;
  double framerate;

  // pitch dimensions
  double pitchX;
  double pitchY;

  // pitches
  std::unique_ptr<Pitch[]> pitches;

  // teams
  Team homeTeam;
  Team awayTeam;
  double homeCtrl;
  double awayCtrl;
  double homeCtrl2;
  double awayCtrl2;

  // ball
  Ball ball;
  bool homePossession; // whether home or away possesses the ball

  PitchModel* pm;

public:
  Match();
  // Match(int); // frames based off 90 mins
  // Match(int, double); // frames, framerate
  // Match(int, int, int); // frames, home, away
  // Match(int, Team, Team, double, double); // frames, homeTeam, awayTeam, pitchX, pitchY
  Match(MatchConfigFile);
  Match(MatchConfigFile, TeamConfigFile, TeamConfigFile);


  // copy and move constructors
  Match(Match&);
  Match(Match&& match) { *this = std::move(match);}

  // copy and move assignments
  Match& operator=(Match&);
  Match& operator=(Match&&);

  // pm
  PitchModel* getModel();
  void setModel(PitchModel*);

  // accessors and mutators
  // sim info
  double getFramerate() const;
  int getFrames() const;
  int getCurrentFrame() const;
  void setMatchID(int);

  // pitch info
  double getPitchX() const;
  double getPitchY() const;
  Pitch& getCurrentPitch() const;

  // Player info
  int getHomeCnt() const;
  int getAwayCnt() const;
  int getPlayerCnt() const; // total players
  void printPlayers() const;
  void printPitch() const;

  // initialise pitch
  void initRandObjPos();
  void initUnitPolygon();
  void updateFrame();
  void checkCollisions();
  void checkCollisions(Player&);

  // get teams
  Team& getHomeTeam();
  Team& getAwayTeam();

  // run simulation
  void startSimulation(bool);

  // get control
  double avgHomeCtrl() const;
  double avgAwayCtrl() const;
  double avgHomeCtrl2() const;
  double avgAwayCtrl2() const;

  // store match
  void saveMatchToFile(std::string, int, bool) const; // file name

  void setHomePossession(bool);
  bool getHomePossession() const;
  bool getAwayPossession() const;
  void toggleHomePossession();


};

#endif
