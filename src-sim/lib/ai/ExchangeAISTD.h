#ifndef EXCHANGEAISTD_H
#define EXCHANGEAISTD_H

#include "AI.h"
#include "../Player.h"

class ExchangeAISTD: public AI {

public:
  // constructors
  ExchangeAISTD();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
