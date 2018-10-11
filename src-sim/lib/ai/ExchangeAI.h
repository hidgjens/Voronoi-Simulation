#ifndef EXCHANGEAI_H
#define EXCHANGEAI_H

#include "AI.h"
#include "../Player.h"

class ExchangeAI: public AI {

public:
  // constructors
  ExchangeAI();

  void updateFrame(Player&, Match&);

};

#endif
