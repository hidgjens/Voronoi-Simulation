#ifndef BALL_H
#define BALL_H

#include "PitchObject.h"

class Ball: public PitchObject {
private:

public:
  // default constructor
  Ball();
  // parameterised constructors
  Ball(double, double);

  Ball(Cart); // pos

  void updateFrame(Match&);
};

#endif
