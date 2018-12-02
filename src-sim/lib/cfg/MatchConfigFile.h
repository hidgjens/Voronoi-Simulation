#ifndef MATCHCONFIGFILE_H
#define MATCHCONFIGFILE_H

#include <string>
#include <unordered_map>

class MatchConfigFile{

private:
    // file info
    std::string configFileName;
    
    // match info
    std::string runName, pitchMdl;
    int numberOfFrames;
    double framerate; // only necessary if trying to keep things physical

    double pitchX, pitchY;
    double decayParam, repelParam, attractParam;

protected:
    void parseConfigFile(const std::unordered_map<std::string, std::string> & );

public:
    // constructors
    MatchConfigFile();
    MatchConfigFile(std::string); // filename

    void loadFile(); // use the filename in class
    void loadFile(std::string); // overload the filename (if given)

    // accessors and mutators
    void setConfigFileName(std::string);
    std::string getConfigFileName() const;

    void setRunName(std::string);
    std::string getRunName() const;

    void setMdlName(std::string);
    std::string getMdlName() const;

    void setFramerate(double);
    double getFramerate() const;
    
    void setNumberOfFrames(int);
    int getNumberOfFrames() const;

    void setPitchX(double);
    double getPitchX() const;

    void setPitchY(double);
    double getPitchY() const;

    void setPitchDims(double, double);
    void setPitchDims(std::pair<double, double>);
    std::pair<double, double> getPitchDims() const;

    void setDecayParam(double);
    double getDecayParam() const;

    void setRepelParam(double);
    double getRepelParam() const;

    void setAttractParam(double);
    double getAttractParam() const;

};

#endif