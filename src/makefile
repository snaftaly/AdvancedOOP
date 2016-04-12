.DEFAULT_GOAL := simulator

CPP_FLAGS=-std=c++11 -O2 -Wall -pedantic -pthread

all: simulator

clean:
	rm *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o SimpleAlgorithm.o ConfigManager.o HouseManager.o House.o ErrorPrinter.o FileUtils.o 
	g++ -o $@ $^ $(CPP_FLAGS)

main.o: main.cpp Simulator.h ErrorPrinter.h FileUtils.h
	g++ $(CPP_FLAGS) -c main.cpp

Simulator.o: Simulator.cpp Simulator.h AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h SimpleAlgorithm.h House.h ConfigManager.h HouseManager.h SimulationFinishState.h FileUtils.h
	g++ $(CPP_FLAGS) -c Simulator.cpp

AlgorithmRunner.o: AlgorithmRunner.cpp AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h Sensor.h House.h SensorInformation.h Direction.h SimulationFinishState.h 
	g++ $(CPP_FLAGS) -c AlgorithmRunner.cpp

SimpleAlgorithm.o: SimpleAlgorithm.cpp SimpleAlgorithm.h AbstractAlgorithm.h AbstractSensor.h
	g++ $(CPP_FLAGS) -c SimpleAlgorithm.cpp

Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h SensorInformation.h  
	g++ $(CPP_FLAGS) -c Sensor.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h ErrorPrinter.h FileUtils.h
	g++ $(CPP_FLAGS) -c HouseManager.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h ErrorPrinter.h
	g++ $(CPP_FLAGS) -c ConfigManager.cpp

House.o: House.cpp House.h FileUtils.h
	g++ $(CPP_FLAGS) -c House.cpp

ErrorPrinter.o: ErrorPrinter.cpp ErrorPrinter.h
	g++ $(CPP_FLAGS) -c ErrorPrinter.cpp

FileUtils.o: FileUtils.cpp FileUtils.h
	g++ $(CPP_FLAGS) -c FileUtils.cpp