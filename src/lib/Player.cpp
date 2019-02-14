#include "Player.h"
#include "Team.h"

Player::Player() {}
Player::Player(int num) : shirt_number(num) {}
Player::Player(int num, Cart pos) : shirt_number(num), position(pos) {}

int Player::getShirtNum() const { return shirt_number; }
void Player::setShirtNum(int num) { shirt_number = num; }

double Player::getControl() const { return current_control; }
void Player::setControl(double ctrl) { current_control = ctrl; }

double Player::getStepSize() const { return max_step_size; }
void Player::setStepSize(double s_size) { max_step_size = s_size;  }

Team* Player::getTeamPtr() { return team; }
void Player::setTeamPtr(Team* tm) { team = tm; }

Cart Player::getPosition() const { return position; }

void Player::changePositionBy(double dx, double dy) { changePositionBy(Cart(dx, dy)); }

void Player::changePositionBy(Cart dPos) {
    // std::cout << "stuff " << std::endl;
    // position.print();

    // check length of move is legal, if not truncate.
    if (dPos.mod() <= max_step_size) {
        // legal move, make it
        position += dPos;
    } else {
        // trying to move too far
        // move max_step_size in desired direction
        position += dPos.unitVect() * max_step_size;
    }

    //std::cout << "SEG_FAULTING_HERE" << std::endl;
    // std::cout << getShirtNum() << std::endl;

    // std::cout << team->getPlayerCount() << "\t" << getShirtNum() << std::endl;


    double xlim = team->xlim();
    double ylim = team->ylim();
    // std::cout << getShirtNum() << std::endl;

    // std::cout << xlim << "\t" << ylim << std::endl;
    // std::cout << getShirtNum() << std::endl;


    // then check if move put player off pitch.
    if (position.xComp() > xlim) {
        // std::cout << "ERROR1" << std::endl;
        // position.print();
        // just correct x component
        position = Cart(xlim, position.yComp());
    } else if (position.xComp() < -1.0 * xlim) {
        // std::cout << "ERROR2" << std::endl;
        // position.print();
        position = Cart( -1.0 * xlim, position.yComp());
    }

    if (position.yComp() > ylim) {
        // std::cout << "ERROR3" << std::endl;
        // position.print();
        position = Cart(position.xComp(), ylim);
    } else if (position.yComp() < -1.0 * ylim) {
        // std::cout << "ERROR4" << std::endl;
        // position.print();
        position = Cart(position.xComp(), -1.0 * ylim) ;
    }

    // position.print();
    // std::cout << " endstuff" << std::endl;


}

void Player::changePositionTo(double dx, double dy) { changePositionTo(Cart(dx, dy)); }

void Player::changePositionTo(Cart target_position) {
    Cart displacement_vect = target_position - position;
    // can now use "changePositionBy" method with displacement vector
    changePositionBy(displacement_vect);
}

void Player::setInitialPosition(Cart init_pos) { position = init_pos;}

bool Player::isHomeTeam() const { return team->isHomeTeam(); }
bool Player::isSameTeam(Player& query_player) const { return isHomeTeam() == query_player.isHomeTeam(); }

void Player::setZeroControl() { current_control = 0; }
void Player::addControl(double dC) { current_control += dC; }
void Player::normControl(double norm) {current_control /= norm;  }