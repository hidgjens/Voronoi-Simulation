#ifndef ROW_H
#define ROW_H

#include<string>
#include "../vect/Cart.h"

class CSV;

class Row{
    // row in csv
private:
    // contents of a row
    int idx;
    int pid;
    double time_s;
    int fid;
    std::string team;
    int num;
    double x;
    double y;
    double dx;
    double dy;
    double ctrl;
    double dCtrl;
    double ctrl2;
    double dCtrl2;
    bool smart;

public:
    // constructors
    Row();
    Row(int, int, double, int, std::string, int, double, double, double, double, double, double, double, double, bool);
    
    // accessors 
    void setValues(int, int, double, int, std::string, int, double, double, double, double, double, double, double, double, bool);

    int get_idx() const;
    int get_pid() const;
    double get_time() const;
    int get_fid() const;
    std::string get_team() const;
    int get_num() const;
    double get_x() const;
    double get_y() const;
    double get_dx() const;
    double get_dy() const;
    Cart get_pos() const;
    double get_ctrl() const;
    double get_dCtrl() const;
    double get_ctrl2() const;
    double get_dCtrl2() const;
    bool get_smart() const;

    void print_row() const;

    // mutators
    void set_idx(int);
    void set_pid(int);
    void set_time(double);
    void set_fid(int);
    void set_team(std::string);
    void set_num(int);
    void set_x(double);
    void set_y(double);
    void set_dx(double);
    void set_dy(double);
    void set_pos(Cart);
    void set_ctrl(double);
    void set_dCtrl(double);
    void set_ctrl2(double);
    void set_dCtrl2(double);
    void set_smart(bool);

};

#endif