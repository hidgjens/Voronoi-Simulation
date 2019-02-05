#include "Match.h"
#include "Team.h"
#include<fstream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Player.h"

Match::Match() {}

Match::Match(TeamConfigFile ht, TeamConfigFile at, MatchConfigFile m) :
mcf(m),
home_team_control_sum(0),
away_team_control_sum(0)
{
    store_frames = mcf.full_save;
    x_samples = mcf.sample_x;
    y_samples = mcf.sample_y;
    possession_flip_reciprocal_probability = mcf.possession_chance;
    pitch_data = Pitch(mcf.pitchX, mcf.pitchY);
    home_team = new Team(ht, true, pitch_data);
    away_team = new Team(at, false, pitch_data);
    weight_model = PitchModel::createPitchModel(mcf.pitchMdl, mcf);
    filename = mcf.save_filename;
    total_frames = mcf.numberOfFrames; 
    if (store_frames)
        frame_history = std::make_unique<Frame[]>(total_frames);
    
}

Match::Match(Match& m) :
store_frames(m.store_frames),
total_frames(m.total_frames),
x_samples(m.x_samples),
y_samples(m.y_samples),
pitch_data(m.pitch_data),
weight_model(m.weight_model),
filename(m.filename),
mcf(m.mcf),
home_team(m.home_team),
away_team(m.away_team),
current_frame_number(m.current_frame_number),
current_frame(m.current_frame),
home_team_control_sum(m.home_team_control_sum),
away_team_control_sum(m.away_team_control_sum),
home_possession(m.home_possession),
possession_flip_reciprocal_probability(m.possession_flip_reciprocal_probability)
{
    if (store_frames){
        frame_history = std::make_unique<Frame[]>(total_frames);
        for (int i{0}; i < total_frames; i++) {
            frame_history[i] = m.frame_history[i];
        }
    }
}

Match& Match::operator=(Match& m) {
    if (this == &m) {
        return *this;
    }

    store_frames = m.store_frames;
    total_frames = m.total_frames;
    possession_flip_reciprocal_probability = m.possession_flip_reciprocal_probability;
    x_samples = m.x_samples;
    y_samples = m.y_samples;
    pitch_data = m.pitch_data;
    weight_model = m.weight_model;
    filename = m.filename;
    mcf = m.mcf;
    home_team = m.home_team;
    away_team = m.away_team;
    current_frame_number = m.current_frame_number;
    current_frame = m.current_frame;
    home_team_control_sum = m.home_team_control_sum;
    away_team_control_sum = m.away_team_control_sum;
    home_possession = m.home_possession;

    if (store_frames){
        frame_history = std::make_unique<Frame[]>(total_frames);
        for (int i{0}; i < total_frames; i++) {
            frame_history[i] = m.frame_history[i];
        }
    }

    return *this;
}

Match& Match::operator=(Match&& m) {
    store_frames = m.store_frames;
    total_frames = m.total_frames;
    possession_flip_reciprocal_probability = m.possession_flip_reciprocal_probability;
    x_samples = m.x_samples;
    y_samples = m.y_samples;
    pitch_data = m.pitch_data; 
    weight_model = m.weight_model;
    filename = m.filename;
    mcf = m.mcf;
    home_team = m.home_team;
    away_team = m.away_team;
    current_frame_number = m.current_frame_number;
    current_frame = m.current_frame;
    home_team_control_sum = m.home_team_control_sum;
    away_team_control_sum = m.away_team_control_sum;
    home_possession = m.home_possession;

    if (store_frames) {
        frame_history = std::move(m.frame_history);
    }

    return *this;
}

Pitch& Match::getPitchData() { return pitch_data; }

void Match::buildMatch() {
    // determine initial possession
    randomPossession();

    // just need to store the first frame
    calculate_control();
    current_frame = createFrame();
    if (store_frames) {
        frame_history[0] = current_frame;
    }
}

void Match::startSimulation() {
    while (current_frame_number < total_frames) {
        std::cout<<"tick!\n";
        tick();
    }
    
    if (store_frames) saveFullMatch();
    saveMatchSummary(); 
}

void Match::saveMatchSummary(){
    // Announce
  std::cout << "\nSaving Match " << matchID << " to data_files/csv/" + filename + "/" + std::to_string(matchID) + ".csv\n\n";

  // check if folder exists
  struct stat st;
  if(!stat(("data_files/csvs/" + filename).c_str(), &st) == 0){
    // directory doesn't exist, make the folder
    system(("mkdir data_files/csvs/" + filename).c_str());
  }
  // open file
  std::ofstream datafile;
  // no legacy support obviously
  datafile.open("data_files/csvs/" + filename + "/" + std::to_string(matchID) + ".sum");

  /*
    File formatting:
    HOME_AVG\tPLAYER1\tPLAYER2\t...PLAYER11\n
    AWAY_AVG\tPLAYER1\t....PLAYER11\n

    ^^ scratch this too lazy for now

  */
  datafile << avgHomeControl() << "\n" << avgAwayControl() << "\n";
  // for (int i{0}; i < homeTeam.getPlayerCount(); i++){
  //   datafile << "\t" << homeTeam.getPlyrCtrl
  // }
  datafile.close();

}

void Match::saveFullMatch() {
  std::cout << "\nSaving Match " << matchID << " to data_files/csvs/" + filename + "/" + std::to_string(matchID) + ".csv\n\n";
  struct stat st;
  if(!stat(("data_files/csvs/" + filename).c_str(), &st) == 0)
    // directory doesn't exist, make it
    system(("mkdir data_files/csvs/" + filename).c_str());
  // open file
  std::ofstream datafile;

  datafile.open("data_files/csvs/" + filename + "/" + std::to_string(matchID) + ".csv");
  
  datafile << "idx\tFID\tTeam\tNum\tX\tY";

  datafile << "\tdX\tdY\tCtrl\tdCtrl\tTmCtrl\tTmdCtrl\tPossession\n"; // header
 
  int index{0}; // index counter must increment with every row
  Cart temp_pos;
  Cart last_pos;

  int last_frame;
  for (int frame_{0}; frame_ < total_frames; frame_++){
    if (frame_ == 0){
      last_frame = 0;
    } else {
      last_frame = frame_ - 1;
    }

    // homeplayers
    double hmCtrl = frame_history[frame_].getHomeTeamControl();
    double awCtrl = frame_history[frame_].getAwayTeamControl();
    double dHmCtrl = hmCtrl - frame_history[last_frame].getHomeTeamControl();
    double dAwCtrl = awCtrl - frame_history[last_frame].getAwayTeamControl();

    for (int i{0}; i < home_team->getPlayerCount(); i++){

      auto pl = home_team->getPlayer(i);
      temp_pos = frame_history[frame_].getHomePosition(i); // get player pos
      last_pos = frame_history[last_frame].getHomePosition(i); // get last pos
      // write to file
      datafile
        << index << "\t"
        << frame_ << "\t"
        << "Home\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp() << "\t"
        << temp_pos.xComp() - last_pos.xComp() << "\t"
        << temp_pos.yComp() - last_pos.yComp() << "\t"
        << frame_history[frame_].getHomeControl(i) << "\t"
        << frame_history[frame_].getHomeControl(i) - frame_history[last_frame].getHomeControl(i) << "\t"
        // << pitches[frame_].getHomePlyrCtrl2(i + 1) << "\t"
        // << pitches[frame_].getHomePlyrCtrl2(i + 1) - pitches[last_frame].getHomePlyrCtrl2(i + 1) << "\t"
        << hmCtrl << "\t"
        << dHmCtrl << "\t"
        << frame_history[frame_].getHomePossStr() << "\n";
      }

      index++; // increment index
    
    // awayplayers
    for (int i{0}; i < away_team->getPlayerCount(); i++){

      auto pl = away_team->getPlayer(i);
      temp_pos = frame_history[frame_].getAwayPosition(i); // get player pos
      last_pos = frame_history[last_frame].getAwayPosition(i); // get last pos
      // write to file
      datafile
        << index << "\t"
        << frame_ << "\t"
        << "Away\t"
        << i + 1 << "\t"
        << temp_pos.xComp() << "\t"
        << temp_pos.yComp() << "\t"
        << temp_pos.xComp() - last_pos.xComp() << "\t"
        << temp_pos.yComp() - last_pos.yComp() << "\t"
        << frame_history[frame_].getAwayControl(i) << "\t"
        << frame_history[frame_].getAwayControl(i) - frame_history[last_frame].getAwayControl(i) << "\t"
        // << pitches[frame_].getHomePlyrCtrl2(i + 1) << "\t"
        // << pitches[frame_].getHomePlyrCtrl2(i + 1) - pitches[last_frame].getHomePlyrCtrl2(i + 1) << "\t"
        << hmCtrl << "\t"
        << dHmCtrl << "\t"
        << frame_history[frame_].getAwayPossStr() << "\n";
      }

      index++; // increment index
    }
  
  datafile.close();

}

void Match::calculate_control() {
  std::cout << "calc_ctrl\n";
  double weighting;
  std::cout << "this";
  double dC;
  std::cout << "that";
  double total_ctrl = 0.0;

  std::cout << "try";
  double hm_count = home_team->getPlayerCount();
  std::cout << "madeit";

  for (int i{0}; i < home_team->getPlayerCount(); i++){
      std::cout << "segging here i bet";
      auto plyr = home_team->getPlayer(i);
      plyr.current_control = 0;
  }
  for (int i{0}; i < away_team->getPlayerCount(); i++){
      auto plyr = away_team->getPlayer(i);
      plyr.current_control = 0;
  }

  double temp_mindist; // current min dist
  double temp_dist;
  Cart temp_pos;
  int temp_player; // IDX OFFSET BY 1 to avoid problems at 0, +ve shirt number for home team, -ve shirt num for away team
  // sample the pitch for control
  for (int j{0}; j < y_samples; j++){ // iterate through y
    for (int i{0}; i < x_samples; i++){ // iterate through x

      Cart loc = convertIdx2Coords(i, j);
      // calculate the distance from the first homeplayer, for now he controls it
      temp_pos = home_team->getPlayer(0).getPosition();
      temp_mindist = loc.dist2(temp_pos); // sponge
      temp_player  = 1; // index + 1; // sponge
      // iterate through rest of home team, look for closer player
      for (int num{1}; num < home_team->getPlayerCount(); num++){
        temp_pos = current_frame.getHomePosition(num);
        temp_dist = loc.dist2(temp_pos);
        if (temp_dist < temp_mindist){
          temp_mindist = temp_dist;
          temp_player = (num + 1);
        }
      }
      // iterate through away team
      for (int num{0}; num < away_team->getPlayerCount(); num++){
        temp_pos = current_frame.getAwayPosition(0);
        temp_dist = loc.dist2(temp_pos);
        // first check if it's the closest away player

        if (temp_dist < temp_mindist){
          temp_mindist = temp_dist;
          temp_player = -1 * (num + 1); // -ve for away player
        }

      }

      if (temp_player > 0){
        // home player
        dC = 1.0 * weight_model->pitchWeight(loc, current_frame);
        auto plyr = home_team->getPlayer(temp_player - 1);
        plyr.current_control += dC;
        
        total_ctrl += dC;

      } else if (temp_player < 0){
        // away player
        dC = 1.0 * weight_model->pitchWeight(loc, current_frame);
        auto plyr = away_team->getPlayer(abs(temp_player) - 1);
        plyr.current_control += dC;

        total_ctrl += dC;
      }
    }
  }
  for (int i{0}; i < home_team->getPlayerCount(); i++){
      auto plyr = home_team->getPlayer(i);
      plyr.current_control /= total_ctrl;
  }
  for (int i{0}; i < away_team->getPlayerCount(); i++){
      auto plyr = away_team->getPlayer(i);
      plyr.current_control /= total_ctrl;
  }
}

void Match::tick() {
    // see if possession changes
    std::cout<<"fliP,";
    tryPossessionFlip();
    // increase frame counter

    current_frame_number++;
    // tell teams to update, give them current frame as info
    std::cout<<"update,";

    home_team->update(current_frame);
    away_team->update(current_frame);
    // recalculate control
    std::cout<<"ctrl,";

    calculate_control();
    // store new frame
    current_frame = createFrame();
    // update sum
    home_team_control_sum += current_frame.getHomeTeamControl();
    away_team_control_sum += current_frame.getAwayTeamControl();
    // if frames are stored, store it
    if (store_frames) frame_history[current_frame_number] = current_frame;

    std::cout<<"done;\n";

}

Frame Match::createFrame() {
    // create frame
    Frame thisframe(current_frame_number);
    
    thisframe.setPossession(home_possession);

    // store home and away positions and controls
    for (int i{0}; i < home_team->getPlayerCount(); i++) {
        auto plyr = home_team->getPlayer(i);
        thisframe.setHomePosition(i, plyr.getPosition());
        thisframe.setHomeControl(i, plyr.getControl());
    }
    for (int i{0}; i < away_team->getPlayerCount(); i++) {
        auto plyr = away_team->getPlayer(i);
        thisframe.setAwayPosition(i, plyr.getPosition());
        thisframe.setAwayControl(i, plyr.getControl());
    }

    return thisframe;
}

void Match::togglePossession() {
    home_possession = !home_possession;
    current_frame.togglePossession();
}
void Match::randomPossession() {
    int flip = (rand()%2)+ 1 ;
    if (flip == 1) togglePossession();
}
void Match::tryPossessionFlip() {
    int flip = (rand()%((int) possession_flip_reciprocal_probability))+ 1 ;
    if (flip == 1) togglePossession();
}

double Match::avgHomeControl(){
    return home_team_control_sum/current_frame_number;
}
double Match::avgAwayControl(){
    return away_team_control_sum/current_frame_number;
}

void Match::setMatchID(int mid) { matchID = mid; }

Cart Match::convertIdx2Coords(int i, int j){
  double X = ((i + 0.5) - x_samples/2) * pitch_data.x_dim / x_samples;
  double Y = ((j + 0.5) - y_samples/2) * pitch_data.y_dim / y_samples;
  return(Cart(X, Y));
}