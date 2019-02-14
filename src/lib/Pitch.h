#ifndef PITCH_H
#define PITCH_H

#include<cmath>



class Pitch {
    private:
        double x_dim, y_dim, length, x_limit, y_limit;

    public:
        Pitch();
        Pitch(double, double);
        

        void setXdimension(double);
        void setYdimension(double);

        double getXdim();
        double getYdim();
        double getXlim();
        double getYlim();
        double getPitchLength();


    
};

#endif