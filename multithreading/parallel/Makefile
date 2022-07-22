
all: ImageFilters.out main.o sepia.o bitmap.o

ImageFilters.out: sepia.o bitmap.o main.o
	g++ -std=c++11 -pthread sepia.o bitmap.o main.o -o ImageFilters.out

sepia.o : def.h sepia.cpp
	g++ -std=c++11 -pthread -c sepia.cpp -o sepia.o

bitmap.o : def.h bitmap.cpp
	g++ -std=c++11 -pthread -c bitmap.cpp -o bitmap.o

main.o: def.h main.cpp
	g++ -std=c++11 -pthread -c main.cpp  -o main.o

clean:
	rm *.o
	rm *.out
