
all: ImageFilters.out main.o sepia.o blur.o washed_out.o bitmap.o

ImageFilters.out: main.o sepia.o washed_out.o blur.o bitmap.o
	g++ -std=c++11 -pthread main.o sepia.o washed_out.o blur.o bitmap.o -o ImageFilters.out

main.o: def.h main.cpp
	g++ -std=c++11 -pthread -c main.cpp  -o main.o

sepia.o : def.h sepia.cpp
	g++ -std=c++11 -pthread -c sepia.cpp -o sepia.o

blur.o : def.h blur.cpp
	g++ -std=c++11 -pthread -c blur.cpp -o blur.o

washed_out.o : def.h washed_out.cpp	
	g++ -std=c++11 -pthread -c washed_out.cpp -o washed_out.o

bitmap.o : def.h bitmap.cpp
	g++ -std=c++11 -pthread -c bitmap.cpp -o bitmap.o

clean:
	rm *.o
	rm *.out
