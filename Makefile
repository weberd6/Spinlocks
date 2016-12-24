
test: test.o AndersonLock.o MCSLock.o CLHLock.o
	g++ -std=c++14 test.o AndersonLock.o MCSLock.o CLHLock.o -lpthread -o test

test.o: test.cpp TicketLock.hpp AndersonLock.hpp MCSLock.hpp CLHLock.hpp
	g++ -std=c++14 -c test.cpp

AndersonLock.o: AndersonLock.cpp AndersonLock.hpp
	g++ -std=c++14 -c AndersonLock.cpp

MCSLock.o: MCSLock.cpp
	g++ -std=c++14 -c MCSLock.cpp

CLHLock.o: CLHLock.cpp
	g++ -std=c++14 -c CLHLock.cpp

