#ifndef PITCH_H
#define PITCH_H

#include<cmath>
#include<string>


class Pitch {
    private:
        double x_dim, y_dim, length, x_limit, y_limit;
        std::string match_name;
        int match_id;

    public:
        Pitch();
        Pitch(double, double);
        
        void setMatchName(std::string);
        void setMatchID(int);
        void setXdimension(double);
        void setYdimension(double);

        double getXdim();
        double getYdim();
        double getXlim();
        double getYlim();
        double getPitchLength();
        std::string getMatchName();
        int getMatchID();
};

#endif