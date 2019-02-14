FILE="ana"
SRCDIR="src-ana"

cd $SRCDIR # go into source code folder

echo "Compiling..." # compile the project

gcc -o $FILE AnalyseRun.cpp lib/CSV.cpp lib/csv/Row.cpp lib/vect/Cart.cpp -lm -std=c++14 -lstdc++

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