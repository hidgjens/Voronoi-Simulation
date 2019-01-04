#ifndef METRICAISTD_H
#define METRICAISTD_H

#include "AI.h"
#include "ExchangeAI.h"
#include "../Player.h"
#include "cfgs/DSquaredConfigFile.h"
#include "cfgs/ExchangeConfigFile.h"

class MetricAISTD: public AI {

private:
  double decay_const;
  double sm_coeff;
  double op_coeff;

  DSquaredConfigFile dscf;
  ExchangeConfigFile ecf;

public:
  // constructors
  MetricAISTD();
  MetricAISTD(double, double, double);

  void updateFrame(Player&, Match&);

  virtual double metricD(Player&, Player&, Match&);
  virtual Cart metricV(Player&, Player&, Match&);

};

#endif
