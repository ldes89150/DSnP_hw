p1a: p1a.main.o p1a.o 
	g++ -o p1a p1a.main.o p1a.o

p1a.main.o: p1a.main.cpp p1a.h
	g++ -c p1a.main.cpp

p1a.o: p1a.cpp
	g++ -c p1a.cpp -g -Wall

p1b: p1b.main.o p1b.o
	g++ -o p1b p1b.main.o p1b.o

p1b.main.o: p1b.main.cpp p1b.h
	g++ -c p1b.main.cpp

p1b.o: p1b.cpp p1a.h
	g++ -c p1b.cpp

bug: p1.bug.o p1a.o
	g++ -o p1.bug -g -Wall p1.bug.o p1a.o

p1.bug.o: p1.bug.cpp p1a.h
	g++ -c p1.bug.cpp -g -Wall

clean:
	rm p1a.main.o p1a.o p1b.main.o p1b.o p1.bug.o
