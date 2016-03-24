CFLAGS=-std=c++11 -O2 -Wall -pedantic -pthread

all: simulator

clean:
	rm *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o SimpleAlgorithm.o ConfigManager.o HouseManager.o House.o 
	g++ $(CFLAGS) main.o Simulator.o AlgorithmRunner.o Sensor.o SimpleAlgorithm.o ConfigManager.o HouseManager.o House.o -o simulator

main.o: main.cpp Simulator.h
	g++ $(CFLAGS) -c main.cpp

Simulator.o: Simulator.cpp Simulator.h AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h SimpleAlgorithm.h House.h ConfigManager.h HouseManager.h SimulationFinishState.h
	g++ $(CFLAGS) -c Simulator.cpp

AlgorithmRunner.o: AlgorithmRunner.cpp AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h Sensor.h House.h SensorInformation.h Direction.h SimulationFinishState.h 
	g++ $(CFLAGS) -c AlgorithmRunner.cpp

SimpleAlgorithm.o: SimpleAlgorithm.cpp SimpleAlgorithm.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CFLAGS) -c SimpleAlgorithm.cpp

Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h SensorInformation.h  
	g++ $(CFLAGS) -c Sensor.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h
	g++ $(CFLAGS) -c HouseManager.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h
	g++ $(CFLAGS) -c ConfigManager.cpp

House.o: House.cpp House.h
	g++ $(CFLAGS) -c House.cpp
