FILE="sim"
SRCDIR="src-sim"

cd $SRCDIR # go into source code folder

echo "Compiling..." # compile the project

gcc -o $FILE BasicSim.cpp lib/Match.cpp lib/Pitch.cpp lib/PitchObject.cpp lib/Player.cpp lib/Team.cpp lib/Ball.cpp lib/vect/Cart.cpp lib/ai/AI.cpp lib/ai/RandomWalk.cpp lib/ai/ExchangeAI.cpp lib/ai/ExchangeMetricAI.cpp lib/ai/TestingAI.cpp lib/scenarios/Scenario.cpp lib/scenarios/UnitPolygon.cpp lib/scenarios/Exchange.cpp lib/scenarios/ExchangeMetric.cpp lib/scenarios/RandomWalkers.cpp lib/scenarios/TestingScenario.cpp -lm -std=c++14 -lstdc++

RESULT=$? # store return code

cd .. # move back to folder

if [ $RESULT = 0 ]; then # if compile was successful, run it
  printf "Success\n"
  mkdir bin
  mkdir data_files
  mkdir data_files/csvs
  mv $SRCDIR/$FILE bin

fi
