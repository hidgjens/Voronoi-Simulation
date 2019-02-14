#include "Ball.h"

Ball::Ball() {
  setObjectRadius(RADIUS);
}

Ball::Ball(double X, double Y) {
  setPos(X, Y);
  setName("Ball");
  setObjectRadius(RADIUS);
}

Ball::Ball(Cart init_pos) {
  setPos(init_pos);
  setName("Ball");
  setObjectRadius(RADIUS);
}

void Ball::updateFrame(Match& match){}
