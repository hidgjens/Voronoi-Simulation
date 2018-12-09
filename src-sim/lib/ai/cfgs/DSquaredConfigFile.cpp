#include "DSquaredConfigFile.h"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

DSquaredConfigFile::DSquaredConfigFile() { loadFile(); }
DSquaredConfigFile::DSquaredConfigFile(std::string nm) : configFileName(nm) { loadFile(); }

void DSquaredConfigFile::setConfigFileName(std::string nm) {configFileName = nm ;}
std::string DSquaredConfigFile::getConfigFileName() const { return configFileName; }

void DSquaredConfigFile::parseConfigFile(const std::unordered_map<std::string, std::string>& umap){
    // umap is the unordered map generated by the loadFile() function
    // going to check whether some required values are given in the file

/*  FIELDS [default]
    -> Decay coeff [1.0]
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

    // edge limit
    if (check_key("EdgeLimit"))
        edge_limit = std::stod(umap.at("EdgeLimit"));
    else
        edge_limit = 10.0;

    // edge scale
    if (check_key("EdgeScale"))
        edge_scale = std::stod(umap.at("EdgeScale"));
    else
        edge_scale = 5.0;

    // players to look at
    if (check_key("NearbyPlayers")) 
        players_to_consider = std::stoi(umap.at("NearbyPlayers"));
    else
        players_to_consider = 5;

    // directions samples to consider
    if (check_key("DirSamples"))
        density_samples = std::stoi(umap.at("DirSamples"));
    else
        density_samples = 18;
}


void DSquaredConfigFile::loadFile() { 
    // load a config file from 'teams/filename_.team' and store the data
    
    // store data from file in unordered map, and then select the values later
    std::unordered_map<std::string, std::string> data_umap;

    // temp variables
    std::string line, field;

    // open file
    std::ifstream CONFIGFILE("configs/parameters/" + configFileName + ".cfg");

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

    std::cout << configFileName + ":\n";
    for (auto x : data_umap) {
        std::cout << x.first << "\t" << x.second << "\n";
    }
    std::cout << "\n";

    // parse arguments
    parseConfigFile(data_umap);

}