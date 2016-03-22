all: simulator

clean:
	rm *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o SimpleAlgorithm.o ConfigManager.o HouseManager.o House.o 
	g++ -std=c++11 main.o House.o ConfigManager.o HouseManager.o Sensor.o SimpleAlgorithm.o -o simulator

main.o: main.cpp House.h ConfigManager.h HouseManager.h Sensor.h AbstractSensor.h
	g++ -std=c++11 -c main.cpp

Simulator.o: Simulator.cpp Simulator.h AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h House.h ConfigManager.h HouseManager.h
	g++ -std=c++11 -c Simulator.cpp

AlgorithmRunner.o: AlgorithmRunner.cpp AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h Sensor.h House.h SensorInformation.h Direction.h
	g++ -std=c++11 -c AlgorithmRunner.cpp

SimpleAlgorithm.o: SimpleAlgorithm.cpp SimpleAlgorithm.h AbstractAlgorithm.h AbstractSensor.h
	g++ -std=c++11 -c SimpleAlgorithm.cpp

Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h SensorInformation.h  
	g++ -std=c++11 -c Sensor.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h
	g++ -std=c++11 -c HouseManager.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h
	g++ -std=c++11 -c ConfigManager.cpp

House.o: House.cpp House.h
	g++ -std=c++11 -c House.cpp
