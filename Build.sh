#!/bin/bash
# compile functions


function compile_configsim () {
    FILE="confsim"
    SRCDIR="src-sim-new"

    cd $SRCDIR # go into source code folder

    printf "Compiling configsim..."
    gcc -o $FILE Simulation.cpp lib/SimManager.cpp \
        lib/Match.cpp \
        lib/Player.cpp lib/Team.cpp \
        lib/Cart.cpp lib/Frame.cpp \
        lib/cfg/MatchConfigFile.cpp lib/cfg/TeamConfigFile.cpp \
        lib/PitchModel.cpp lib/models/Uniform.cpp lib/models/Xlinear.cpp \
        lib/TeamStrategy.cpp lib/strategies/Random.cpp lib/strategies/Exchange.cpp lib/strategies/Metric.cpp lib/strategies/Spreading.cpp \
        lib/strategies/Tether.cpp lib/strategies/TeamExchange.cpp \
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
    if [ $proj = "conf" ]; then
        compile_configsim
    else
        printf "Unknown project $proj\n"
    fi
done
