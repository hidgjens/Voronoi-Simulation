#include "Pitch.h"
#include "Match.h"

// default constructor
Pitch::Pitch()
  : framerate(5.0)
  , frame(0)
  , pitchX(105.0)
  , pitchY(68.0)
  , homeCnt(11)
  , awayCnt(11)
{
  homePos = std::make_unique<Cart[]>(homeCnt);
  awayPos = std::make_unique<Cart[]>(awayCnt);
  xSamp = (int) pitchX;
  ySamp = (int) pitchY;
  control = std::make_unique<int[]>(xSamp * ySamp);
  //points = std::make_unique<Point[]>(xSamp * ySamp);
  homePlyrCtrl = std::make_unique<double[]>(homeCnt);
  awayPlyrCtrl = std::make_unique<double[]>(awayCnt);
  //homePlyrCtrl2 = std::make_unique<double[]>(homeCnt);
  //awayPlyrCtrl2 = std::make_unique<double[]>(awayCnt);
}

// parameterised constructors
Pitch::Pitch(int new_fm)
: framerate(5.0)
, frame(new_fm)
, pitchX(105.0)
, pitchY(68.0)
, homeCnt(11)
, awayCnt(11)
{
  homePos = std::make_unique<Cart[]>(homeCnt);
  awayPos = std::make_unique<Cart[]>(awayCnt);
  xSamp = (int) pitchX;
  ySamp = (int) pitchY;
  control = std::make_unique<int[]>(xSamp * ySamp);
  //points = std::make_unique<Point[]>(xSamp * ySamp);
  homePlyrCtrl = std::make_unique<double[]>(homeCnt);
  awayPlyrCtrl = std::make_unique<double[]>(awayCnt);
  //homePlyrCtrl2 = std::make_unique<double[]>(homeCnt);
  //awayPlyrCtrl2 = std::make_unique<double[]>(awayCnt);
}
// parameterised constructors
Pitch::Pitch(Match& match)
: framerate(match.getFramerate())
, frame(match.getCurrentFrame())
, pitchX(match.getPitchX())
, pitchY(match.getPitchY())
, homeCnt(match.getHomeCnt())
, awayCnt(match.getAwayCnt())
{
  homePos = std::make_unique<Cart[]>(homeCnt);
  awayPos = std::make_unique<Cart[]>(awayCnt);
  xSamp = (int) pitchX;
  ySamp = (int) pitchY;
  control = std::make_unique<int[]>(xSamp * ySamp);
  //points = std::make_unique<Point[]>(xSamp * ySamp);
  homePlyrCtrl = std::make_unique<double[]>(homeCnt);
  awayPlyrCtrl = std::make_unique<double[]>(awayCnt);
  //homePlyrCtrl2 = std::make_unique<double[]>(homeCnt);
  //awayPlyrCtrl2 = std::make_unique<double[]>(awayCnt);
}

// copy constructor
Pitch::Pitch(Pitch& ptch) : homeCnt(ptch.homeCnt), awayCnt(ptch.awayCnt), pitchX(ptch.pitchX), pitchY(ptch.pitchY), framerate(ptch.framerate), frame(ptch.frame), xSamp(ptch.xSamp), ySamp(ptch.ySamp), ball(ptch.ball) {
  homePos = std::make_unique<Cart[]>(homeCnt);
  awayPos = std::make_unique<Cart[]>(awayCnt);
  for (int i{0}; i < homeCnt; i++){
    homePos[i] = ptch.homePos[i];
  }
  for (int i{0}; i < awayCnt; i++){
    awayPos[i] = ptch.awayPos[i];
  }

  control = std::make_unique<int[]>(xSamp * ySamp);
  for (int i{0}; i < xSamp * ySamp; i++){
    control[i] = ptch.control[i];
  }
  // points = std::make_unique<Point[]>(xSamp * ySamp);
  // for (int i{0}; i < xSamp * ySamp; i++){
  //   points[i] = ptch.points[i];
  // }
  homePlyrCtrl = std::make_unique<double[]>(homeCnt);
  awayPlyrCtrl = std::make_unique<double[]>(awayCnt);
  for (int i{0}; i < homeCnt; i++){
    homePlyrCtrl[i] = ptch.homePlyrCtrl[i];
  }
  for (int i{0}; i < awayCnt; i++){
    awayPlyrCtrl[i] = ptch.awayPlyrCtrl[i];
  }

  // homePlyrCtrl2 = std::make_unique<double[]>(homeCnt);
  // awayPlyrCtrl2 = std::make_unique<double[]>(awayCnt);

  // for (int i{0}; i < homeCnt; i++){
  //   homePlyrCtrl2[i] = ptch.homePlyrCtrl2[i];
  // }
  // for (int i{0}; i < awayCnt; i++){
  //   awayPlyrCtrl2[i] = ptch.awayPlyrCtrl2[i];
  // }
}

// deep copy assignments
Pitch& Pitch::operator=(Pitch& ptch){
  if (&ptch == this) { // no self assignment
    return *this;
  }

  homeCnt = ptch.homeCnt;
  awayCnt = ptch.awayCnt;
  pitchX = ptch.pitchX;
  pitchY = ptch.pitchY;
  xSamp = ptch.xSamp;
  ySamp = ptch.ySamp;
  framerate = ptch.framerate;
  frame = ptch.frame;
  ball = ptch.ball;

  homePos = std::make_unique<Cart[]>(homeCnt);
  awayPos = std::make_unique<Cart[]>(awayCnt);
  for (int i{0}; i < homeCnt; i++){
    homePos[i] = ptch.homePos[i];
  }
  for (int i{0}; i < awayCnt; i++){
    awayPos[i] = ptch.awayPos[i];
  }
  control = std::make_unique<int[]>(xSamp * ySamp);
  for (int i{0}; i < xSamp * ySamp; i++){
    control[i] = ptch.control[i];
  }
  // points = std::make_unique<Point[]>(xSamp * ySamp);
  // for (int i{0}; i < xSamp * ySamp; i++){
  //   points[i] = ptch.points[i];
  // }
  homePlyrCtrl = std::make_unique<double[]>(homeCnt);
  awayPlyrCtrl = std::make_unique<double[]>(awayCnt);

  for (int i{0}; i < homeCnt; i++){
    homePlyrCtrl[i] = ptch.homePlyrCtrl[i];
  }
  for (int i{0}; i < awayCnt; i++){
    awayPlyrCtrl[i] = ptch.awayPlyrCtrl[i];
  }

  // homePlyrCtrl2 = std::make_unique<double[]>(homeCnt);
  // awayPlyrCtrl2 = std::make_unique<double[]>(awayCnt);

  // for (int i{0}; i < homeCnt; i++){
  //   homePlyrCtrl2[i] = ptch.homePlyrCtrl2[i];
  // }
  // for (int i{0}; i < awayCnt; i++){
  //   awayPlyrCtrl2[i] = ptch.awayPlyrCtrl2[i];
  // }

  return *this;
}

// move assignment
Pitch& Pitch::operator=(Pitch&& ptch){
  if (&ptch == this) { // so self assignment
    return *this;
  }
  homeCnt = ptch.homeCnt;
  awayCnt = ptch.awayCnt;
  pitchX = ptch.pitchX;
  pitchY = ptch.pitchY;
  xSamp = ptch.xSamp;
  ySamp = ptch.ySamp;
  framerate = ptch.framerate;
  frame = ptch.frame;
  ball = ptch.ball;

  homePos = std::move(ptch.homePos);
  awayPos = std::move(ptch.awayPos);
  control = std::move(ptch.control);
  // points = std::move(ptch.points);
  homePlyrCtrl = std::move(ptch.homePlyrCtrl);
  awayPlyrCtrl = std::move(ptch.awayPlyrCtrl);
  // homePlyrCtrl2 = std::move(ptch.homePlyrCtrl2);
  // awayPlyrCtrl2 = std::move(ptch.awayPlyrCtrl2);

  ptch.homeCnt = 0;
  ptch.awayCnt = 0;
  ptch.xSamp = 0;
  ptch.ySamp = 0;

  return *this;
}

// accessors and mutators
// sim info
double Pitch::getFramerate() const{
  return framerate;
}
void Pitch::setFramerate(double new_fr) {
  framerate = new_fr;
}

int Pitch::getFrame() const{
  return frame;
}
void Pitch::setFrame(int new_fm) {
  frame = new_fm;
}

double Pitch::getMatchTime() const{
  return frame / framerate;
}

// pitch dims
double Pitch::getPitchX() const{
  return pitchX;
}
double Pitch::getPitchY() const{
  return pitchY;
}

void Pitch::setPitchX(double new_pX) {
  pitchX = new_pX;
}
void Pitch::setPitchY(double new_pY) {
  pitchY = new_pY;
}
void Pitch::setPitchDims(double new_pX, double new_pY) {
  pitchX = new_pX;
  pitchY = new_pY;
}

void Pitch::storeFrame(Match& match) {
  // update frame
  frame = match.currentframe;
  ball = match.ball.getPos();

  for (int i{0}; i < homeCnt; i++){
    homePos[i] = match.homeTeam.getPos(i + 1);
  }
  for (int i{0}; i < awayCnt; i++){
    awayPos[i] = match.awayTeam.getPos(i + 1);
  }
}

void Pitch::computeControl() {
  homeCtrl = 0;
  awayCtrl = 0;
  for (int i{0}; i < homeCnt; i++) { homePlyrCtrl[i] = 0.0; }
  for (int i{0}; i < awayCnt; i++) { awayPlyrCtrl[i] = 0.0; }
  double temp_mindist; // current min dist
  double temp_dist;
  int temp_player; // +ve shirt number for home team, -ve shirt num for away team
  // sample the pitch for control
  for (int j{0}; j < ySamp; j++){ // iterate through y
    for (int i{0}; i < xSamp; i++){ // iterate through x
      Cart loc = convertIdx2Coords(i, j);
      // calculate the distance from the first homeplayer, for now he controls it
      temp_mindist = loc.dist2(homePos[0]);
      temp_player = 1; // index = shirtnum - 1;
      // iterate through rest of home team, look for closer player
      for (int num{1}; num < homeCnt; num++){
        temp_dist = loc.dist2(homePos[num]);
        if (temp_dist < temp_mindist){
          temp_mindist = temp_dist;
          temp_player = (num + 1);
        }
      }
      // iterate through away team
      for (int num{0}; num < awayCnt; num++){
        temp_dist = loc.dist2(awayPos[num]);
        if (temp_dist < temp_mindist){
          temp_mindist = temp_dist;
          temp_player = -1 * (num + 1); // -ve for away player
        }
      }
      // set corresponding control array element to
      control[i + xSamp * j] = temp_player;
      if (temp_player > 0){
        // home player
        homeCtrl += 1.0;
        homePlyrCtrl[temp_player - 1] += 1.0;

      } else if (temp_player < 0){
        // away player
        awayCtrl += 1.0;
        awayPlyrCtrl[(-1 * temp_player) - 1] += 1.0;
      }
    }
  }
  homeCtrl /= xSamp * ySamp;
  awayCtrl /= xSamp * ySamp;
  for (int i{0}; i < homeCnt; i++){
    homePlyrCtrl[i] /= xSamp * ySamp;
  }
  for (int i{0}; i < awayCnt; i++){
    awayPlyrCtrl[i] /= xSamp * ySamp;
  }
  //std::cout << "homeCtrl ptch " << homeCtrl << "\tawayCtrl " << awayCtrl << "\n";
}

// void Pitch::computeControl2(){
//   homeCtrl2 = 0;
//   awayCtrl2 = 0;
//   for (int i{0}; i < homeCnt; i++) { homePlyrCtrl2[i] = 0.0; }
//   for (int i{0}; i < awayCnt; i++) { awayPlyrCtrl2[i] = 0.0; }
//   // sample the pitch for control
//   for (int j{0}; j < ySamp; j++){ // iterate through y
//     for (int i{0}; i < xSamp; i++){ // iterate through x
      
//       Cart loc = convertIdx2Coords(i, j); // position on pitch
//       // distances
//       double hm_X = homePos[0].dist(loc);
//       double aw_X = awayPos[0].dist(loc);
//       // shirt numbers
//       int hm_num = 1;
//       int aw_num = 1;
//       // check if any other player is closer
//       for (int k{1}; k < homeCnt; k++){
//         double temp_X = homePos[k].dist(loc);
//         if (temp_X < hm_X){
//           // player is closer, update info
//           hm_X = temp_X;
//           hm_num = k + 1;
//         }
//       }
//       for (int k{1}; k < awayCnt; k++){
//         double temp_X = awayPos[k].dist(loc);
//         if (temp_X < aw_X){
//           aw_X = temp_X;
//           aw_num = k + 1;
//         }
//       }

//       points[i + j * xSamp] = Point(loc, hm_X, aw_X, 1.0);

//       homeCtrl2 += points[i + j * xSamp].getHomeArea();
//       awayCtrl2 += points[i + j * xSamp].getAwayArea();
//       //std::cout << "Home: " << points[i + j * xSamp].getHomeArea() << "\tAway: " << points[i + j * xSamp].getAwayArea() << "\n";

//       homePlyrCtrl2[hm_num - 1] += points[i + j * xSamp].getHomeArea();
//       awayPlyrCtrl2[aw_num - 1] += points[i + j * xSamp].getAwayArea();
//       //std::cout << "Home: " << homeCtrl2 << "\tAway: " << awayCtrl2 << "\n";
//       //std::cout << "Home: " << homePlyrCtrl2[hm_num - 1] << "\tAway: " << awayPlyrCtrl2[aw_num - 1] << "\n";

//     }
//   }

//   homeCtrl2 /= xSamp * ySamp;
//   awayCtrl2 /= xSamp * ySamp;
//   for (int i{0}; i < homeCnt; i++){
//     homePlyrCtrl2[i] /= xSamp * ySamp;
//   }
//   for (int i{0}; i < awayCnt; i++){
//     awayPlyrCtrl2[i] /= xSamp * ySamp;
//   }
// }

Cart Pitch::convertIdx2Coords(int i, int j){
  double X = ((i + 0.5) - xSamp/2) * pitchX / xSamp;
  double Y = ((j + 0.5) - ySamp/2) * pitchY / ySamp;
  return(Cart(X, Y));
}

double Pitch::getHomeCtrl() const{
  return homeCtrl;
}
double Pitch::getAwayCtrl() const{
  return awayCtrl;
}
// double Pitch::getHomeCtrl2() const{
//   return homeCtrl2;
// }
// double Pitch::getAwayCtrl2() const{
//   return awayCtrl2;
// }

Cart Pitch::getHomePos(int shirt_num) const{
  return homePos[shirt_num - 1];
}
Cart Pitch::getAwayPos(int shirt_num) const{
  return awayPos[shirt_num - 1];
}
Cart Pitch::getBallPos() const{
  return ball;
}
double Pitch::getHomePlyrCtrl(int shirt_num) const{
  return homePlyrCtrl[shirt_num - 1];
}
double Pitch::getAwayPlyrCtrl(int shirt_num) const{
  return awayPlyrCtrl[shirt_num - 1];
}
// double Pitch::getHomePlyrCtrl2(int shirt_num) const{
//   return homePlyrCtrl2[shirt_num - 1];
// }
// double Pitch::getAwayPlyrCtrl2(int shirt_num) const{
//   return awayPlyrCtrl2[shirt_num - 1];
// }
