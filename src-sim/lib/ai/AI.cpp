#include "AI.h"
#include "../Player.h"
#include "../Match.h"
// AI for factory
#include "ExchangeAI.h"
#include "MetricAI.h"
#include "RandomWalk.h"
#include "TestingAI.h"
#include "Stationary.h"
#include "Spreading.h"
#include "DSquared.h"
#include "ExchDsqrd.h"
#include "DSquaredSTD.h"
#include "ExchangeAISTD.h"
#include "NaiveExchange.h"
#include "MetricAISTD.h"

AI::AI() : desc("unspecified") {}

AI::AI(std::string des) : desc(des) {}

AI* AI::makeAI(std::string ai_type){
  /*
    AI factory, takes string of AI name and returns that AI.
    To add new AI, include it at the top of this file, then add an if statement following the examples
  */

  std::cout << ai_type + "\n";
  // return pointer
  AI* return_ai;

  if (ai_type == "RandomWalk")
    return_ai = new RandomWalk();
  else
  if (ai_type == "Testing")
    return_ai = new TestingAI();
  else
  if (ai_type == "Exchange")
    return_ai = new ExchangeAI();
  else
  if (ai_type == "Metric")
    return_ai = new MetricAI();
  else
  if (ai_type == "Stationary")
    return_ai = new StationaryAI();
  else
  if (ai_type == "Spreading")
    return_ai = new Spreading();
  else
  if (ai_type == "DSquared")
    return_ai = new DSquared();
  else
  if (ai_type == "ExchDsqrd")
    return_ai = new ExchDsqrd();
  else
  if (ai_type == "DSquaredSTD")
    return_ai = new DSquaredSTD();
  else
  if (ai_type == "ExchangeSTD")
    return_ai = new ExchangeAISTD();
  else
  if (ai_type == "NaiveExch")
    return_ai = new NaiveExchange();
  else
  if (ai_type == "MetricSTD")
    return_ai = new MetricAISTD();
  /*
    add more here
  */
  else {
    // unrecognised argument, throw exception
    throw std::invalid_argument("Invalid AI type: " + ai_type);
  }

  // set up the AI

  return return_ai;

}

void AI::setDesc(std::string des) {
  desc = des;
}
std::string AI::getDesc() const {
  return desc;
}

void AI::updateFrame(Player& plyr, Match& match) {
  //std::cout<< "AI UPDATE\n";
}
