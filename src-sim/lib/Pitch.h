#ifndef PITCH_H
#define PITCH_H

#include<iostream>
#include<string>
#include<cmath>
#include<memory>
#include<utility>
#include "Team.h"

class Match;

class Pitch{
private:
  // simulation info
  double framerate;
  int frame;


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
  std::unique_ptr<double[]> homePlyrCtrl;
  std::unique_ptr<double[]> awayPlyrCtrl;
  double homeCtrl;
  double awayCtrl;

  // ball
  Cart ball;

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
  Cart convertIdx2Coords(int, int);

  double getHomeCtrl() const;
  double getAwayCtrl() const;

  Cart getHomePos(int) const;
  Cart getAwayPos(int) const;
  Cart getBallPos() const;

  double getHomePlyrCtrl(int) const;
  double getAwayPlyrCtrl(int) const;

};

#endif
