#include "Match.h"
#include "vect/Cart.h"
#include<fstream>
#include "Player.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


// // default constructor
// Match::Match() : matchID(1), frames(100), currentframe(0), framerate(5.0), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0) {
//   pitches = std::make_unique<Pitch[]>(frames);
//   for (int i{0}; i < frames; i ++ ){
//     pitches[i] = Pitch(*this);
//   }
// }
// // parameterised constructors
// Match::Match(int frm) : matchID(1), frames(frm), currentframe(0), framerate(5.0), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
//   pitches = std::make_unique<Pitch[]>(frames);
//   for (int i{0}; i < frames; i ++ ){
//     pitches[i] = Pitch(*this);
//   }
// }
// Match::Match(int frm, double fmrt) : matchID(1), frames(frm), currentframe(0), framerate(fmrt), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
//   pitches = std::make_unique<Pitch[]>(frames);
//   for (int i{0}; i < frames; i ++ ){
//     pitches[i] = Pitch(*this);
//   }
// }
// Match::Match(int frm, int hm, int aw) : matchID(1), frames(frm), currentframe(0), framerate(5.0), pitchX(100.0), pitchY(100.0), homeTeam("Home", hm), awayTeam("Away", aw), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
//   pitches = std::make_unique<Pitch[]>(frames);
//   for (int i{0}; i < frames; i ++ ){
//     pitches[i] = Pitch(*this);
//   }
// }
// Match::Match(int frms, Team hmteam, Team awteam, double PitchX, double PitchY) : matchID(1), frames(frms), currentframe(0), framerate(5.0), pitchX(PitchX), pitchY(PitchY), homeTeam(hmteam), awayTeam(awteam), ball(), homeCtrl(0.0), awayCtrl(0.0) {
//   pitches = std::make_unique<Pitch[]>(frames);
//   for (int i{0}; i < frames; i ++ ){
//     pitches[i] = Pitch(*this);
//   }
// }

// default constructor
Match::Match() {}
// config constructor
Match::Match(MatchConfigFile mcf_) 
: frames(mcf_.getNumberOfFrames())
, framerate(mcf_.getFramerate())
, currentframe(0)
, pitchX(mcf_.getPitchX())
, pitchY(mcf_.getPitchY()) 
{
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}
Match::Match(MatchConfigFile mcf_, TeamConfigFile home_tcf, TeamConfigFile away_tcf) 
: frames(mcf_.getNumberOfFrames())
, framerate(mcf_.getFramerate())
, currentframe(0)
, pitchX(mcf_.getPitchX())
, pitchY(mcf_.getPitchY()) 
, homeTeam(home_tcf)
, awayTeam(away_tcf)
{
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}

// copy constructor
Match::Match(Match& match) : matchID(match.matchID), frames(match.frames), framerate(match.framerate), currentframe(match.currentframe), pitchX(match.pitchX), pitchY(match.pitchY), homeTeam(match.homeTeam), awayTeam(match.awayTeam), ball(match.ball), homeCtrl(match.homeCtrl), awayCtrl(match.awayCtrl) {
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i++){
    pitches[i] = match.pitches[i];
  }
}
// copy assignment
Match& Match::operator=(Match& match){
  if (&match == this){
    return *this;
  }

  matchID = match.matchID;
  frames = match.frames;
  framerate = match.framerate;
  currentframe = match.currentframe;
  pitchX = match.pitchX;
  pitchY = match.pitchY;
  homeTeam = match.homeTeam;
  awayTeam = match.awayTeam;
  ball = match.ball;
  homeCtrl = match.homeCtrl;
  awayCtrl = match.awayCtrl;
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i++){
    pitches[i] = match.pitches[i];
  }
  return *this;
}
// move assignment
Match& Match::operator=(Match&& match){
  if (&match == this){
    return *this;
  }

  matchID = match.matchID;
  frames = match.frames;
  framerate = match.framerate;
  currentframe = match.currentframe;
  pitchX = match.pitchX;
  pitchY = match.pitchY;
  homeTeam = match.homeTeam;
  awayTeam = match.awayTeam;
  ball = match.ball;
  homeCtrl = match.homeCtrl;
  awayCtrl = match.awayCtrl;
  pitches = std::move(match.pitches);
  match.frames = 0;

  return *this;
}

// sim info
double Match::getFramerate() const{
  return framerate;
}
int Match::getFrames() const{
  return frames;
}
int Match::getCurrentFrame() const{
  return currentframe;
}
void Match::setMatchID(int i) {
  matchID = i;
}

// pitch info
double Match::getPitchX() const{
  return pitchX;
}
double Match::getPitchY() const{
  return pitchY;
}
Pitch& Match::getCurrentPitch() const{
  return pitches[currentframe];
}

// player info
int Match::getHomeCnt() const{
  return homeTeam.getPlayerCount();
}
int Match::getAwayCnt() const{
  return awayTeam.getPlayerCount();
}
int Match::getPlayerCnt() const{
  return homeTeam.getPlayerCount() + awayTeam.getPlayerCount();
}

void Match::printPlayers() const {
  std::cout << "Frame: " << currentframe << "\n";
  homeTeam.printPlayers();

  awayTeam.printPlayers();
}

void Match::printPitch() const {
  std::cout << "Frame: " << currentframe << "\n";
  std::cout << "\tBall: (" << ball.getPosX() << ", " << ball.getPosY() << ")\n";
  homeTeam.printPlayers();

  awayTeam.printPlayers();

}

// initialise pitch
void Match::initRandObjPos() {

  homeTeam.initRandObjPos(*this);
  awayTeam.initRandObjPos(*this);
  checkCollisions();
  setHomePossession(true);
  // toggle with 50% chance
  int flip = (rand()%2)+ 1 ;
  if (flip == 1) toggleHomePossession();

  pitches[0].storeFrame(*this);
}

void Match::initUnitPolygon() {
  homeTeam.initCentre(*this);
  awayTeam.initUnitPolygon(*this);
  pitches[0].storeFrame(*this);
}

void Match::updateFrame(){
  // toggle possession with 2% chance
  int flip = (rand()%50)+ 1 ;
  if (flip == 1) toggleHomePossession();
  // update teams
  homeTeam.updateFrame(*this);
  awayTeam.updateFrame(*this);
  // check for collisions
  //checkCollisions();
  // update frame counter
  currentframe++;
  // store new positions for later
  pitches[currentframe].storeFrame(*this);

}

void Match::checkCollisions(){
  bool no_col = false;

  do{
    // set no_col to true, if contradiction is found it is changed
    no_col = true;
    // iterate through home team
    for (int i{0}; i < homeTeam.getPlayerCount(); i++){
      auto iPos = homeTeam.getPos(i + 1);
      // for current player, iterate though rest of home team
      for (int j{i + 1}; j < homeTeam.getPlayerCount(); j++){
        auto jPos = homeTeam.getPos(j + 1);
        // check distance from two points
        if (iPos.dist2(jPos) == 0.0){
          // collision detected
          no_col = false;
          // scatter players
          double radius = 0.1;
          double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
          homeTeam.scatterPlayer(i + 1, radius, theta);
          homeTeam.scatterPlayer(j + 1, radius, M_PI + theta);
        }
      }
      // then away team
      for (int j{0}; j < awayTeam.getPlayerCount(); j++){
        auto jPos = awayTeam.getPos(j + 1);
        // check distance from two points
        if (iPos.dist2(jPos) == 0.0){
          // collision detected
          no_col = false;
          // scatter players
          double radius = 0.1;
          double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
          homeTeam.scatterPlayer(i + 1, radius, theta);
          awayTeam.scatterPlayer(j + 1, radius, M_PI + theta);

        }
      }
    }
    // once home team is checked, check none of the away players collide with each other
    for (int i{0}; i < awayTeam.getPlayerCount() - 1; i++){ // -1 because very last player doesn't need a check
      auto iPos = awayTeam.getPos(i + 1);
      // check against rest of players on away team
      for (int j{i + 1}; j < awayTeam.getPlayerCount(); j++){
        auto jPos = awayTeam.getPos(j + 1);

        if (iPos.dist2(jPos) == 0.0){
          // collision detected
          no_col = false;
          // scatter players
          double radius = 0.1;
          double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
          awayTeam.scatterPlayer(i + 1, radius, theta);
          awayTeam.scatterPlayer(j + 1, radius, M_PI + theta);

        }
      }
    }
    if (!no_col){
      std::cout << "COLLISION\n";
    }
  }
  while(!no_col);

}

void Match::checkCollisions(Player& iPlyr){
  bool no_col = false;

  do{
    // set no_col to true, if contradiction is found it is changed
    no_col = true;
    // iterate through home team
    auto iPos = iPlyr.getPos();
    // for current player, iterate though rest of home team
    for (int j{0}; j < homeTeam.getPlayerCount(); j++){
      auto jPos = homeTeam.getPos(j + 1);
      // check distance from two points
      if (iPos.dist2(jPos) == 0.0 && &homeTeam.getPlayerR(j + 1) != &iPlyr){
        // collision detected
        no_col = false;
        // scatter players
        double radius = 0.1;
        double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
        iPlyr.scatter(radius, theta);
        homeTeam.scatterPlayer(j + 1, radius, M_PI + theta);
      }
    }
    // then away team
    for (int j{0}; j < awayTeam.getPlayerCount(); j++){
      auto jPos = awayTeam.getPos(j + 1);
      // check distance from two points
      if (iPos.dist2(jPos) == 0.0 && &awayTeam.getPlayerR(j + 1) != &iPlyr){
        // collision detected
        no_col = false;
        // scatter players
        double radius = 0.1;
        double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
        iPlyr.scatter(radius, theta);
        awayTeam.scatterPlayer(j + 1, radius, M_PI + theta);

        }
      }

    // if (!no_col){
    //   //std::cout << "COLLISION\n";
    // }
  }
  while(!no_col);

}
// teams
Team& Match::getHomeTeam(){
  return homeTeam;
}
Team& Match::getAwayTeam(){
  return awayTeam;
}

// sim
void Match::startSimulation(bool calc_space){
  homeCtrl = 0.0;
  awayCtrl = 0.0;
  homeCtrl2 = 0.0;
  awayCtrl2 = 0.0;
  if (calc_space){
    pitches[0].computeControl();
    homeCtrl += pitches[0].getHomeCtrl();
    awayCtrl += pitches[0].getAwayCtrl();
    // pitches[currentframe].computeControl2();
    // homeCtrl2 += pitches[currentframe].getHomeCtrl2();
    // awayCtrl2 += pitches[currentframe].getAwayCtrl2();
  }


  for(int i{1}; i < frames; i++){
    // update frame
    updateFrame();
    if (calc_space){
      pitches[currentframe].computeControl();
      homeCtrl += pitches[currentframe].getHomeCtrl();
      awayCtrl += pitches[currentframe].getAwayCtrl();
      // pitches[currentframe].computeControl2();
      // homeCtrl2 += pitches[currentframe].getHomeCtrl2();
      // awayCtrl2 += pitches[currentframe].getAwayCtrl2();
    }
    if (i % 500 == 0 || i + 1 == frames) {
      printPlayers();
      if (calc_space){
        std::cout << "\nHomeCtrl: " << avgHomeCtrl() << "\n";
        std::cout << "AwayCtrl: " << avgAwayCtrl() << "\n";
        // std::cout << "\nHomeCtrl2: " << avgHomeCtrl2() << "\n";
        // std::cout << "AwayCtrl2: " << avgAwayCtrl2() << "\n";
      }
    }
  }
}
// control
double Match::avgHomeCtrl() const{
  //std::cout << "\nHomeCTRL " << homeCtrl << "\n";
  return homeCtrl / ((double) currentframe + 1);
}
double Match::avgAwayCtrl() const{

  //std::cout << "AwayCTRL " << homeCtrl << "\n\n";
  return awayCtrl / ((double) currentframe + 1);
}
double Match::avgHomeCtrl2() const{
  //std::cout << "\nHomeCTRL " << homeCtrl2 << "\n";
  return homeCtrl2 / ((double) currentframe + 1);
}
double Match::avgAwayCtrl2() const{

  //std::cout << "AwayCTRL " << awayCtrl2 << "\n\n";
  return awayCtrl2 / ((double) currentframe + 1);
}

void Match::saveMatchToFile(std::string file_name, int i, bool legacy) const {
  std::cout << "\nSaving Match " << matchID << " to data_files/csvs/" + file_name + "/" + std::to_string(i) + ".csv\n\n";
  struct stat st;
  if(!stat(("data_files/csvs/" + file_name).c_str(), &st) == 0)
    // directory doesn't exist, make it
    system(("mkdir data_files/csvs/" + file_name).c_str());
  // open file
  std::ofstream datafile;
  if (legacy){
    datafile.open("data_files/legacy_csvs/" + file_name + "/"  + std::to_string(i) + ".csv");
  } else {
    datafile.open("data_files/csvs/" + file_name + "/" + std::to_string(i) + ".csv");
  }
  datafile << "idx\tPID\tTime\tFID\tTeam\tNum\tX\tY";
  if (legacy) {
    datafile << "\n";
  } else {
    datafile << "\tdX\tdY\tCtrl\tdCtrl\tTmCtrl\tTmdCtrl\tPossession\tSmart\n"; // header
  }
  int index{0}; // index counter must increment with every row
  Cart temp_pos;
  Cart last_pos;
  int PID;
  int last_frame;
  for (int frame_{0}; frame_ < frames; frame_++){
    if (frame_ == 0){
      last_frame = 0;
    } else {
      last_frame = frame_ - 1;
    }

    // check PID
    if (pitches[frame_].getMatchTime() >= 2700){ // 2700 seconds in a half
      PID = 2; // second half of a match
    } else {
      PID = 1; // first half of a match
    }
    // ball
    temp_pos = pitches[frame_].getBallPos();
    last_pos = pitches[last_frame].getBallPos();
    datafile
      << index << "\t"
      << PID << "\t"
      << pitches[frame_].getMatchTime() << "\t"
      << frame_ << "\t"
      << "Ball\t"
      << 0 << "\t"
      << temp_pos.xComp() << "\t"
      << temp_pos.yComp();
      // check whether compatibility with Summer-2018 preprocessors is required
    if (legacy){
      // need legacy format, stop here
      datafile << "\n";
    } else {
      // legacy not required, add extra information
      datafile << "\t"
        << temp_pos.xComp() - last_pos.xComp() << "\t"
        << temp_pos.yComp() - last_pos.yComp() << "\t"
        << 0.0 << "\t"
        << 0.0 << "\t"
        // << 0.0 << "\t"
        // << 0.0 << "\t"
        << 0.0 << "\t"
        << 0.0 << "\t"
        << "False" << "\t"
        << "False" << "\n";
    }

    index++; // increment index

    // homeplayers
    double hmCtrl = pitches[frame_].getHomeCtrl();
    double awCtrl = pitches[frame_].getAwayCtrl();
    double dHmCtrl = hmCtrl - pitches[last_frame].getHomeCtrl();
    double dAwCtrl = awCtrl - pitches[last_frame].getAwayCtrl();

    for (int i{0}; i < homeTeam.getPlayerCount(); i++){

      auto pl = homeTeam.getPlayer(i + 1);
      temp_pos = pitches[frame_].getHomePos(i + 1); // get player pos
      last_pos = pitches[last_frame].getHomePos(i + 1); // get last pos
      // write to file
      datafile
        << index << "\t"
        << PID << "\t"
        << pitches[frame_].getMatchTime() << "\t"
        << frame_ << "\t"
        << "Home\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp();
      if (legacy) {
        datafile << "\n";
      } else {
        datafile << "\t"
          << temp_pos.xComp() - last_pos.xComp() << "\t"
          << temp_pos.yComp() - last_pos.yComp() << "\t"
          << pitches[frame_].getHomePlyrCtrl(i + 1) << "\t"
          << pitches[frame_].getHomePlyrCtrl(i + 1) - pitches[last_frame].getHomePlyrCtrl(i + 1) << "\t"
          // << pitches[frame_].getHomePlyrCtrl2(i + 1) << "\t"
          // << pitches[frame_].getHomePlyrCtrl2(i + 1) - pitches[last_frame].getHomePlyrCtrl2(i + 1) << "\t"
          << hmCtrl << "\t"
          << dHmCtrl << "\t"
          << pitches[frame_].getHomePossStr() << "\t"
          << pl.getSmartStr() << "\n";
      }

      index++; // increment index
    }
    // awayplayers
    for (int i{0}; i < awayTeam.getPlayerCount(); i++){
      auto pl = awayTeam.getPlayer(i + 1);
      temp_pos = pitches[frame_].getAwayPos(i + 1);
      last_pos = pitches[last_frame].getAwayPos(i + 1);
      datafile
        << index << "\t"
        << PID << "\t"
        <<  pitches[frame_].getMatchTime() << "\t"
        << frame_ << "\t"
        << "Away\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp();
      if (legacy) {
        datafile << "\n";
      } else {
        datafile << "\t"
          << temp_pos.xComp() - last_pos.xComp() << "\t"
          << temp_pos.yComp() - last_pos.yComp() << "\t"
          << pitches[frame_].getAwayPlyrCtrl(i + 1) << "\t"
          << pitches[frame_].getAwayPlyrCtrl(i + 1) - pitches[last_frame].getAwayPlyrCtrl(i + 1) << "\t"
          // << pitches[frame_].getAwayPlyrCtrl2(i + 1) << "\t"
          // << pitches[frame_].getAwayPlyrCtrl2(i + 1) - pitches[last_frame].getAwayPlyrCtrl2(i + 1) << "\t"
          << awCtrl << "\t"
          << dAwCtrl << "\t"
          << pitches[frame_].getAwayPossStr() << "\t"
          << pl.getSmartStr() << "\n";
      }

      index++;
    }
  }
  datafile.close();
  // if legacy is required, a corresponding mdata file must also be produced
  if (legacy) {
    std::ofstream mdata;
    mdata.open("data_files/legacy_csvs/" + file_name + ".mdata");
    // mdata has frames, pitch X, pitchY, homeID, awayID
    mdata << frames << "\t"
      << pitchX << "\t"
      << pitchY << "\t"
      << 1 << "\t" // arbitrary home and away ID
      << 2 << "\n";

  }

}

void Match::setHomePossession(bool hp_) { homePossession = hp_; homeTeam.setPossession(hp_); awayTeam.setPossession(!hp_); }
bool Match::getHomePossession() const { return homePossession; }
bool Match::getAwayPossession() const { return !homePossession; }
void Match::toggleHomePossession() { homePossession = !homePossession; homeTeam.togglePossession(); awayTeam.togglePossession();}
