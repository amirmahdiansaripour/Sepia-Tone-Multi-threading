
all: ImageFilters.out main.o sepia.o washed_out.o cross.o

ImageFilters.out: main.o sepia.o washed_out.o cross.o
	g++ -std=c++11 -pthread main.o sepia.o washed_out.o cross.o -o ImageFilters.out

main.o: def.h main.cpp
	g++ -std=c++11 -pthread -c main.cpp  -o main.o

sepia.o : def.h sepia.cpp
	g++ -std=c++11 -pthread -c sepia.cpp -o sepia.o

washed_out.o : def.h washed_out.cpp	
	g++ -std=c++11 -pthread -c washed_out.cpp -o washed_out.o

cross.o : def.h cross.cpp
	g++ -std=c++11 -pthread -c cross.cpp -o cross.o


clean:
	rm *.o
	rm *.out
