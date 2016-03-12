all: testCPP

clean:
	rm *.o testCPP

testCPP: main.o
	g++ main.o -o testCPP

main.o: main.cpp
	g++ -c main.cpp


	