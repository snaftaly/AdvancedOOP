all: simulator

clean:
	rm *.o simulator

simulator: main.o House.o ConfigManager.o HouseManager.o Sensor.o SimpleAlgorithm.o
	g++ -std=c++11 main.o House.o ConfigManager.o HouseManager.o Sensor.o SimpleAlgorithm.o -o simulator

main.o: main.cpp House.h ConfigManager.h HouseManager.h Sensor.h AbstractSensor.h
	g++ -std=c++11 -c main.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h
	g++ -std=c++11 -c ConfigManager.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h
	g++ -std=c++11 -c HouseManager.cpp

Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h
	g++ -std=c++11 -c Sensor.cpp

House.o: House.cpp House.h
	g++ -std=c++11 -c House.cpp

SimpleAlgorithm.o: SimpleAlgorithm.cpp SimpleAlgorithm.h AbstractAlgorithm.h Sensor.h
	g++ -std=c++11 -c SimpleAlgorithm.cpp