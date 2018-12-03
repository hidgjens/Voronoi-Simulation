#ifndef DSQUAREDCONFIGFILE_H
#define DSQUAREDCONFIGFILE_H

#include <unordered_map>

class DSquaredConfigFile {

    friend class DSquared;
    friend class DSquaredSTD;
    friend class ExchDsqrd;

    private:
        std::string configFileName = "DSquared";
        double edge_limit;
        double edge_scale;
        int players_to_consider;
        int density_samples;

    protected:
        void parseConfigFile(const std::unordered_map<std::string, std::string> & );
    public:
        DSquaredConfigFile();
        DSquaredConfigFile(std::string);

        void loadFile();

        // accessors and mutators
        void setConfigFileName(std::string);
        std::string getConfigFileName() const;

};

#endif