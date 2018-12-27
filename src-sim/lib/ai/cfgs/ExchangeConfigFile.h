#ifndef EXCHANGECONFIGFILE_H
#define EXCHANGECONFIGFILE_H

#include <unordered_map>

class ExchangeConfigFile {

    friend class ExchangeAI;
    friend class ExchangeAISTD;
    friend class ExchDsqrd;
    friend class MetricAI;
    friend class NaiveExchange;
    

    private:
        std::string configFileName = "Exchange";
        double decay_coeff = 1.0; 
        double same_coeff = 1.0;
        double opp_coeff = 1.0;

    protected:
        void parseConfigFile(const std::unordered_map<std::string, std::string> & );
    
    public:
        ExchangeConfigFile();
        ExchangeConfigFile(std::string);

        void loadFile();

        // accessors and mutators
        void setConfigFileName(std::string);
        std::string getConfigFileName() const;

};

#endif