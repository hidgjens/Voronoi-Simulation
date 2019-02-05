#ifndef METRICAI_H
#define METRICAI_H

#include "AI.h"
#include "ExchangeAI.h"
#include "../Player.h"
#include "cfgs/DSquaredConfigFile.h"
#include "cfgs/ExchangeConfigFile.h"

class MetricAI: public AI {

private:
  double decay_const;
  double sm_coeff;
  double op_coeff;

  DSquaredConfigFile dscf;
  ExchangeConfigFile ecf;

public:
  // constructors
  MetricAI();
  MetricAI(double, double, double);

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
