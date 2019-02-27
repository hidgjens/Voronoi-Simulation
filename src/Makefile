# variables
CC=g++
CFLAGS=-std=c++14

# Makefile

confsim: Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o PitchGrid.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o XlinYquad.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o TetherGrid.o
	${CC} Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o PitchGrid.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o XlinYquad.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o TetherGrid.o -o confsim ${CFLAGS}

Simulation.o: Simulation.cpp
	${CC} -c Simulation.cpp ${CFLAGS}

# lib

Cart.o: lib/Cart.cpp lib/Cart.h
	${CC} -c lib/Cart.cpp ${CFLAGS}

Frame.o: lib/Frame.cpp lib/Frame.h
	${CC} -c lib/Frame.cpp ${CFLAGS}

Match.o: lib/Match.cpp lib/Match.h
	${CC} -c lib/Match.cpp ${CFLAGS}

Pitch.o: lib/Pitch.cpp lib/Pitch.h
	${CC} -c lib/Pitch.cpp ${CFLAGS}

PitchModel.o: lib/PitchModel.cpp lib/PitchModel.h
	${CC} -c lib/PitchModel.cpp ${CFLAGS}

PitchGrid.o: lib/PitchGrid.cpp lib/PitchGrid.h
	${CC} -c lib/PitchGrid.cpp ${CFLAGS}

Player.o: lib/Player.cpp lib/Player.o
	${CC} -c lib/Player.cpp ${CFLAGS}

SimManager.o: lib/SimManager.cpp lib/SimManager.h
	${CC} -c lib/SimManager.cpp ${CFLAGS}

Team.o: lib/Team.cpp lib/Team.h
	${CC} -c lib/Team.cpp ${CFLAGS}

TeamStrategy.o: lib/TeamStrategy.cpp lib/TeamStrategy.h
	${CC} -c lib/TeamStrategy.cpp ${CFLAGS}

# lib/cfg

MatchConfigFile.o: lib/cfg/MatchConfigFile.cpp lib/cfg/MatchConfigFile.h
	${CC} -c lib/cfg/MatchConfigFile.cpp ${CFLAGS}

TeamConfigFile.o: lib/cfg/TeamConfigFile.cpp lib/cfg/TeamConfigFile.h
	${CC} -c lib/cfg/TeamConfigFile.cpp ${CFLAGS}

# lib/models

Uniform.o: lib/models/Uniform.cpp lib/models/Uniform.h
	${CC} -c lib/models/Uniform.cpp ${CFLAGS}

Xlinear.o: lib/models/Xlinear.cpp lib/models/Xlinear.h
	${CC} -c lib/models/Xlinear.cpp ${CFLAGS}

XlinYquad.o: lib/models/XlinYquad.cpp lib/models/XlinYquad.h
	${CC} -c lib/models/XlinYquad.cpp ${CFLAGS}

# lib/strategies

Exchange.o: lib/strategies/Exchange.cpp lib/strategies/Exchange.h
	${CC} -c lib/strategies/Exchange.cpp ${CFLAGS}

Metric.o: lib/strategies/Metric.cpp lib/strategies/Metric.h
	${CC} -c lib/strategies/Metric.cpp ${CFLAGS}

Random.o: lib/strategies/Random.cpp lib/strategies/Random.h
	${CC} -c lib/strategies/Random.cpp ${CFLAGS}

Spreading.o: lib/strategies/Spreading.cpp lib/strategies/Spreading.h
	${CC} -c lib/strategies/Spreading.cpp ${CFLAGS}

TeamExchange.o: lib/strategies/TeamExchange.cpp lib/strategies/TeamExchange.h
	${CC} -c lib/strategies/TeamExchange.cpp ${CFLAGS}

Tether.o: lib/strategies/Tether.cpp lib/strategies/Tether.h
	${CC} -c lib/strategies/Tether.cpp ${CFLAGS}

TetherSpreading.o: lib/strategies/TetherSpreading.cpp lib/strategies/TetherSpreading.h
	${CC} -c lib/strategies/TetherSpreading.cpp ${CFLAGS}

TetherGrid.o: lib/strategies/TetherGrid.cpp lib/strategies/TetherGrid.h
	${CC} -c lib/strategies/TetherGrid.cpp ${CFLAGS}

clean:
	-rm *.o 
	-rm confsim

	