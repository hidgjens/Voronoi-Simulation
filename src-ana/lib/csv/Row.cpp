#include "Row.h"
#include "../CSV.h"

// constructors
Row::Row(){};
Row::Row(int IDX, int PID, double TIME, int FID, std::string TEAM, int NUM, double X, double Y, double dX, double dY, double CTRL, double DCTRL, double CTRL2, double DCTRL2, bool SMART)
    : idx(IDX)
    , pid(PID)
    , time_s(TIME)
    , team(TEAM)
    , num(NUM)
    , x(X)
    , y(Y)
    , dx(dX)
    , dy(dY)
    , ctrl(CTRL)
    , dCtrl(DCTRL)
    , ctrl2(CTRL2)
    , dCtrl2(DCTRL2)
    , smart(SMART)
{}

// set values from full set
void Row::setValues(int IDX, int PID, double TIME, int FID, std::string TEAM, int NUM, double X, double Y, double dX, double dY, double CTRL, double DCTRL, double CTRL2, double DCTRL2, bool SMART)
{
    set_idx(IDX);
    set_pid(PID);
    set_time(TIME);
    set_fid(FID);
    set_team(TEAM);
    set_num(NUM);
    set_x(X);
    set_y(Y);
    set_dx(dX);
    set_dy(dY);
    set_ctrl(CTRL);
    set_dCtrl(DCTRL);
    set_ctrl2(CTRL2);
    set_dCtrl2(DCTRL2);
    set_smart(SMART);
}

// accessors
int Row::get_idx() const { return idx; }
int Row::get_pid() const { return pid; }
double Row::get_time() const { return time_s; }
int Row::get_fid() const { return fid; }
std::string Row::get_team() const { return team; }
int Row::get_num() const { return num; }
double Row::get_x() const { return x; }
double Row::get_y() const { return y; }
Cart Row::get_pos() const { return Cart(x, y); }
double Row::get_ctrl() const { return ctrl; }
double Row::get_dCtrl() const { return dCtrl; }
bool Row::get_smart() const { return smart; }

void Row::print_row() const {
    std::cout 
        << idx << "\t"
        << pid << "\t"
        << time_s << "\t"
        << fid << "\t"
        << team << "\t"
        << num << "\t"
        << x << "\t"
        << y << "\t"
        << ctrl << "\t"
        << dCtrl << "\t"
        << smart << "\n";
}

// mutators
void Row::set_idx(int IDX) { idx = IDX; }
void Row::set_pid(int PID) { pid = PID; }
void Row::set_time(double TIME) { time_s = TIME; }
void Row::set_fid(int FID) { fid = FID; }
void Row::set_team(std::string TEAM) { team = TEAM; }
void Row::set_num(int NUM) { num = NUM; }
void Row::set_x(double X) { x = X; }
void Row::set_y(double Y) { y = Y; }
void Row::set_pos(Cart pos) { x = pos.xComp(); y = pos.yComp(); }
void Row::set_ctrl(double CTRL) { ctrl = CTRL; }
void Row::set_dCtrl(double DCTRL) { dCtrl = DCTRL; }
void Row::set_smart(bool SMART) { smart = SMART;}


