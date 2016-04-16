.DEFAULT_GOAL := simulator

CPP_FLAGS=-std=c++11 -O2 -Wall -pedantic -pthread

all: simulator

clean:
	rm *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o _305220980_A.o _305220980_B.o _305220980_C.o SimpleAlgorithm.o ConfigManager.o HouseManager.o House.o 
	g++ -o $@ $^ $(CPP_FLAGS)

main.o: main.cpp Simulator.h
	g++ $(CPP_FLAGS) -c main.cpp

Simulator.o: Simulator.cpp Simulator.h AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h _305220980_A.h _305220980_B.h _305220980_C.h SimpleAlgorithm.h House.h ConfigManager.h HouseManager.h SimulationFinishState.h
	g++ $(CPP_FLAGS) -c Simulator.cpp

AlgorithmRunner.o: AlgorithmRunner.cpp AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h Sensor.h House.h SensorInformation.h Direction.h SimulationFinishState.h 
	g++ $(CPP_FLAGS) -c AlgorithmRunner.cpp

SimpleAlgorithm.o: SimpleAlgorithm.cpp SimpleAlgorithm.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CPP_FLAGS) -c SimpleAlgorithm.cpp
	
_305220980_A.o: _305220980_A.cpp _305220980_A.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CPP_FLAGS) -c _305220980_A.cpp
	
_305220980_B.o: _305220980_B.cpp _305220980_B.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CPP_FLAGS) -c _305220980_B.cpp
	
_305220980_C.o: _305220980_C.cpp _305220980_C.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CPP_FLAGS) -c _305220980_C.cpp

Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h SensorInformation.h  
	g++ $(CPP_FLAGS) -c Sensor.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h
	g++ $(CPP_FLAGS) -c HouseManager.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h
	g++ $(CPP_FLAGS) -c ConfigManager.cpp

House.o: House.cpp House.h
	g++ $(CPP_FLAGS) -c House.cpp
