#include "PitchObject.h"
#include "Match.h"
#include <cmath>
# define M_PI           3.14159265358979323846  /* pi */

// default constructor
PitchObject::PitchObject()
  : name("PitchObject")
  , pos(Cart(0.0, 0.0))
  , vel(Cart(0.0, 0.0))
{}

// accessors and mutators
// name
std::string PitchObject::getName() const{
  return name;
}
void PitchObject::setName(std::string new_nm){
  name = new_nm;
}

// position
double PitchObject::getPosX() const{
  return pos.xComp();
}
double PitchObject::getPosY() const{
  return pos.yComp();
}
Cart PitchObject::getPos() const{
  return pos;
}
void PitchObject::setPos(Cart new_p) {
  pos = new_p;
}
void PitchObject::setPosX(double new_pX){
  pos.setX(new_pX);
}
void PitchObject::setPosY(double new_pY){
  pos.setY(new_pY);
}
void PitchObject::setPos(double new_pX, double new_pY){
  pos.set(new_pX, new_pY);
}
void PitchObject::setdPosX(double dPosX){
  pos.dX(dPosX);
}
void PitchObject::setdPosY(double dPosY){
  pos.dY(dPosY);
}
void PitchObject::setdPos(Cart dp){
  pos += dp;
}
void PitchObject::setdPos(double dPosX, double dPosY){
  pos.d(dPosX, dPosY);
}
void PitchObject::printPos() const{
  pos.print();
}

void PitchObject::randPos(Match& ptch){
  // get bounds of pitch
  // double lower_boundX = -52.5;//-1.0 * ptch.getPitchX() / 2.0;
  // double upper_boundX = 52.5;//ptch.getPitchX() / 2.0;
  // double lower_boundY = -34.0;//-1.0 * ptch.getPitchY() / 2.0;
  // double upper_boundY = 34.0;//ptch.getPitchY() / 2.0;

  // generate random number for pitch coords
  // initialise seed
  setPosX((((double) rand() / RAND_MAX) - 0.5) * ptch.getPitchX());
  setPosY((((double) rand() / RAND_MAX) - 0.5) * ptch.getPitchY());
}

void PitchObject::updateFrame(Match& ptch) {
  // normal update for the 'dim' player

  //double radius = (((double) rand() / RAND_MAX) - 0.5) * 1.0;
  //double phi = ((double) rand() / RAND_MAX) * 360.0;
  //setdPosX(radius * cos(phi));
  //setdPosY(radius * sin(phi));

checkLegalPosition(ptch);
}

void PitchObject::smartUpdate(Match& ptch) {
  // special update for the 'smart' player
  // double radius = (((double) rand() / RAND_MAX) - 0.5) * ptch.getPitchX();
  // double phi = ((double) rand() / RAND_MAX) * 2 * M_PI ;
  // setdPosX(radius * cos(phi));
  // setdPosY(radius * sin(phi));
  if (ptch.getCurrentFrame() % 2 == 0){ // even frame reset
    setPos(0.0, 0.0);
  } else {
    double dx = ((double) rand() / RAND_MAX - 0.5) * ptch.getPitchX();
    double dy = ((double) rand() / RAND_MAX - 0.5) * (ptch.getPitchY() - 2.0 * fabs(dx));
    setdPos(0.5 * dx, 0.5 * dy);
  }
  checkLegalPosition(ptch);

}

void PitchObject::checkLegalPosition(Match& match){
  if (getPosX() > match.getPitchX() / 2.0){
    setPosX(match.getPitchX() / 2.0);
  }
  if (getPosX() < -1.0 * match.getPitchX() / 2.0){
    setPosX( -1.0 * match.getPitchX() / 2.0);
  }
  if (getPosY() > match.getPitchY() / 2.0){
    setPosY(match.getPitchY() / 2.0);
  }
  if (getPosY() < -1.0 * match.getPitchY() / 2.0){
    setPosY( -1.0 * match.getPitchY() / 2.0);
  }
}

void PitchObject::checkSmartLegalPosition(Match& match){

  if (getPosX() > match.getPitchX() / 2.0){
    std::cout << "illegal " << getPosX() << " " << getPosY() << "\n";
    setPosX(match.getPitchX() / 2.0);
  }
  if (getPosX() < -1.0 * match.getPitchX() / 2.0){
    std::cout << "illegal " << getPosX() << " " << getPosY() << "\n";
    setPosX( -1.0 * match.getPitchX() / 2.0);
  }
  if (getPosY() > match.getPitchY() / 2.0){
    std::cout << "illegal " << getPosX() << " " << getPosY() << "\n";
    setPosY(match.getPitchY() / 2.0);
  }
  if (getPosY() < -1.0 * match.getPitchY() / 2.0){
    std::cout << "illegal " << getPosX() << " " << getPosY() << "\n";
    setPosY( -1.0 * match.getPitchY() / 2.0);
  }
}
