#!/bin/bash

FILE="confsim"
SRCDIR="src"

function compile () {
    # move into source directory and compile code
    cd $SRCDIR
    make

    RESULT=$? # store result to check if code compiled correctly

    cd .. # move back to starting folder

    if [ $RESULT = 0 ]; then # if compile was successful
        printf "Success\n"

        # make workspace folders
        if [ ! -d "bin" ]; then
            mkdir bin
        fi

        if [ ! -d "data_files" ]; then
            mkdir data_files
        fi

        if [ ! -d "data_files/csvs" ]; then
            mkdir data_files/csvs
        fi

        # move compiled file into bin directory
        cp $SRCDIR/$FILE bin
 
    fi
}

function clean () {
    cd $SRCDIR
    make clean

    RESULT=$?

    cd ..

    if [ $RESULT = 0 ]; then # if compile was successful
        printf "Success\n"
    fi
}

if [ $# = 0 ]; then
    compile
elif [ $1 = "clean" ]; then
    clean
fi


# function compile_configsim () {
#     FILE="confsim"
#     SRCDIR="src-sim-new"

#     cd $SRCDIR # go into source code folder

#     printf "Compiling configsim..."
#     gcc -o $FILE Simulation.cpp lib/SimManager.cpp \
#         lib/Match.cpp \
#         lib/Player.cpp lib/Team.cpp \
#         lib/Cart.cpp lib/Frame.cpp lib/Pitch.cpp \
#         lib/cfg/MatchConfigFile.cpp lib/cfg/TeamConfigFile.cpp \
#         lib/PitchModel.cpp lib/models/Uniform.cpp lib/models/Xlinear.cpp \
#         lib/TeamStrategy.cpp lib/strategies/Random.cpp lib/strategies/Exchange.cpp lib/strategies/Metric.cpp lib/strategies/Spreading.cpp \
#         lib/strategies/Tether.cpp lib/strategies/TeamExchange.cpp lib/strategies/TetherSpreading.cpp \
#     -lm -std=c++14 -lstdc++

#     RESULT=$?

#     cd .. # move back to folder

#     if [ $RESULT = 0 ]; then # if compile was successful, run it
#         printf "Success\n"
#         # make folders
#         if [ ! -d "bin" ]; then
#             mkdir bin
#         fi

#         if [ ! -d "data_files" ]; then
#             mkdir data_files
#         fi

#         if [ ! -d "data_files/csvs" ]; then
#             mkdir data_files/csvs
#         fi

#         mv $SRCDIR/$FILE bin

#     fi
# }


