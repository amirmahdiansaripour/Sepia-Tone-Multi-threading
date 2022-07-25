
all: ParellelFiltering.out parallel-main.o ../filters/sepia.o ../filters/bitmap.o

ParellelFiltering.out: sepia.o bitmap.o parallel-main.o
	g++ -std=c++11 -pthread ../filters/sepia.o ../filters/bitmap.o parallel-main.o -o ParellelFiltering.out

sepia.o : ../filters/def.h ../filters/sepia.cpp
	g++ -std=c++11 -pthread -c ../filters/sepia.cpp -o ../filters/sepia.o

bitmap.o : ../filters/def.h ../filters/bitmap.cpp
	g++ -std=c++11 -pthread -c ../filters/bitmap.cpp -o ../filters/bitmap.o

parallel-main.o: ../filters/def.h parallel-main.cpp
	g++ -std=c++11 -pthread -c parallel-main.cpp  -o parallel-main.o

clean:
	rm *.o
	rm *.out
