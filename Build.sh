# compile functions

function compile_teamsim () {
    FILE="sim"
    SRCDIR="src-sim"

    cd $SRCDIR # go into source code folder

    printf "Compiling teamsim..."
    gcc -o $FILE TeamSim.cpp lib/Match.cpp lib/Pitch.cpp lib/PitchObject.cpp lib/Player.cpp lib/Team.cpp lib/Ball.cpp lib/vect/Cart.cpp lib/vect/Point.cpp lib/ai/AI.cpp lib/ai/RandomWalk.cpp lib/ai/ExchangeAI.cpp lib/ai/ExchangeMetricAI.cpp lib/ai/TestingAI.cpp lib/teams/ManagedTeam.cpp lib/teams/MetricTeam.cpp lib/teams/ExchangeTeam.cpp lib/teams/RandomWalkTeam.cpp lib/scenarios/Scenario.cpp lib/scenarios/UnitPolygon.cpp lib/scenarios/Exchange.cpp lib/scenarios/ExchangeMetric.cpp lib/scenarios/RandomWalkers.cpp lib/scenarios/TestingScenario.cpp lib/scenarios/ScenarioFromTeams.cpp -lm -std=c++14 -lstdc++

    RESULT=$?

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
}

function compile_metricsim () {
    FILE="metricsim"
    SRCDIR="src-sim"

    cd $SRCDIR # go into source code folder

    printf "Compiling metricsim..."
    gcc -o $FILE MetricSim.cpp lib/Match.cpp lib/Pitch.cpp lib/PitchObject.cpp lib/Player.cpp lib/Team.cpp lib/Ball.cpp lib/vect/Cart.cpp lib/vect/Point.cpp lib/ai/AI.cpp lib/ai/RandomWalk.cpp lib/ai/ExchangeAI.cpp lib/ai/ExchangeMetricAI.cpp lib/ai/TestingAI.cpp lib/teams/ManagedTeam.cpp lib/teams/MetricTeam.cpp lib/teams/ExchangeTeam.cpp lib/teams/RandomWalkTeam.cpp lib/scenarios/Scenario.cpp lib/scenarios/UnitPolygon.cpp lib/scenarios/Exchange.cpp lib/scenarios/ExchangeMetric.cpp lib/scenarios/RandomWalkers.cpp lib/scenarios/TestingScenario.cpp lib/scenarios/ScenarioFromTeams.cpp -lm -std=c++14 -lstdc++

    RESULT=$?

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
}

function compile_configsim () {
    FILE="confsim"
    SRCDIR="src-sim"

    cd $SRCDIR # go into source code folder

    printf "Compiling configsim..."
    gcc -o $FILE ConfigSim.cpp lib/SimManager.cpp \
        lib/Match.cpp lib/Pitch.cpp \
        lib/PitchObject.cpp lib/Player.cpp lib/Team.cpp lib/Ball.cpp \
        lib/vect/Cart.cpp lib/vect/Point.cpp \
        lib/PitchModel.cpp lib/models/Uniform.cpp lib/models/LinearX.cpp lib/models/Step.cpp lib/models/FourStep.cpp \
        lib/ai/AI.cpp lib/ai/RandomWalk.cpp lib/ai/Stationary.cpp lib/ai/ExchangeAI.cpp lib/ai/MetricAI.cpp \
        lib/ai/TestingAI.cpp lib/ai/Spreading.cpp lib/ai/DSquared.cpp lib/ai/ExchDsqrd.cpp lib/ai/DSquaredSTD.cpp \
        lib/ai/ExchangeAISTD.cpp lib/ai/influences/PitchEdge.cpp \
        lib/ai/cfgs/ExchangeConfigFile.cpp lib/ai/cfgs/DSquaredConfigFile.cpp \
        lib/cfg/MatchConfigFile.cpp lib/cfg/TeamConfigFile.cpp  \
    -lm -std=c++14 -lstdc++

    RESULT=$?

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
}

if [ $# = 0 ]; then
    echo "Build script: Type projects to build as arguments"
    exit 1
fi

for proj in "$@"
do
    if [ $proj = "met" ]; then
        compile_metricsim
    elif [ $proj = "team" ]; then
        compile_teamsim
    elif [ $proj = "conf" ]; then
        compile_configsim
    else
        printf "Unknown project $proj\n"
    fi
done
