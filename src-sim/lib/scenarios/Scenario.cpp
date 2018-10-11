#include "Scenario.h"

Scenario::Scenario() : filename("testing"), homeCnt(11), awayCnt(11), frames(100), pitchX(105.0), pitchY(68.0), hNormAI(), hSmartAI(), aAI() {

  homeTeam = Team("Home", 11, hNormAI, 1, hSmartAI);
  awayTeam= Team("Away", 11, aAI);

}

Scenario::Scenario(std::string fn) : filename(fn), homeCnt(11), awayCnt(11), frames(100), pitchX(105.0), pitchY(68.0), hNormAI(), hSmartAI(), aAI() {

  homeTeam = Team("Home", 11, hNormAI, 1, hSmartAI);
  awayTeam= Team("Away", 11, aAI);
  setLegacy(false);

}
Scenario::Scenario(int frms, std::string fn) : filename(fn), homeCnt(11), awayCnt(11), frames(frms), pitchX(105.0), pitchY(68.0), hNormAI(), hSmartAI(), aAI() {

  homeTeam = Team("Home", 11, hNormAI, 1, hSmartAI);
  awayTeam= Team("Away", 11, aAI);
  setLegacy(false);

}
Scenario::Scenario(int frms, std::string fn, bool leg) : filename(fn), homeCnt(11), awayCnt(11), frames(frms), pitchX(105.0), pitchY(68.0), hNormAI(), hSmartAI(), aAI() {

  homeTeam = Team("Home", 11, hNormAI, 1, hSmartAI);
  awayTeam= Team("Away", 11, aAI);
  setLegacy(leg);

}

void Scenario::setFilename(std::string fn) {
  filename = fn;
}
std::string Scenario::getFilename() const {
  return filename;
}

void Scenario::setHomeCnt(int hm) {
  homeCnt = hm;
}
int Scenario::getHomeCnt() const {
  return homeCnt;
}
void Scenario::setAwayCnt(int aw) {
  awayCnt = aw;
}
int Scenario::getAwayCnt() const {
  return awayCnt;
}

void Scenario::setFrames(int frms) {
  frames = frms;
}
int Scenario::getFrames() const {
  return frames;
}

void Scenario::setPitchDims(double X, double Y){
  pitchX = X;
  pitchY = Y;
}
double Scenario::getPitchX() const {
  return pitchX;
}
double Scenario::getPitchY() const {
  return pitchY;
}

void Scenario::setHomeTeam(Team hmtm) {
  homeTeam = hmtm;
}
Team& Scenario::getHomeTeam() {
  return homeTeam;
}
void Scenario::setAwayTeam(Team awtm) {
  awayTeam = awtm;
}
Team& Scenario::getAwayTeam() {
  return awayTeam;
}
void Scenario::setLegacy(bool leg){
  legacy = leg;
}
bool Scenario::getLegacy() const {
  return legacy;
}

Match Scenario::buildMatch() {
  Match match = Match(frames, homeTeam, awayTeam, pitchX, pitchY);
  std::cout << "here";
  match.initRandObjPos();
  return match;
}

void Scenario::start(int samples, bool leg) {
  start(samples, 0, leg);
}

void Scenario::start(int samples, int init_num, bool leg) {
  // get starting time
  auto t_start = std::chrono::high_resolution_clock::now();
  // run samples
  for (int i{init_num}; i < samples + init_num; i++){
    std::cout << "Sample " << i << "\n\n";
    // start match
    Match match = buildMatch();
    std::cout << "build\n";
    match.printPlayers();
    std::cout << "print\n";
    match.startSimulation(!leg);
    match.saveMatchToFile(getFilename() + "_" + std::to_string(i), leg);
  }
  // finish time
  auto t_fin = std::chrono::high_resolution_clock::now();
  std::cout << "\nTime elapsed:\n"
    << std::chrono::duration_cast<std::chrono::seconds>(t_fin - t_start).count()
    << " seconds for " << samples << " samples\n";

}
