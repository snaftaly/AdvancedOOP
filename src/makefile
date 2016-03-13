all: simulator

clean:
	rm *.o simulator

simulator: main.o House.o ConfigManager.o HouseManager.o
	g++ main.o House.o ConfigManager.o HouseManager.o -o simulator

main.o: main.cpp House.h ConfigManager.h HouseManager.h
	g++ -c main.cpp

ConfigManager.o: ConfigManager.cpp ConfigManager.h
	g++ -c ConfigManager.cpp

HouseManager.o: HouseManager.cpp HouseManager.h House.h
	g++ -c HouseManager.cpp

House.o: House.cpp House.h
	g++ -c House.cpp