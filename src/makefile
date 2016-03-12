all: testCPP

clean:
	rm *.o testCPP

testCPP: main.o House.o
	g++ main.o House.o -o testCPP

main.o: main.cpp House.h
	g++ -c main.cpp

House.o: House.cpp House.h
	g++ -c House.cpp


	