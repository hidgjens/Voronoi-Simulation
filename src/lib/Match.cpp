#include "Match.h"
#include "Team.h"
#include<fstream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Player.h"
#include<iostream>


Match::Match() {}

Match::Match(TeamConfigFile ht, TeamConfigFile at, MatchConfigFile m) :
mcf(m),
home_team_control_sum(0),
away_team_control_sum(0)
{
    current_frame_number = 0;
    store_frames = mcf.full_save;
    x_samples = mcf.sample_x;
    y_samples = mcf.sample_y;
    player_radius = mcf.player_radius;
    player_scatter_length = mcf.player_scatter_length;
    possession_flip_reciprocal_probability = mcf.possession_chance;
    pitch_data = Pitch(mcf.pitchX, mcf.pitchY);
    // std::cout << "match " << pitch_data.getXlim() << "\t" << pitch_data.getYlim() << "\t" << pitch_data.getPitchLength() <<  "\t" << &pitch_data << std::endl;

    weight_model = PitchModel::createPitchModel(mcf.pitchMdl, mcf);
    home_team = Team(ht, true, weight_model, pitch_data);
    away_team = Team(at, false, weight_model, pitch_data);
    // std::cout << "Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;
    
    filename = "";//mcf.save_filename + ":" + ht.getConfigFileName() + ":" + at.getConfigFileName();
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
possession_flip_reciprocal_probability(m.possession_flip_reciprocal_probability),
player_radius(m.player_radius),
player_scatter_length(m.player_scatter_length)
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
    weight_model = new PitchModel(*m.weight_model);
    filename = m.filename;
    mcf = m.mcf;
    home_team = m.home_team;
    away_team = m.away_team;
    current_frame_number = m.current_frame_number;
    current_frame = m.current_frame;
    home_team_control_sum = m.home_team_control_sum;
    away_team_control_sum = m.away_team_control_sum;
    home_possession = m.home_possession;
    player_radius = m.player_radius;
    player_scatter_length = m.player_scatter_length;

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
    player_radius = m.player_radius;
    player_scatter_length = m.player_scatter_length;

    if (store_frames) {
        frame_history = std::move(m.frame_history);
    }

    return *this;
}

Pitch& Match::getPitchData() { return pitch_data; }

void Match::buildMatch() {
    //std::cout << home_team.getPlayerCount();
    //std::cout << "Match::buildMatch()\n"<< std::flush;
    //std::cout << "not even here\n"<< std::flush;
    // determine initial possession
    home_possession = true;
    randomPossession();

    current_frame = createFrame();
    // just need to store the first frame
    calculate_control();
    current_frame = createFrame();
    std::cout << "Initial positions" << std::endl;
    current_frame.printFrame();
    //std::cout<< "making frame"<< std::flush;
    
    if (store_frames) {
        frame_history[0] = current_frame;
    }
}

void Match::startSimulation() {
    std::cout << "Simulation starting" << std::endl;
    while (current_frame_number < total_frames - 1) {
        //std::cout<<"tick!\n"<< std::flush;
        tick();

        if (current_frame_number % ((int) total_frames / 10) == 0 || current_frame_number == total_frames - 1) {
            current_frame.printFrame();
        }
    }
    
    if (store_frames) saveFullMatch();
    saveMatchSummary();
    saveHeatmap();
    std::cout << "Match finished" << std::endl;
}

void Match::saveMatchSummary(){
    // Announce
  std::cout << "\nSaving summary for Match " << matchID << " to data_files/csv/" + filename + "/" + std::to_string(matchID) + ".sum\n"<< std::flush;

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

  std::cout << "\nSaving full Match " << matchID << " to data_files/csvs/" + filename + "/" + std::to_string(matchID) + ".csv\n\n"<< std::flush;
  
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

    for (int i{0}; i < home_team.getPlayerCount(); i++){

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

        index++; // increment index

      }

    
    // awayplayers
    for (int i{0}; i < away_team.getPlayerCount(); i++){

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

        index++; // increment index

      }

    }
  
  datafile.close();

}

void Match::calculate_control() {
  //std::cout << "calc_ctrdl\n"<< std::flush;
  //std::cout << "s"<< std::flush;
  double weighting;
  //std::cout << "this "<< std::flush;
  double dC;
  //std::cout << "that "<< std::flush;
  double total_ctrl = 0.0;

  //std::cout << "try "<< std::flush;
  double hm_count = home_team.getPlayerCount();
  //std::cout << "madeit "<< std::flush;

  for (int i{0}; i < home_team.getPlayerCount(); i++){
      //std::cout << "segging here i bet "<< std::flush;
    //   auto plyr = home_team.getPlayer(i);
    //   plyr.setZeroControl();
        home_team.setZeroControl(i);
  }
  for (int i{0}; i < away_team.getPlayerCount(); i++){
      //std::cout << "segging here i bet "<< std::flush;
        away_team.setZeroControl(i);
    //   auto plyr = away_team.getPlayer(i);
    //   plyr.setZeroControl();
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
      temp_pos = home_team.getPlayer(0).getPosition();
      temp_mindist = loc.dist2(temp_pos); // sponge
      temp_player  = 1; // index + 1; // sponge
      // iterate through rest of home team, look for closer player
      for (int num{1}; num < home_team.getPlayerCount(); num++){
        //std::cout << num << " act segging here i bet \n"<< std::flush;

        temp_pos = current_frame.getHomePosition(num);
        //std::cout << num << " act segging here i bet \n"<< std::flush;
    
        temp_dist = loc.dist2(temp_pos);
        if (temp_dist < temp_mindist){
          temp_mindist = temp_dist;
          temp_player = (num + 1);
        }
      }
      // iterate through away team
      for (int num{0}; num < away_team.getPlayerCount(); num++){
        temp_pos = current_frame.getAwayPosition(num);
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
        if (log10(dC) < -300.0) {
            std::cout << "low dC! " << dC << std::endl;
        }
        //auto plyr = home_team.getPlayer(temp_player - 1);
        //plyr.addControl(dC);
        home_team.addControl(temp_player - 1, dC);
        total_ctrl += dC;

      } else if (temp_player < 0){
        // std::cout << temp_player << std::endl;
          
        // away player
        //dC = 1.0 ;//* weight_model->pitchWeight(loc, current_frame);
        //auto plyr = away_team.getPlayer(abs(temp_player) - 1);
        away_team.addControl(abs(temp_player) - 1, dC);
        //plyr.addControl(dC);

        total_ctrl += dC;
      }
    }
  }
    // std::cout << "finished compute " << total_ctrl << std::endl;

    home_team.normControl(total_ctrl);
    away_team.normControl(total_ctrl);
}

void Match::tick() {
    // std::cout << "Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;

    // see if possession changes
    // std::cout<<"fliP,"<< std::flush;
    tryPossessionFlip();
    // increase frame counter

    current_frame_number++;
    // tell teams to update, give them current frame as info
    // std::cout<<"update," << std::flush;
    // std::cout << "2Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;


    home_team.update(current_frame);
    away_team.update(current_frame);
    // std::cout << "3Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;

    // recalculate control
    // std::cout<<"ctrl,"<< std::flush;
    collisionCheck();
    // std::cout << "Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;


    calculate_control();
    // store new frame
    current_frame = createFrame();
    // update sum
    home_team_control_sum += current_frame.getHomeTeamControl();
    away_team_control_sum += current_frame.getAwayTeamControl();

    // std::cout << "Home " << home_team.xlim() << "," << home_team.ylim() << " Away " << away_team.pitchptr() << std::endl;

    // if frames are stored, store it
    if (store_frames) frame_history[current_frame_number] = current_frame;

    // std::cout<<"done;\n"<< std::flush;

}

Frame Match::createFrame() {
    // create frame
    Frame thisframe(current_frame_number, home_team.getPlayerCount(), away_team.getPlayerCount());
    
    thisframe.setPossession(home_possession);

    // store home and away positions and controls
    for (int i{0}; i < home_team.getPlayerCount(); i++) {
        //std::cout << "get player" << std::flush;
        auto plyr = home_team.getPlayer(i);
        //std::cout << "Home Player pos " << std::flush; plyr.printPos(); std::cout << " player control " << plyr.getControl() << std::endl;
        //std::cout << "get pos" << std::flush;
        thisframe.setHomePosition(i, plyr.getPosition());
        //std::cout << "get ctrl" << std::flush;
        thisframe.setHomeControl(i, plyr.getControl());
    }
    for (int i{0}; i < away_team.getPlayerCount(); i++) {
        auto plyr = away_team.getPlayer(i);
        //std::cout << "Away Player pos " << std::flush; plyr.printPos(); std::cout << " player control " << plyr.getControl() << std::endl;

        thisframe.setAwayPosition(i, plyr.getPosition());
        thisframe.setAwayControl(i, plyr.getControl());
    }

    return thisframe;
}

void Match::togglePossession() {\

    home_possession = !home_possession;
    current_frame.togglePossession();
}
void Match::randomPossession() {
    int flip = (rand()%2)+ 1 ;
    if (flip == 1) togglePossession();
}
void Match::tryPossessionFlip() {
    int flip = (rand()%((int) possession_flip_reciprocal_probability))+ 1 ;

    if (flip == 1) {togglePossession();}
}

double Match::avgHomeControl(){
    return home_team_control_sum/current_frame_number;
}
double Match::avgAwayControl(){
    return away_team_control_sum/current_frame_number;
}

void Match::setMatchID(int mid) { matchID = mid; }

Cart Match::convertIdx2Coords(int i, int j){
  double X = ((i + 0.5) - x_samples/2.0) * pitch_data.getXdim()/ x_samples;
  double Y = ((j + 0.5) - y_samples/2.0) * pitch_data.getYdim()/ y_samples;
  return(Cart(X, Y));
}

void Match::appendToFilename(std::string extra) {
    filename = extra + filename;
}

void Match::collisionCheck() {
    /*
        Check if players are too close together (separation smaller than their physical size).
        If so we scatter the plays off each other by equal distances in opposite directions.
        Think of it as them running into each other and bouncing off one another
    */

    bool collision_detected = false; // set to false if collision is detected
    // temporary variables
    
    int run_counter = 0;

    // A collision is defined as D <= 2*R_player
    // run the collision check at least once, repeat if collisions were found to see if scattering solved the problem.
    do {
        Player* plyr1; Player* plyr2; 
        Cart pos1, pos2;
        // std::cout << "check" << std::endl;
        // reset boolean from previous check
        collision_detected = false; // "assume no collision until one is found"

        /*
            Check distance between each pair of players
            Get home player and compare against the remaining home players and the entire away team.
            Then check away player against remaining away players 
        */

        for (int i{0}; i < home_team.getPlayerCount(); i++) {
            // get first player
            plyr1 = home_team.getPlayerPtr(i);
            pos1 = plyr1->getPosition(); 

            // iterate through remaining home players
            for (int j{i + 1} ; j < home_team.getPlayerCount(); j++){
                // get second player
                plyr2 = home_team.getPlayerPtr(j);
                
                // get player positions
                pos2 = plyr2->getPosition();

                // check if distance between players 1 & 2 suggests a collision
                if (pos1.dist(pos2) <= 2 * player_radius) {
                    // std::cout << "home " << i << " home " << j << std::endl;
                    // this is a collision
                    collision_detected = true; // this ensures the process repeats to check collisions are removed
                    
                    // std::cout << plyr1->isHomeTeam() << "." << plyr1->getShirtNum() << " (" << pos1.xComp() << "," << pos1.yComp() << ")\t" << plyr2->isHomeTeam() << "." << plyr2->getShirtNum() << " (" << pos2.xComp() << "," << pos2.yComp() << ")" << std::endl; 
                    // scatter the players to remove the collision
                    //std::cout << plyr1 << "\t" << &plyr2 << std::endl;

                    scatterPlayers(plyr1, plyr2);
                }
                // end of for loop for remaining home players
            }

            // iterate through away team and compare with player1
            for (int j{0}; j < away_team.getPlayerCount(); j++) {
                // get second player
                plyr2 = away_team.getPlayerPtr(j);

                // get player positions
                pos2 = plyr2->getPosition();

                // check if distance between players 1 & 2 suggests a collision
                if (pos1.dist(pos2) <= 2 * player_radius) {
                    // std::cout << "home " << i << " away " << j << std::endl;

                    // this is a collision
                    collision_detected = true;
                    // std::cout << plyr1->isHomeTeam() << "." << plyr1->getShirtNum() << " (" << pos1.xComp() << "," << pos1.yComp() << ")\t" << plyr2->isHomeTeam() << "." << plyr2->getShirtNum() << " (" << pos2.xComp() << "," << pos2.yComp() << ")" << std::endl; 

                    // scatter the players to remove the collision
                    // std::cout << &plyr1 << "\t" << &plyr2 << std::endl;

                    scatterPlayers(plyr1, plyr2);
                }
                // end of loop for remaining away players
            }
        // end of home player 1 loop
        }

        // need to now check that no away team players are colliding with each other
        for (int i{0}; i < away_team.getPlayerCount(); i++){
            // get player one
            plyr1 = away_team.getPlayerPtr(i);
            pos1 = plyr1->getPosition(); 

            // iterate through remaining away team
            for (int j{i + 1}; j < away_team.getPlayerCount(); j++) {
                // get second player
                plyr2 = away_team.getPlayerPtr(j);

                // get player positions
                pos2 = plyr2->getPosition();

                // check if distance between players 1 & 2 suggests a collision
                if (pos1.dist(pos2) <= 2 * player_radius) {
                    // std::cout << "away " << i << " away " << j << std::endl;

                    // this is a collision
                    collision_detected = true;
                    // std::cout << plyr1->isHomeTeam() << "." << plyr1->getShirtNum() << " (" << pos1.xComp() << "," << pos1.yComp() << ")\t" << plyr2->isHomeTeam() << "." << plyr2->getShirtNum() << " (" << pos2.xComp() << "," << pos2.yComp() << ")" << std::endl; 

                    // scatter the players to remove the collision
                    // std::cout << &plyr1 << "\t" << &plyr2 << std::endl;

                    scatterPlayers(plyr1, plyr2);
                }
                // end of loop for remaining away players
            }

        // end of away player 1 loop
        }

        // end of collision check
        run_counter++;
        // if (collision_detected) std::cout << "COLLISION!" << std::endl;
        
    } while (collision_detected);

    //if (run_counter != 1)
    //    std::cout << "Frame " << current_frame_number << " needed " << run_counter << " collision check(s)" << std::endl;
}

void Match::scatterPlayers(Player* plyr1, Player* plyr2) {

    // std::cout << &plyr1 << "\t" << &plyr2 << std::endl;

    Cart pos1 = plyr1->getPosition();
    Cart pos2 = plyr2->getPosition();

    // std::cout << "Scatter before: "; 
    // std::cout << plyr1->isHomeTeam() << "." << plyr1->getShirtNum() << " (" << pos1.xComp() << "," << pos1.yComp() << ")\t" << plyr2->isHomeTeam() << "." << plyr2->getShirtNum() << " (" << pos2.xComp() << "," << pos2.yComp() << ")" << std::endl; 

    double rand_direction = ((double) rand() / RAND_MAX) * 2 * M_PI;
    double angle1 = rand_direction; double angle2 = rand_direction + M_PI; // angle2 = angle1 + 180 degrees (opposite directions)

    Cart dPos1(player_scatter_length * cos(angle1), player_scatter_length * sin(angle1));
    Cart dPos2(player_scatter_length * cos(angle2), player_scatter_length * sin(angle2));

    // dPos1.print(); dPos2.print(); std::cout << std::endl;

    plyr1->changePositionBy(dPos1); plyr2->changePositionBy(dPos2); 

    pos1 = plyr1->getPosition();
    pos2 = plyr2->getPosition();

    // std::cout << "Scatter after: ";
    // std::cout << plyr1->isHomeTeam() << "." << plyr1->getShirtNum() << " (" << pos1.xComp() << "," << pos1.yComp() << ")\t" << plyr2->isHomeTeam() << "." << plyr2->getShirtNum() << " (" << pos2.xComp() << "," << pos2.yComp() << ")" << std::endl; 
    // std::cout << std::endl;
    
}

void Match::saveHeatmap() {
  std::ofstream datafile;
  datafile.open("heatmap.csv");

  double weight;
  auto f = Frame(); f.setPossession(true);


  for (int j{0}; j < y_samples; j++) {
    for (int i{0}; i < x_samples; i++) {
        weight = weight_model->pitchWeight(convertIdx2Coords(i,j), f);

        datafile << weight << "\t";
    }
    datafile << "\n";
  }

}