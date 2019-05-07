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
#include "strategies/MetricN.h"
#include "strategies/TetherCounter.h"
#include "strategies/Metric1.h"
#include "strategies/Metric3.h"
#include "strategies/TetherElastic.h"
#include "strategies/TetherMetricN.h"
#include "strategies/TetherElasticQuad.h"
#include "strategies/TetherElasticSpreading.h"
#include "strategies/TetherElasticMetricN.h"


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
    else if (name == "MetricN")
        return_strat = new MetricN(tcf_, pm);
    else if (name == "Metric3")
        return_strat = new Metric3(tcf_, pm);        
    else if (name == "TetherElastic")
        return_strat = new TetherElastic(tcf_, pm);
    else if (name == "TetherMetricN")
        return_strat = new TetherMetricN(tcf_, pm);
    else if (name == "TetherElasticQuad")
        return_strat = new TetherElasticQuad(tcf_, pm);
    else if (name == "TetherElasticMetricN")
        return_strat = new TetherElasticMetricN(tcf_, pm);
    else if (name == "TetherElasticSpreading")
        return_strat = new TetherElasticSpreading(tcf_, pm);
    // add more here
    else
    {
        // unrecognised value, throw error
        throw std::invalid_argument("Invalid AI type: " + name);
    }
    
    //return_strat->setPitch(pitch);
    return return_strat;
}