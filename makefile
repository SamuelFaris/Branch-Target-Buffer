
prog: main.o BTB.o
	g++ -Wall -std=c++11 main.o BTB.o -o BTB

main.o: main.cpp BTB.h
	g++ -g -c main.cpp

BTB.o: BTB.cpp BTB.h Entry.h
	g++ -g -c BTB.cpp

clean:
	-rm *.o BTB

debug:
	gdb BTB

run:
	@./BTB