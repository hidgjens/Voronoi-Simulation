#ifndef EXCHANGEMETRICAI_H
#define EXCHANGEMETRICAI_H

#include "AI.h"
#include "ExchangeAI.h"
#include "../Player.h"

class ExchangeMetricAI: public AI {

public:
  // constructors
  ExchangeMetricAI();

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
