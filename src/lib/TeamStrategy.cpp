#include "TeamStrategy.h"

// add new strategies to include here and if statement in method below
#include "strategies/Random.h"
#include "strategies/Exchange.h"
#include "strategies/Metric.h"
#include "strategies/Spreading.h"
#include "strategies/Tether.h"
#include "strategies/TeamExchange.h"
#include "strategies/TetherSpreading.h"
#include "strategies/TetherGrid.h"

#include "strategies/TetherCounter.h"
#include "strategies/Metric1.h"


TeamStrategy* TeamStrategy::CreateStrat(PitchModel* pm, TeamConfigFile tcf_) {

    std::string name = tcf_.team_strat_raw;

    TeamStrategy* return_strat;

    if (name == "Random")
        return_strat = new Random(pm);
    else if (name == "Exchange")
        return_strat = new Exchange(tcf_, pm);
    else if (name == "Metric")
        return_strat = new Metric(tcf_, pm);
    else if (name == "Spreading")
        return_strat = new Spreading(tcf_, pm);
    else if (name == "Tether")
        return_strat = new Tether(tcf_, pm);
    else if (name == "TeamExchange")
        return_strat = new TeamExchange(tcf_, pm);
    else if (name == "TetherSpreading")
        return_strat = new TetherSpreading(tcf_, pm);
    else if (name == "TetherGrid")
        return_strat = new TetherGrid(tcf_, pm);
    else if (name == "TetherCounter")
        return_strat = new TetherCounter(tcf_, pm);
    else if (name == "Metric1")
        return_strat = new Metric1(tcf_, pm);
 
    // add more here
    else
    {
        // unrecognised value, throw error
        throw std::invalid_argument("Invalid AI type: " + name);
    }
    
    //return_strat->setPitch(pitch);
    return return_strat;
}