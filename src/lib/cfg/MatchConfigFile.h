#ifndef MATCHCONFIGFILE_H
#define MATCHCONFIGFILE_H

#include <string>
#include <unordered_map>

class MatchConfigFile{
    friend class Match;
private:
    // file info
    std::string configFileName;

protected:
    void parseConfigFile(const std::unordered_map<std::string, std::string> & );

public:

    ////// FILE CONTENTS

    // sim info
    std::string save_filename;
    bool full_save;
    int sample_x, sample_y;
    double player_radius;
    double player_scatter_length;
    double possession_chance;
    double pass_chance;
    double ball_peak;
    double ball_decay;

    // match info
    int numberOfFrames;

    // pitch info
    double pitchX;
    double pitchY;

    std::string pitchMdl;
    double x_min, x_max; // min max for linear x model]
    double y_min, y_max;

    // raw parameters, incase we missed anything here
    std::unordered_map<std::string, std::string> raw_parameters;



    // constructors
    MatchConfigFile();
    MatchConfigFile(std::string); // filename

    void loadFile(); // use the filename in class
    void loadFile(std::string); // overload the filename (if given)

    // accessors and mutators
    void setConfigFileName(std::string);
    std::string getConfigFileName() const;

};

#endif