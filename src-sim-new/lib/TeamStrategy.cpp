#include "TeamStrategy.h"

// add new strategies to include here and if statement in method below
#include "strategies/Random.h"
#include "strategies/Exchange.h"
#include "strategies/Metric.h"
#include "strategies/Spreading.h"
#include "strategies/Tether.h"
#include "strategies/TetherCounter.h"

TeamStrategy* TeamStrategy::CreateStrat(Pitch* pitch, TeamConfigFile tcf_) {

    std::string name = tcf_.team_strat_raw;

    TeamStrategy* return_strat;

    if (name == "Random")
        return_strat = new Random(pitch);
    else if (name == "Exchange")
        return_strat = new Exchange(tcf_, pitch);
    else if (name == "Metric")
        return_strat = new Metric(tcf_, pitch);
    else if (name == "Spreading")
        return_strat = new Spreading(tcf_, pitch);
    else if (name == "Tether")
        return_strat = new Tether(tcf_, pitch);
    else if (name == "TetherCounter")
        return_strat = new TetherCounter(tcf_, pitch);
    // add more here
    else
    {
        // unrecognised value, throw error
        throw std::invalid_argument("Invalid AI type: " + name);
    }
    
    //return_strat->setPitch(pitch);

    return return_strat;
}