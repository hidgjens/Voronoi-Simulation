#ifndef EXCHDSQRD_H
#define EXCHDSQRD_H

#include "AI.h"
#include "../Player.h"
#include "DSquared.h"
#include "cfgs/ExchangeConfigFile.h"


class ExchDsqrd: public AI {
private:
  DSquared dsq;

  ExchangeConfigFile ecf;

public:
  // constructors
  ExchDsqrd();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
