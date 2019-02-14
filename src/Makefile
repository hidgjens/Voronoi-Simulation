
confsim: Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o
	g++ Simulation.o Cart.o Frame.o Match.o Pitch.o PitchModel.o Player.o SimManager.o Team.o TeamStrategy.o MatchConfigFile.o TeamConfigFile.o Uniform.o Xlinear.o Exchange.o Metric.o Random.o Spreading.o TeamExchange.o Tether.o TetherSpreading.o -o confsim -std=c++14

Simulation.o: Simulation.cpp
	g++ -c Simulation.cpp -std=c++14

# lib

Cart.o: lib/Cart.cpp lib/Cart.h
	g++ -c lib/Cart.cpp -std=c++14

Frame.o: lib/Frame.cpp lib/Frame.h
	g++ -c lib/Frame.cpp -std=c++14

Match.o: lib/Match.cpp lib/Match.h
	g++ -c lib/Match.cpp -std=c++14

Pitch.o: lib/Pitch.cpp lib/Pitch.h
	g++ -c lib/Pitch.cpp -std=c++14

PitchModel.o: lib/PitchModel.cpp lib/PitchModel.h
	g++ -c lib/PitchModel.cpp -std=c++14

Player.o: lib/Player.cpp lib/Player.o
	g++ -c lib/Player.cpp -std=c++14

SimManager.o: lib/SimManager.cpp lib/SimManager.h
	g++ -c lib/SimManager.cpp -std=c++14

Team.o: lib/Team.cpp lib/Team.h
	g++ -c lib/Team.cpp -std=c++14

TeamStrategy.o: lib/TeamStrategy.cpp lib/TeamStrategy.h
	g++ -c lib/TeamStrategy.cpp -std=c++14

# lib/cfg

MatchConfigFile.o: lib/cfg/MatchConfigFile.cpp lib/cfg/MatchConfigFile.h
	g++ -c lib/cfg/MatchConfigFile.cpp -std=c++14

TeamConfigFile.o: lib/cfg/TeamConfigFile.cpp lib/cfg/TeamConfigFile.h
	g++ -c lib/cfg/TeamConfigFile.cpp -std=c++14

# lib/models

Uniform.o: lib/models/Uniform.cpp lib/models/Uniform.h
	g++ -c lib/models/Uniform.cpp -std=c++14

Xlinear.o: lib/models/Xlinear.cpp lib/models/Xlinear.h
	g++ -c lib/models/Xlinear.cpp -std=c++14

# lib/strategies

Exchange.o: lib/strategies/Exchange.cpp lib/strategies/Exchange.h
	g++ -c lib/strategies/Exchange.cpp -std=c++14

Metric.o: lib/strategies/Metric.cpp lib/strategies/Metric.h
	g++ -c lib/strategies/Metric.cpp -std=c++14

Random.o: lib/strategies/Random.cpp lib/strategies/Random.h
	g++ -c lib/strategies/Random.cpp -std=c++14

Spreading.o: lib/strategies/Spreading.cpp lib/strategies/Spreading.h
	g++ -c lib/strategies/Spreading.cpp -std=c++14

TeamExchange.o: lib/strategies/TeamExchange.cpp lib/strategies/TeamExchange.h
	g++ -c lib/strategies/TeamExchange.cpp -std=c++14

Tether.o: lib/strategies/Tether.cpp lib/strategies/Tether.h
	g++ -c lib/strategies/Tether.cpp -std=c++14

TetherSpreading.o: lib/strategies/TetherSpreading.cpp lib/strategies/TetherSpreading.h
	g++ -c lib/strategies/TetherSpreading.cpp -std=c++14

clean:
	rm *.o confsim