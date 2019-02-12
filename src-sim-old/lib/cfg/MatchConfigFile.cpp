#include "MatchConfigFile.h"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

MatchConfigFile::MatchConfigFile(){}
MatchConfigFile::MatchConfigFile(std::string fn_) : configFileName(fn_) { loadFile(); }

void MatchConfigFile::parseConfigFile(const std::unordered_map<std::string, std::string>& umap){
    // umap is the unordered map generated by the loadFile() function
    // going to check whether some required values are given in the file

/*  FIELDS [default]
    -> RunName [filename]
    -> NumberOfFrames [2000]
    -> Framerate [5.0]
    -> PitchX [105.0]
    -> PitchY [68.0]
    -> DecayParam [1.0]
    -> RepelParam [1.0]
    -> AttractParam [1.0]
*/

    // fn to check if key is present in umap
    auto check_key = [umap](std::string key_) -> bool{
        // check if key is in map
        if (umap.find(key_) == umap.end())
            // if key isn't found, umap.find returns end
            return false;
        else
            // key is found
            return true;
    };

    // RunName
    if (check_key("RunName"))
        runName = umap.at("RunName");
    else
        runName = configFileName;

    // PitchModel
    if (check_key("PitchModel"))
        pitchMdl = umap.at("PitchModel");
    else
        pitchMdl = "Uniform";

    // PitchModel details
    if (check_key("Wmin"))
        wmin = std::stod(umap.at("Wmin"));
    else
        wmin = 0.5;

    if (check_key("Wmax"))
        wmax = std::stod(umap.at("Wmax"));
    else
        wmax = 1.0;

    // NumberOfFrames
    if (check_key("NumberOfFrames"))
        numberOfFrames = std::stoi(umap.at("NumberOfFrames"));
    else
        numberOfFrames = 2000;

    // Framerate
    if (check_key("Framerate"))
        framerate = std::stod(umap.at("Framerate"));
    else
        framerate = 5.0;

    // PitchX
    if (check_key("PitchX"))
        pitchX = std::stod(umap.at("PitchX"));
    else
        pitchX = 105.0;

    // PitchY
    if (check_key("PitchY"))
        pitchY = std::stod(umap.at("PitchY"));
    else
        pitchY = 68.0;

    // DecayParam
    if (check_key("DecayParam"))
        decayParam = std::stod(umap.at("DecayParam"));
    else
        decayParam = 1.0;

    // RepelParam
    if (check_key("RepelParam"))
        repelParam = std::stod(umap.at("RepelParam"));
    else
        repelParam = 1.0;

    // AttractParam
    if (check_key("AttractParam"))
        attractParam = std::stod(umap.at("AttractParam"));
    else
        attractParam = 1.0;

}


void MatchConfigFile::loadFile() { loadFile(configFileName); }

void MatchConfigFile::loadFile(std::string filename_){
    // load a config file from 'teams/filename_.team' and store the data
    if (filename_ != configFileName) configFileName = filename_;
    // store data from file in unordered map, and then select the values later
    std::unordered_map<std::string, std::string> data_umap;

    // temp variables
    std::string line, field;

    // open file
    std::ifstream CONFIGFILE("configs-old/matches/" + configFileName + ".cfg");

    // iterate over lines of config file
    while(std::getline(CONFIGFILE, line)){
        // for each line, need to split the line at the colon to get the key and value
        // use a vector to store values temporarily
        std::vector<std::string> fields; // lines are Key:Value

        std::istringstream iss(line); // create a stringstream with that string and split by ":"

        // splits the line by ':' and iterates through each section of the line adding to vector. Should only be 2 values
        while(std::getline(iss, field, ':')){
            fields.push_back(field);
        }
        // add the loaded data to the umap
        data_umap.insert(std::make_pair(fields.at(0), fields.at(1)));
        
    }
    // close file
    CONFIGFILE.close();

    std::cout << filename_ + ":\n";
    for (auto x : data_umap) {
        std::cout << x.first << "\t" << x.second << "\n";
    }
    std::cout << "\n";

    // parse arguments
    parseConfigFile(data_umap);

}

// accessors and mutators
void MatchConfigFile::setConfigFileName(std::string fn_){ configFileName = fn_; }
std::string MatchConfigFile::getConfigFileName() const { return configFileName; }

void MatchConfigFile::setRunName(std::string rn_){ runName = rn_; }
std::string MatchConfigFile::getRunName() const { return runName; }

void MatchConfigFile::setMdlName(std::string mdl){ pitchMdl = mdl; }
std::string MatchConfigFile::getMdlName() const { return pitchMdl; }

void MatchConfigFile::setFramerate(double fr_) { framerate = fr_; }
double MatchConfigFile::getFramerate() const { return framerate; }

void MatchConfigFile::setNumberOfFrames(int nof_) { numberOfFrames = nof_; }
int MatchConfigFile::getNumberOfFrames() const { return numberOfFrames; }

void MatchConfigFile::setPitchX(double px_) { pitchX = px_; }
double MatchConfigFile::getPitchX() const { return pitchX; }

void MatchConfigFile::setPitchY(double py_) { pitchY = py_; }
double MatchConfigFile::getPitchY() const { return pitchY; }

void MatchConfigFile::setPitchDims(double px_, double py_) {pitchX = px_; pitchY = py_; }
void MatchConfigFile::setPitchDims(std::pair<double, double> pdims){pitchX = pdims.first; pitchY = pdims.second; }
std::pair<double, double> MatchConfigFile::getPitchDims() const { return std::make_pair(pitchX, pitchY); }

void MatchConfigFile::setDecayParam(double dp_) { decayParam = dp_; }
double MatchConfigFile::getDecayParam() const { return decayParam; }

void MatchConfigFile::setAttractParam(double ap_) { attractParam = ap_; }
double MatchConfigFile::getAttractParam() const { return attractParam; }

void MatchConfigFile::setRepelParam(double rp_) { repelParam = rp_; }
double MatchConfigFile::getRepelParam() const { return repelParam; }