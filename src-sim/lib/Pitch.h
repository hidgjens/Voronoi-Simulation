#ifndef PITCH_H
#define PITCH_H

#include<iostream>
#include<string>
#include<cmath>
#include<memory>
#include<utility>
#include "Team.h"
#include "vect/Point.h"
#include "PitchModel.h"

class Match;

class Pitch{
private:
  // simulation info
  double framerate;
  int frame;

  PitchModel* pm;

  // pitch dims
  double pitchX;
  double pitchY;
  int xSamp;
  int ySamp;

  // player info
  int homeCnt;
  int awayCnt;
  std::unique_ptr<Cart[]> homePos;
  std::unique_ptr<Cart[]> awayPos;
  std::unique_ptr<int[]> control;
  // std::unique_ptr<Point[]> points;
  double homeCtrl;
  double awayCtrl;
  std::unique_ptr<double[]> homePlyrCtrl;
  std::unique_ptr<double[]> awayPlyrCtrl;
  
  // contested control
  double contestedHomeControl;
  double contestedAwayControl;
  std::unique_ptr<double[]> contestedHomePlyrCtrl;
  std::unique_ptr<double[]> contestedAwayPlyrCtrl;

  // ball
  Cart ball;
  bool homePossession;

public:
  // default constructor
  Pitch();
  // parameterised constructor
  Pitch(int); // frame
  Pitch(Match&);

  // copy and move constructors
  Pitch(Pitch&);
  Pitch(Pitch&& ptch) { *this = std::move(ptch); }

  // copy and move assignments
  Pitch& operator=(Pitch&);
  Pitch& operator=(Pitch&&);

  // accessors and mutators
  // sim info
  double getFramerate() const;
  void setFramerate(double);

  int getFrame() const;
  void setFrame(int);

  double getMatchTime() const;

  // pitch dims
  double getPitchX() const;
  double getPitchY() const;

  void setPitchX(double);
  void setPitchY(double);
  void setPitchDims(double, double);

  void storeFrame(Match&);
  void computeControl();
  //void computeControl2();
  Cart convertIdx2Coords(int, int);

  double getHomeCtrl() const;
  double getAwayCtrl() const;
  double getContHomeCtrl() const;
  double getContAwayCtrl() const;

  Cart getHomePos(int) const;
  Cart getAwayPos(int) const;
  Cart getBallPos() const;

  double getHomePlyrCtrl(int) const;
  double getAwayPlyrCtrl(int) const;
  double getContHomePlyrCtrl(int) const;
  double getContAwayPlyrCtrl(int) const;

  bool getHomePossession() const;
  bool getAwayPossession() const;
  std::string getHomePossStr() const;
  std::string getAwayPossStr() const;

};

#endif
