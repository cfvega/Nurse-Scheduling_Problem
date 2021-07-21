all: NSP

NSP: NSP.o
	gcc -Wall -o NSP NSP.o
NSP.o: NSP.cpp
	gcc -Wall -c NSP.cpp
clean:
	rm *.o NSP