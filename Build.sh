FILE="sim"
SRCDIR="src-sim"

cd $SRCDIR # go into source code folder

echo "Compiling..." # compile the project

gcc -o $FILE TeamSim.cpp lib/Match.cpp lib/Pitch.cpp lib/PitchObject.cpp lib/Player.cpp lib/Team.cpp lib/Ball.cpp lib/vect/Cart.cpp lib/vect/Point.cpp lib/ai/AI.cpp lib/ai/RandomWalk.cpp lib/ai/ExchangeAI.cpp lib/ai/ExchangeMetricAI.cpp lib/ai/TestingAI.cpp lib/teams/ManagedTeam.cpp lib/teams/MetricTeam.cpp lib/teams/ExchangeTeam.cpp lib/teams/RandomWalkTeam.cpp lib/scenarios/Scenario.cpp lib/scenarios/UnitPolygon.cpp lib/scenarios/Exchange.cpp lib/scenarios/ExchangeMetric.cpp lib/scenarios/RandomWalkers.cpp lib/scenarios/TestingScenario.cpp lib/scenarios/ScenarioFromTeams.cpp -lm -std=c++14 -lstdc++

RESULT=$? # store return code

cd .. # move back to folder

if [ $RESULT = 0 ]; then # if compile was successful, run it
    printf "Success\n"
    # make folders
    if [ ! -d "bin" ]; then
        mkdir bin
    fi

    if [ ! -d "data_files" ]; then
        mkdir data_files
    fi

    if [ ! -d "data_files/csvs" ]; then
        mkdir data_files/csvs
    fi

    mv $SRCDIR/$FILE bin

fi