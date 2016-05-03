.DEFAULT_GOAL := all

CPP_FLAGS=-std=c++11 -O2 -Wall -pedantic -pthread
LIBS = -ldl

ALG_DIR=Algorithms/
COM_DIR=Common/
SIM_DIR=Simulation/

all: simulator 305220980_A_.so 305220980_B_.so 305220980_C_.so  

clean:
	rm *.so *.o simulator

simulator: main.o Simulator.o AlgorithmRunner.o Sensor.o AlgorithmManager.o AlgorithmRegistration.o AlgorithmRegistrar.o ConfigManager.o HouseManager.o House.o ErrorPrinter.o FileUtils.o $(LIBS)
	g++ -rdynamic -o $@ $^ $(CPP_FLAGS)

305220980_A_.so: _305220980_A.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_A_.so -o 305220980_A_.so _305220980_A.o GenericAlgorithm.o BatteryManager.o
	
305220980_B_.so: _305220980_B.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_B_.so -o 305220980_B_.so _305220980_B.o GenericAlgorithm.o BatteryManager.o

305220980_C_.so: _305220980_C.o GenericAlgorithm.o BatteryManager.o
	g++ -shared -Wl,-soname,305220980_C_.so -o 305220980_C_.so _305220980_C.o GenericAlgorithm.o BatteryManager.o

_305220980_A.o: $(ALG_DIR)_305220980_A.cpp $(ALG_DIR)_305220980_A.h $(ALG_DIR)GenericAlgorithm.h $(ALG_DIR)BatteryManager.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h $(COM_DIR)AlgorithmRegistration.h
	g++ $(CPP_FLAGS) -c $(ALG_DIR)_305220980_A.cpp

_305220980_B.o: $(ALG_DIR)_305220980_B.cpp $(ALG_DIR)_305220980_B.h $(ALG_DIR)GenericAlgorithm.h $(ALG_DIR)BatteryManager.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h $(COM_DIR)AlgorithmRegistration.h
	g++ $(CPP_FLAGS) -c $(ALG_DIR)_305220980_B.cpp
	
_305220980_C.o: $(ALG_DIR)_305220980_C.cpp $(ALG_DIR)_305220980_C.h $(ALG_DIR)GenericAlgorithm.h $(ALG_DIR)BatteryManager.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h $(COM_DIR)AlgorithmRegistration.h
	g++ $(CPP_FLAGS) -c $(ALG_DIR)_305220980_C.cpp

GenericAlgorithm.o: $(ALG_DIR)GenericAlgorithm.cpp $(ALG_DIR)GenericAlgorithm.h $(ALG_DIR)BatteryManager.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h $(COM_DIR)AlgorithmRegistration.h $(COM_DIR)MakeUniqueAdder.h
	g++ $(CPP_FLAGS) -c $(ALG_DIR)GenericAlgorithm.cpp

BatteryManager.o: $(ALG_DIR)BatteryManager.cpp $(ALG_DIR)BatteryManager.h
	g++ $(CPP_FLAGS) -c $(ALG_DIR)BatteryManager.cpp

main.o: $(SIM_DIR)main.cpp $(SIM_DIR)Simulator.h $(SIM_DIR)ErrorPrinter.h $(SIM_DIR)FileUtils.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)main.cpp

Simulator.o: $(SIM_DIR)Simulator.cpp $(SIM_DIR)Simulator.h $(SIM_DIR)AlgorithmRunner.h $(SIM_DIR)House.h $(SIM_DIR)ConfigManager.h $(SIM_DIR)HouseManager.h $(SIM_DIR)SimulationState.h $(SIM_DIR)FileUtils.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)Simulator.cpp

AlgorithmManager.o: $(SIM_DIR)AlgorithmManager.cpp $(SIM_DIR)AlgorithmManager.h $(SIM_DIR)AlgorithmRunner.h $(SIM_DIR)ConfigManager.h $(SIM_DIR)AlgorithmRegistrar.h $(SIM_DIR)AlgoRegState.h $(SIM_DIR)FileUtils.h $(SIM_DIR)ErrorPrinter.h 
	g++ $(CPP_FLAGS) -c $(SIM_DIR)AlgorithmManager.cpp

AlgorithmRegistrar.o: $(SIM_DIR)AlgorithmRegistrar.cpp $(SIM_DIR)AlgorithmRegistrar.h $(SIM_DIR)FileUtils.h $(SIM_DIR)AlgoRegState.h $(COM_DIR)AlgorithmRegistration.h $(COM_DIR)MakeUniqueAdder.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)AlgorithmRegistrar.cpp

AlgorithmRegistration.o: $(SIM_DIR)AlgorithmRegistration.cpp $(SIM_DIR)AlgorithmRegistrar.h $(COM_DIR)AlgorithmRegistration.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)MakeUniqueAdder.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)AlgorithmRegistration.cpp

AlgorithmRunner.o: $(SIM_DIR)AlgorithmRunner.cpp $(SIM_DIR)AlgorithmRunner.h $(SIM_DIR)Sensor.h $(SIM_DIR)House.h $(SIM_DIR)SimulationState.h $(COM_DIR)AbstractAlgorithm.h $(COM_DIR)AbstractSensor.h $(COM_DIR)SensorInformation.h $(COM_DIR)Direction.h 
	g++ $(CPP_FLAGS) -c $(SIM_DIR)AlgorithmRunner.cpp
	
Sensor.o: $(SIM_DIR)Sensor.cpp $(SIM_DIR)Sensor.h $(SIM_DIR)House.h $(COM_DIR)Direction.h $(COM_DIR)SensorInformation.h $(COM_DIR)AbstractSensor.h  
	g++ $(CPP_FLAGS) -c $(SIM_DIR)Sensor.cpp

HouseManager.o: $(SIM_DIR)HouseManager.cpp $(SIM_DIR)HouseManager.h $(SIM_DIR)House.h $(SIM_DIR)ErrorPrinter.h $(SIM_DIR)FileUtils.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)HouseManager.cpp

ConfigManager.o: $(SIM_DIR)ConfigManager.cpp $(SIM_DIR)ConfigManager.h $(SIM_DIR)ErrorPrinter.h $(SIM_DIR)FileUtils.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)ConfigManager.cpp

House.o: $(SIM_DIR)House.cpp $(SIM_DIR)House.h $(SIM_DIR)FileUtils.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)House.cpp

ErrorPrinter.o: $(SIM_DIR)ErrorPrinter.cpp $(SIM_DIR)ErrorPrinter.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)ErrorPrinter.cpp

FileUtils.o: $(SIM_DIR)FileUtils.cpp $(SIM_DIR)FileUtils.h
	g++ $(CPP_FLAGS) -c $(SIM_DIR)FileUtils.cpp