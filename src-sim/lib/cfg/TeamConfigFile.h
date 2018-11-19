#ifndef TEAMCONFIGFILE_H
#define TEAMCONFIGFILE_H

#include <string>
#include <unordered_map>

class TeamConfigFile{
    // object to store all the information provided by the config file
private:
    // file info
    std::string configFileName;
    // team info
    std::string teamName;
    int playerCount;
    int smartPlayerCount;
    // string from file, not the AI object itself
    std::string ATK_normalAI_raw; // atk -> in possession
    std::string ATK_smartAI_raw;
    std::string DEF_normalAI_raw; // def -> out of possession
    std::string DEF_smartAI_raw;

protected:
    void parseConfigFile(const std::unordered_map<std::string, std::string> & );

public:
    TeamConfigFile();
    TeamConfigFile(std::string); // filename

    void loadFile(); // use the filename in class
    void loadFile(std::string); // overload the initial filename (if given)

    // accessors and mutators
    void setConfigFileName(std::string);
    std::string getConfigFileName() const;

    void setTeamName(std::string);
    std::string getTeamName() const;

    void setPlayerCount(int);
    int getPlayerCount() const;

    void setSmartCount(int);
    int getSmartCount() const;

    void setAtkNormAI(std::string);
    std::string getAtkNormAI() const;

    void setAtkSmartAI(std::string);
    std::string getAtkSmartAI() const;

    void setDefNormAI(std::string);
    std::string getDefNormAI() const;

    void setDefSmartAI(std::string);
    std::string getDefSmartAI() const;
};

#endif