# variables
CC=g++
STD=c++14

# Makefie

confsim: Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o TetherCounter.o
	${CC} Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o TetherCounter.o -o confsim -std=${STD}

Simulation.o: Simulation.cpp
	${CC} -c Simulation.cpp -std=${STD}

# lib

Cart.o: lib/Cart.cpp lib/Cart.h
	${CC} -c lib/Cart.cpp -std=${STD}

Frame.o: lib/Frame.cpp lib/Frame.h
	${CC} -c lib/Frame.cpp -std=${STD}

Match.o: lib/Match.cpp lib/Match.h
	${CC} -c lib/Match.cpp -std=${STD}

Pitch.o: lib/Pitch.cpp lib/Pitch.h
	${CC} -c lib/Pitch.cpp -std=${STD}

PitchModel.o: lib/PitchModel.cpp lib/PitchModel.h
	${CC} -c lib/PitchModel.cpp -std=${STD}

Player.o: lib/Player.cpp lib/Player.o
	${CC} -c lib/Player.cpp -std=${STD}

SimManager.o: lib/SimManager.cpp lib/SimManager.h
	${CC} -c lib/SimManager.cpp -std=${STD}

Team.o: lib/Team.cpp lib/Team.h
	${CC} -c lib/Team.cpp -std=${STD}

TeamStrategy.o: lib/TeamStrategy.cpp lib/TeamStrategy.h
	${CC} -c lib/TeamStrategy.cpp -std=${STD}

# lib/cfg

MatchConfigFile.o: lib/cfg/MatchConfigFile.cpp lib/cfg/MatchConfigFile.h
	${CC} -c lib/cfg/MatchConfigFile.cpp -std=${STD}

TeamConfigFile.o: lib/cfg/TeamConfigFile.cpp lib/cfg/TeamConfigFile.h
	${CC} -c lib/cfg/TeamConfigFile.cpp -std=${STD}

# lib/models

Uniform.o: lib/models/Uniform.cpp lib/models/Uniform.h
	${CC} -c lib/models/Uniform.cpp -std=${STD}

Xlinear.o: lib/models/Xlinear.cpp lib/models/Xlinear.h
	${CC} -c lib/models/Xlinear.cpp -std=${STD}

# lib/strategies

Exchange.o: lib/strategies/Exchange.cpp lib/strategies/Exchange.h
	${CC} -c lib/strategies/Exchange.cpp -std=${STD}

Metric.o: lib/strategies/Metric.cpp lib/strategies/Metric.h
	${CC} -c lib/strategies/Metric.cpp -std=${STD}

Random.o: lib/strategies/Random.cpp lib/strategies/Random.h
	${CC} -c lib/strategies/Random.cpp -std=${STD}

Spreading.o: lib/strategies/Spreading.cpp lib/strategies/Spreading.h
	${CC} -c lib/strategies/Spreading.cpp -std=${STD}

TeamExchange.o: lib/strategies/TeamExchange.cpp lib/strategies/TeamExchange.h
	${CC} -c lib/strategies/TeamExchange.cpp -std=${STD}

Tether.o: lib/strategies/Tether.cpp lib/strategies/Tether.h
	${CC} -c lib/strategies/Tether.cpp -std=${STD}

TetherSpreading.o: lib/strategies/TetherSpreading.cpp lib/strategies/TetherSpreading.h
	${CC} -c lib/strategies/TetherSpreading.cpp -std=${STD}

TetherCounter.o: lib/strategies/TetherCounter.cpp lib/strategies/TetherCounter.h
	${CC} -c lib/strategies/TetherCounter.cpp -std=${STD}

clean:
	rm *.o confsim