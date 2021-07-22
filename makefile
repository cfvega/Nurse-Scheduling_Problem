all: NSP

NSP: NSP.o
	g++ -Wall -o NSP NSP.o
NSP.o: NSP.cpp
	g++ -Wall -c NSP.cpp
clear:
	rm *.o NSP