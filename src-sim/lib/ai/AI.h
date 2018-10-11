#ifndef AI_h
#define AI_h

#include<string>
#include<iostream>

// forward declarations
class Player;
class Match;

class AI{
private:
  std::string desc;
public:
  // constructors
  AI(); // defaul constructor
  AI(std::string); // desc

  void setDesc(std::string);
  std::string getDesc() const;

  virtual void updateFrame(Player&, Match&);
};

#endif
