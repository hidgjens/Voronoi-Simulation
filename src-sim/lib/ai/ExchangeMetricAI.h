#ifndef EXCHANGEMETRICAI_H
#define EXCHANGEMETRICAI_H

#include "AI.h"
#include "ExchangeAI.h"
#include "../Player.h"

class ExchangeMetricAI: public AI {

private:
  double decay_const;
  double sm_coeff;
  double op_coeff;

public:
  // constructors
  ExchangeMetricAI();
  ExchangeMetricAI(double, double, double);

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
