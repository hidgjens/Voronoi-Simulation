#ifndef EXCHDSQRD_H
#define EXCHDSQRD_H

#include "AI.h"
#include "../Player.h"
#include "DSquaredSTD.h"


class ExchDsqrd: public AI {
private:
  DSquaredSTD dsq;

public:
  // constructors
  ExchDsqrd();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
