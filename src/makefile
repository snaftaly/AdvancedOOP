.DEFAULT_GOAL := all

CPP_FLAGS=-std=c++11 -O2 -Wall -pedantic -pthread
LIBS = -ldl

all: simulator 305220980_A_.so 305220980_B_.so 305220980_C_.so  

clean:
	rm *.so *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o AlgorithmManager.o ConfigManager.o HouseManager.o House.o ErrorPrinter.o FileUtils.o $(LIBS)
	g++ -rdynamic -o $@ $^ $(CPP_FLAGS)

305220980_A_.so: _305220980_A.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_A_.so -o 305220980_A_.so _305220980_A.o GenericAlgorithm.o BatteryManager.o
	
305220980_B_.so: _305220980_B.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_B_.so -o 305220980_B_.so _305220980_B.o GenericAlgorithm.o BatteryManager.o

305220980_C_.so: _305220980_C.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_C_.so -o 305220980_C_.so _305220980_C.o GenericAlgorithm.o BatteryManager.o

_305220980_A.o: _305220980_A.cpp _305220980_A.h AbstractAlgorithm.h GenericAlgorithm.h AbstractSensor.h BatteryManager.h AlgosCommon.h
	g++ $(CPP_FLAGS) -c _305220980_A.cpp

_305220980_B.o: _305220980_B.cpp _305220980_B.h AbstractAlgorithm.h GenericAlgorithm.h AbstractSensor.h BatteryManager.h AlgosCommon.h
	g++ $(CPP_FLAGS) -c _305220980_B.cpp
	
_305220980_C.o: _305220980_C.cpp _305220980_C.h AbstractAlgorithm.h GenericAlgorithm.h AbstractSensor.h BatteryManager.h AlgosCommon.h
	g++ $(CPP_FLAGS) -c _305220980_C.cpp

GenericAlgorithm.o: GenericAlgorithm.cpp GenericAlgorithm.h AbstractAlgorithm.h AbstractSensor.h BatteryManager.h AlgosCommon.h
	g++ $(CPP_FLAGS) -c GenericAlgorithm.cpp

BatteryManager.o: BatteryManager.cpp BatteryManager.h
	g++ $(CPP_FLAGS) -c BatteryManager.cpp

main.o: main.cpp Simulator.h ErrorPrinter.h FileUtils.h
	g++ $(CPP_FLAGS) -c main.cpp

Simulator.o: Simulator.cpp Simulator.h AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h House.h ConfigManager.h HouseManager.h SimulationState.h FileUtils.h
	g++ $(CPP_FLAGS) -c Simulator.cpp

AlgorithmManager.o: AlgorithmManager.cpp AlgorithmManager.h AlgorithmRunner.h ConfigManager.h FileUtils.h ErrorPrinter.h AlgosCommon.h
	g++ $(CPP_FLAGS) -c AlgorithmManager.cpp

AlgorithmRunner.o: AlgorithmRunner.cpp AlgorithmRunner.h AbstractAlgorithm.h AbstractSensor.h Sensor.h House.h SensorInformation.h Direction.h SimulationState.h 
	g++ $(CPP_FLAGS) -c AlgorithmRunner.cpp
	
Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h Direction.h SensorInformation.h  
	g++ $(CPP_FLAGS) -c Sensor.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h ErrorPrinter.h FileUtils.h
	g++ $(CPP_FLAGS) -c HouseManager.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h ErrorPrinter.h FileUtils.h
	g++ $(CPP_FLAGS) -c ConfigManager.cpp

House.o: House.cpp House.h FileUtils.h
	g++ $(CPP_FLAGS) -c House.cpp

ErrorPrinter.o: ErrorPrinter.cpp ErrorPrinter.h
	g++ $(CPP_FLAGS) -c ErrorPrinter.cpp

FileUtils.o: FileUtils.cpp FileUtils.h
	g++ $(CPP_FLAGS) -c FileUtils.cpp