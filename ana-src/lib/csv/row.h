#ifndef ROW_H
#define ROW_H

class Row{
    // row in csv
private:
    // contents of a row
    int idx;
    int pid;
    double time;
    int fid;
    std::string team;
    int num;
    double x;
    double y;
    double ctrl;
    double dCtrl;
    bool smart;

public:
    // constructors
    Row();
    Row(int, int, double, int, std::string, int, double, double, double, double, bool);
    
    // accessors 
    SetValues(int, int, double, int, std::string, int, double, double, double, double, bool);

    int get_idx() const;
    int get_pid() const;
    double get_time() const;
    int get_fid() const;
    std::string get_team() const;
    int get_num() const;
    double get_x() const;
    double get_y() const;
    double get_ctrl() const;
    double get_dCtrl() const;
    bool get_smart() const;

    // mutators
    void set_idx(int);
    void set_pid(int);
    void set_time(double);
    void set_fid(int);
    void set_team(std::string);
    void set_num(int);
    void set_x(double);
    void set_y(double);
    void set_ctrl(double);
    void set_dCtrl(double);
    void set_smart(bool);

};

#endif