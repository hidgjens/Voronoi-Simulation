#include "Ball.h"

Ball::Ball() {

}

Ball::Ball(double X, double Y) {
  setPos(X, Y);
  setName("Ball");
}

Ball::Ball(Cart init_pos) {
  setPos(init_pos);
  setName("Ball");
}

void Ball::updateFrame(Match& match){}
