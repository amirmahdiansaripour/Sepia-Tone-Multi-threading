all: ImageFilters.out main.o blur.o sepia.o washed_out.o cross.o

ImageFilters.out: main.o blur.o sepia.o washed_out.o cross.o
	g++ -std=c++11 main.o blur.o sepia.o washed_out.o cross.o -o ImageFilters.out

main.o: def.h main.cpp
	g++ -std=c++11 -c main.cpp  -o main.o

blur.o: def.h blur.cpp 
	g++ -std=c++11 -c blur.cpp -o blur.o

sepia.o : def.h sepia.cpp
	g++ -std=c++11 -c sepia.cpp -o sepia.o

washed_out.o : def.h washed_out.cpp	
	g++ -std=c++11 -c washed_out.cpp -o washed_out.o

cross.o : def.h cross.cpp
	g++ -std=c++11 -c cross.cpp -o cross.o


clean:
	rm *.o
	rm *.out
