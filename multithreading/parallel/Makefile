
all: ParellelFiltering.out main.o sepia.o bitmap.o

ParellelFiltering.out: sepia.o bitmap.o main.o
	g++ -std=c++11 -pthread sepia.o bitmap.o main.o -o ParellelFiltering.out

sepia.o : ../filters/def.h ../filters/sepia.cpp
	g++ -std=c++11 -pthread -c ../filters/sepia.cpp -o sepia.o

bitmap.o : ../filters/def.h ../filters/bitmap.cpp
	g++ -std=c++11 -pthread -c ../filters/bitmap.cpp -o bitmap.o

main.o: ../filters/def.h main.cpp
	g++ -std=c++11 -pthread -c main.cpp  -o main.o

clean:
	rm *.o
	rm *.out
