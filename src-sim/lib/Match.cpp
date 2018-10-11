#include "Match.h"
#include "vect/Cart.h"
#include<fstream>
#include "Player.h"

// default constructor
Match::Match() : matchID(1), frames(100), currentframe(0), framerate(5.0), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0) {
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}
// parameterised constructors
Match::Match(int frm) : matchID(1), frames(frm), currentframe(0), framerate(5.0), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}
Match::Match(int frm, double fmrt) : matchID(1), frames(frm), currentframe(0), framerate(fmrt), pitchX(105.0), pitchY(68.0), homeTeam("Home", 11), awayTeam("Away", 11), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}
Match::Match(int frm, int hm, int aw) : matchID(1), frames(frm), currentframe(0), framerate(5.0), pitchX(100.0), pitchY(100.0), homeTeam("Home", hm), awayTeam("Away", aw), ball(), homeCtrl(0.0), awayCtrl(0.0)  {
  pitches = std::make_unique<Pitch[]>(frames);
  for (int i{0}; i < frames; i ++ ){
    pitches[i] = Pitch(*this);
  }
}
Match::Match(int frms, Team hmteam, Team awteam, double PitchX, double PitchY) : matchID(1), frames(frms), currentframe(0), framerate(5.0), pitchX(PitchX), pitchY(PitchY), homeTeam(hmteam), awayTeam(awteam), ball(), homeCtrl(0.0), awayCtrl(0.0) {
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
  pitches[0].storeFrame(*this);
}

void Match::initUnitPolygon() {
  homeTeam.initCentre(*this);
  awayTeam.initUnitPolygon(*this);
  pitches[0].storeFrame(*this);
}

void Match::updateFrame(){

  // update teams
  homeTeam.updateFrame(*this);
  awayTeam.updateFrame(*this);
  // update frame counter
  currentframe++;
  // store new positions for later
  pitches[currentframe].storeFrame(*this);

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
  if (calc_space){
    pitches[0].computeControl();
    homeCtrl += pitches[0].getHomeCtrl();
    awayCtrl += pitches[0].getAwayCtrl();
  }


  for(int i{1}; i < frames; i++){
    // update frame
    updateFrame();
    if (calc_space){
      pitches[currentframe].computeControl();
      homeCtrl += pitches[currentframe].getHomeCtrl();
      awayCtrl += pitches[currentframe].getAwayCtrl();
    }
    if (i % 500 == 0 || i + 1 == frames) {
      printPlayers();
      if (calc_space){
        std::cout << "\nHomeCtrl: " << avgHomeCtrl() << "\n";
        std::cout << "AwayCtrl: " << avgAwayCtrl() << "\n";
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

void Match::saveMatchToFile(std::string file_name) const {
  std::cout << "\nSaving Match " << matchID << " to data_files/csvs/" + file_name + ".csv\n\n";
  // open file
  std::ofstream datafile;
  datafile.open("data_files/csvs/" + file_name + ".csv");
  datafile << "idx\tPID\tTime\tFID\tTeam\tNum\tX\tY\tCtrl\tdCtrl\tSmart\n"; // header
  int index{0}; // index counter must increment with every row
  Cart temp_pos;
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
    datafile
      << index << "\t"
      << PID << "\t"
      << pitches[frame_].getMatchTime() << "\t"
      << frame_ << "\t"
      << "Ball\t"
      << 0 << "\t"
      << temp_pos.xComp() << "\t"
      << temp_pos.yComp() << "\t"
      << 0.0 << "\t"
      << 0.0 << "\t"
      << "False" << "\n";

    index++; // increment index

    // homeplayers
    for (int i{0}; i < homeTeam.getPlayerCount(); i++){

      auto pl = homeTeam.getPlayer(i + 1);
      temp_pos = pitches[frame_].getHomePos(i + 1); // get player pos
      // write to file
      datafile
        << index << "\t"
        << PID << "\t"
        << pitches[frame_].getMatchTime() << "\t"
        << frame_ << "\t"
        << "Home\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp() << "\t"
        << pitches[frame_].getHomePlyrCtrl(i + 1) << "\t"
        << pitches[frame_].getHomePlyrCtrl(i + 1) - pitches[last_frame].getHomePlyrCtrl(i + 1) << "\t"
        << pl.getSmartStr() << "\n";

      index++; // increment index
    }
    // awayplayers
    for (int i{0}; i < awayTeam.getPlayerCount(); i++){
      auto pl = awayTeam.getPlayer(i + 1);
      temp_pos = pitches[frame_].getAwayPos(i + 1);
      datafile
        << index << "\t"
        << PID << "\t"
        <<  pitches[frame_].getMatchTime() << "\t"
        << frame_ << "\t"
        << "Away\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp() << "\t"
        << pitches[frame_].getAwayPlyrCtrl(i + 1) << "\t"
        << pitches[frame_].getAwayPlyrCtrl(i + 1) - pitches[last_frame].getAwayPlyrCtrl(i + 1) << "\t"
        << pl.getSmartStr() << "\n";

      index++;
    }
  }
  datafile.close();

}
