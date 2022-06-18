all: map-reduce.out reduce.out map.out

map-reduce.out: main.o def.o
	g++ main.o def.o -o map-reduce.out

reduce.out: reduce.o def.o
	g++  reduce.o def.o -o reduce.out

map.out: map.o def.o
	g++ map.o def.o -o map.out

main.o: main.cpp def.h
	g++ -c main.cpp  -o main.o

reduce.o: reduce.cpp  def.h
	g++ -c reduce.cpp   -o reduce.o

map.o: map.cpp  def.h
	g++ -c map.cpp  -o map.o

def.o: def.cpp def.h 
	g++ -c def.cpp -o def.o

clean:
	rm *.o
	rm *.out
