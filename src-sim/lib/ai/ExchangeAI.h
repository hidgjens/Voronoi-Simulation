#ifndef EXCHANGEAI_H
#define EXCHANGEAI_H

#include "AI.h"
#include "../Player.h"
#include "cfgs/ExchangeConfigFile.h"

class ExchangeAI: public AI {
private:
  ExchangeConfigFile ecf;

public:
  // constructors
  ExchangeAI();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
