#include "Frame.h"
#include "Player.h"

Frame::Frame() {
    home_player_count = away_player_count = 11;
    homePositions = std::make_unique<Cart[]>(home_player_count);
    awayPositions = std::make_unique<Cart[]>(away_player_count);
    homeControl = std::make_unique<double[]>(home_player_count);
    awayControl = std::make_unique<double[]>(away_player_count);
}
Frame::Frame(int num, int h, int a) : frameNumber(num), home_player_count(h), away_player_count(a) {
    homePositions = std::make_unique<Cart[]>(home_player_count);
    awayPositions = std::make_unique<Cart[]>(away_player_count);
    homeControl = std::make_unique<double[]>(home_player_count);
    awayControl = std::make_unique<double[]>(away_player_count);
}

Frame::Frame(Frame& frame) : 
    frameNumber(frame.frameNumber),
    home_player_count(frame.home_player_count),
    away_player_count(frame.away_player_count),
    home_possession(frame.home_possession),
    ball_tile(frame.ball_tile),
    ball_position(frame.ball_position) {
        homePositions = std::make_unique<Cart[]>(home_player_count);
        for (int i{0}; i < home_player_count; i++){
            homePositions[i] = frame.homePositions[i];
        }

        homeControl = std::make_unique<double[]>(home_player_count);
        for (int i{0}; i < home_player_count; i++){
            homeControl[i] = frame.homeControl[i];
        }

        awayPositions = std::make_unique<Cart[]>(away_player_count);
        for (int i{0}; i < away_player_count; i++){
            awayPositions[i] = frame.awayPositions[i];
        }

        awayControl = std::make_unique<double[]>(away_player_count);
        for (int i{0}; i < away_player_count; i++) {
            awayControl[i] = frame.awayControl[i];
        }
    }

Frame& Frame::operator=(Frame& frame) {
    if (&frame == this) {
        return *this;
    }

    frameNumber = frame.frameNumber;
    home_player_count = frame.home_player_count;
    away_player_count = frame.away_player_count;
    home_possession = frame.home_possession;
    ball_tile = frame.ball_tile;
    ball_position = frame.ball_position;

    homePositions = std::make_unique<Cart[]>(home_player_count);
    for (int i{0}; i < home_player_count; i++){
        homePositions[i] = frame.homePositions[i];
    }

    homeControl = std::make_unique<double[]>(home_player_count);
    for (int i{0}; i < home_player_count; i++){
        homeControl[i] = frame.homeControl[i];
    }

    awayPositions = std::make_unique<Cart[]>(away_player_count);
    for (int i{0}; i < away_player_count; i++){
        awayPositions[i] = frame.awayPositions[i];
    }

    awayControl = std::make_unique<double[]>(away_player_count);
    for (int i{0}; i < away_player_count; i++) {
        awayControl[i] = frame.awayControl[i];
    }

    return *this;
}

Frame& Frame::operator=(Frame&& frame) {
    if (&frame == this) {
        return *this;
    }

    frameNumber = frame.frameNumber;
    home_player_count = frame.home_player_count;
    away_player_count = frame.away_player_count;
    home_possession = frame.home_possession;
    ball_tile = frame.ball_tile;
    ball_position = frame.ball_position;

    homePositions = std::move(frame.homePositions);
    homeControl = std::move(frame.homeControl);
    frame.home_player_count = 0;

    awayPositions = std::move(frame.awayPositions);
    awayControl = std::move(frame.awayControl);
    frame.away_player_count = 0;

    return *this;

}

void Frame::setFrameNumber(int num) { frameNumber = num; }
int Frame::getFrameNumber() const { return frameNumber; }

void Frame::setPossession(bool pos) { home_possession = pos; }
bool Frame::getPossession() const { return home_possession; }
void Frame::togglePossession() { home_possession != home_possession; }

void Frame::setHomePosition(int num, Cart pos) { homePositions[num] = pos; }
Cart Frame::getHomePosition(int num) { return homePositions[num]; }

void Frame::setHomeControl(int num, double ctrl) { homeControl[num] = ctrl; }
double Frame::getHomeControl(int num) const { return homeControl[num]; }

void Frame::setAwayPosition(int num, Cart pos) { awayPositions[num] = pos; }
Cart Frame::getAwayPosition(int num) { return awayPositions[num]; }

void Frame::setAwayControl(int num, double ctrl) { awayControl[num] = ctrl; }
double Frame::getAwayControl(int num) const { return awayControl[num]; }

void Frame::printFrame() const {
    std::string possession_str;
    if (home_possession) {
        possession_str = "HOME";
    } else {
        possession_str = "AWAY";
    }
    std::cout << "\n-------------------------------------------------------------------------" << std::endl;
    std::cout << "Frame " << frameNumber << " | " << "Possession: " << possession_str << " | Ball Tile: " << ball_tile << " (" << ball_position.xComp() << "," << ball_position.yComp() << ")" << std::endl;
    std::cout << "\tHome Team:" << std::endl;
    for (int i{0}; i < home_player_count; i++){
        std::cout << "\t\tPlyr " << i << "\t(" << homePositions[i].xComp() << "," << homePositions[i].yComp() << ")\tCtrl: " << homeControl[i] << std::endl;
    }
    std::cout << "\n\tAway Team:" << std::endl;
    for (int i{0}; i < away_player_count; i++){
        std::cout << "\t\tPlyr " << i << "\t(" << awayPositions[i].xComp() << "," << awayPositions[i].yComp() << ")\tCtrl: " << awayControl[i] << std::endl;
    }
    std::cout << "\n-------------------------------------------------------------------------" << std::endl;

}

int Frame::getOpponentCount(bool hometeam) {
    if (hometeam) return away_player_count;
    else return home_player_count;
}
Cart Frame::getOpponentPosition(bool hometeam, int shirt_num) {
    if (hometeam) return awayPositions[shirt_num];
    else return homePositions[shirt_num];
}
double Frame::getOpponentControl(bool hometeam, int shirt_num) {
    if (hometeam) return awayControl[shirt_num];
    else return homeControl[shirt_num];
}

int Frame::getAlliedCount(bool hometeam) {
    if (hometeam) return home_player_count;
    else return away_player_count;
}
Cart Frame::getAlliedPosition(bool hometeam, int shirt_num) {
    if (hometeam) return homePositions[shirt_num];
    else return awayPositions[shirt_num];
}
double Frame::getAlliedControl(bool hometeam, int shirt_num) {
    if (hometeam) return homeControl[shirt_num];
    else return awayControl[shirt_num];
}

Cart Frame::getNearestOpponentPos(bool hometeam, Cart pos) {
    double mindist = -10.0, temp_dist;
    Cart closest_position, temp_pos;

    for (int i{0}; i < getOpponentCount(hometeam); i++) {
        temp_pos = getOpponentPosition(hometeam, i);
        temp_dist = temp_pos.dist(pos);

        if (temp_dist < mindist || mindist < 0) {
            mindist = temp_dist;
            closest_position = temp_pos;
        }
    }

    return closest_position;
}

double Frame::getNearestOpponentDist(bool hometeam, Cart pos) {
    Cart nearestpos = getNearestOpponentPos(hometeam, pos);
    return nearestpos.dist(pos);
}

double Frame::getNearestOpponentCtrl(bool hometeam, Cart pos) {
    double mindist = -10.0, temp_dist;
    double closest_control; Cart temp_pos;

    for (int i{0}; i < getOpponentCount(hometeam); i++) {
        temp_pos = getOpponentPosition(hometeam, i);
        temp_dist = temp_pos.dist(pos);

        if (temp_dist < mindist || mindist < 0) {
            mindist = temp_dist;
            closest_control = getOpponentControl(hometeam, i);
        }
    }

    return closest_control;
}

int Frame::getNearestOpponentShirtNum(bool hometeam, Cart pos) {
    double mindist = -10.0, temp_dist;
    int closest_sn; Cart temp_pos;

    for (int i{0}; i < getOpponentCount(hometeam); i++) {
        temp_pos = getOpponentPosition(hometeam, i);
        temp_dist = temp_pos.dist(pos);

        if (temp_dist < mindist || mindist < 0) {
            mindist = temp_dist;
            closest_sn = i;
        }
    }

    return closest_sn;
}


Cart Frame::getNearestAllyPos(bool hometeam, Player* plyr) {
    Cart pos = plyr->getPosition();
    int shirt_num = plyr->getShirtNum();

    double mindist = -10.0, temp_dist;
    Cart closest_position, temp_pos;

    for (int i{0}; i < getAlliedCount(hometeam); i++) {
        if (i != shirt_num){
            // dont want to include the same player
            temp_pos = getAlliedPosition(hometeam, i);
            temp_dist = temp_pos.dist(pos);

            if (temp_dist < mindist || mindist < 0) {
                mindist = temp_dist;
                closest_position = temp_pos;
            }
        }
        
    }

    return closest_position;
}

double Frame::getNearestAllyDist(bool hometeam, Player* plyr) {
    Cart nearestpos = getNearestAllyPos(hometeam, plyr);
    Cart pos = plyr->getPosition();
    return nearestpos.dist(pos);  
}

double Frame::getNearestAllyCtrl(bool hometeam, Player* plyr){
    Cart pos = plyr->getPosition();
    int shirt_num = plyr->getShirtNum();

    double mindist = -10.0, temp_dist;
    double closest_control; Cart temp_pos;

    for (int i{0}; i < getAlliedCount(hometeam); i++) {
        if (i != shirt_num){
            // dont want to include the same player
            temp_pos = getAlliedPosition(hometeam, i);
            temp_dist = temp_pos.dist(pos);

            if (temp_dist < mindist || mindist < 0) {
                mindist = temp_dist;
                closest_control = getAlliedControl(hometeam, i);
            }
        }
        
    }

    return closest_control;
}

int Frame::getNearestAllyShirtNum(bool hometeam, Player* plyr) {
    Cart pos = plyr->getPosition();
    int shirt_num = plyr->getShirtNum();

    double mindist = -10.0, temp_dist;
    int closest_sn; Cart temp_pos;

    for (int i{0}; i < getAlliedCount(hometeam); i++) {
        if (i != shirt_num){
            // dont want to include the same player
            temp_pos = getAlliedPosition(hometeam, i);
            temp_dist = temp_pos.dist(pos);

            if (temp_dist < mindist || mindist < 0) {
                mindist = temp_dist;
                closest_sn = i;
            }
        }
        
    }

    return closest_sn;    
}

double Frame::getHomeTeamControl() {
    double teamctrl = 0.0;
    for (int i{0}; i < home_player_count; i++) {
        teamctrl += homeControl[i];
    }
    return teamctrl;
}

double Frame::getAwayTeamControl() {
    double teamctrl = 0.0;
    for (int i{0}; i < away_player_count; i++) {
        teamctrl += awayControl[i];
    }
    return teamctrl;
}

std::string Frame::getHomePossStr() {
    if (home_possession)
        return "True";
    else
        return "False";
}

std::string Frame::getAwayPossStr() {
    if (home_possession)
        return "False";
    else
        return "True";
}

int Frame::getBallTile() { return ball_tile; }
void Frame::setBallTile(int i) { ball_tile = i; }
Cart Frame::getBallPosition() { return ball_position; }
void Frame::setBallPosition(Cart c) { ball_position = c; }