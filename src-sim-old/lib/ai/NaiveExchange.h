#ifndef NAIVEEXCHANGE_H
#define NAIVEEXCHANGE_H

#include "AI.h"
#include "../Player.h"

#include "cfgs/ExchangeConfigFile.h"

class NaiveExchange: public AI {

private:
  ExchangeConfigFile ecf;

public:
  // constructors
  NaiveExchange();

  void updateFrame(Player&, Match&);

};

#endif
