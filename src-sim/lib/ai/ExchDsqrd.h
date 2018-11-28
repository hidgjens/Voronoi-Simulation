#ifndef EXCHDSQRD_H
#define EXCHDSQRD_H

#include "AI.h"
#include "../Player.h"
#include "DSquared.h"


class ExchDsqrd: public AI {
private:
  DSquared dsq;

public:
  // constructors
  ExchDsqrd();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
