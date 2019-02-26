#include "TeamConfigFile.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

TeamConfigFile::TeamConfigFile()
    {} // default constructor

TeamConfigFile::TeamConfigFile(std::string filename_) 
    : configFileName(filename_) 
    { loadFile(); }

void TeamConfigFile::parseConfigFile(const std::unordered_map<std::string, std::string>& umap){
    // umap is the unordered map generated by the loadFile() function
    // going to check whether some required values are given in the file

    // FIELDS [default]
    // -> TeamName [filename]
    // -> PlayerCount [11]
    // -> SmartCount [1]
    // -> ATK_NormAI [RandomWalk]
    // -> ATK_SmartAI
    // -> DEF_NormAI [ATK_NormAI]
    // -> DEF_SmartAI [ATK_SmartAI]

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

    // PlayerCount
    if (check_key("PlayerCount"))
        player_count = std::stoi(umap.at("PlayerCount"));
    else
        player_count = 11;

    // team strat
    if (check_key("Strategy"))
        team_strat_raw = umap.at("Strategy");
    else
        throw std::invalid_argument("Team " + configFileName + " has not Strategy!");

    if (check_key("MinDist"))
        min_team_dist = std::stoi(umap.at("MinDist"));
    else
        min_team_dist = 3;
    
    // exchange/metric parameters
    if (check_key("DecayCoeff"))
        decay_coefficient = std::stod(umap.at("DecayCoeff"));
    else
        decay_coefficient = 1.0;

    if (check_key("AttrCoeff"))
        attract_coefficient = std::stod(umap.at("AttrCoeff"));
    else
        attract_coefficient = 1.0;
    
    if (check_key("RepelCoeff"))
        repel_coefficient = std::stod(umap.at("RepelCoeff"));
    else
        repel_coefficient = 1.0;

    // spreading
    if (check_key("EdgeCoeff"))
        edge_coefficient = std::stod(umap.at("EdgeCoeff"));
    else
        edge_coefficient = 1.0;

    if (check_key("AllyCoeff"))
        ally_coefficient = std::stod(umap.at("AllyCoeff"));
    else
        ally_coefficient = 1.0;

    if (check_key("PlayersToSum"))
        players_to_consider = std::stoi(umap.at("PlayersToSum"));
    else
        players_to_consider = 5;

    if (check_key("MaxiSamples"))
        maxi_samples = std::stoi(umap.at("MaxiSamples"));
    else
        maxi_samples = 36;

    if (check_key("PostDistance"))
        max_post_distance = std::stod(umap.at("PostDistance"));
    else
        max_post_distance = 20.0;

    if (check_key("PriTargets"))
        targets_to_prioritise = std::stoi(umap.at("PriTargets"));
    else
        targets_to_prioritise = 2;
      
    

}

void TeamConfigFile::loadFile(){ loadFile(configFileName); } // use filename stored

void TeamConfigFile::loadFile(std::string filename_){
    // load a config file from 'teams/filename_.team' and store the data
    if (filename_ != configFileName) configFileName = filename_;
    // store data from file in unordered map, and then select the values later
    std::unordered_map<std::string, std::string> data_umap;

    // temp variables
    std::string line, field;

    // open file
    std::ifstream CONFIGFILE("configs/teams/" + configFileName + ".cfg");

    // iterate over lines of config file
    while(std::getline(CONFIGFILE, line)){
        // for each line, need to split the line at the colon to get the key and value
        // use a vector to store values temporarily
        std::vector<std::string> fields; // lines are Key:Value

        std::istringstream iss(line); // create a stringstream with that string and split by ":"

        // splits the line by ':' and iterates through each section of the line adding to vector. Should only be 2 values
        while(std::getline(iss, field, '=')){
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
    raw_parameters = data_umap;
    parseConfigFile(data_umap);

}

void TeamConfigFile::setConfigFileName(std::string fn_) {configFileName = fn_; }
std::string TeamConfigFile::getConfigFileName() const { return configFileName; }