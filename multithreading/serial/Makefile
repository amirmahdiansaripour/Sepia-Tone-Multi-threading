all: SerialFiltering.out serial-main.o ../filters/bitmap.o ../filters/sepia.o

SerialFiltering.out: serial-main.o bitmap.o sepia.o
	g++ -std=c++11 serial-main.o ../filters/bitmap.o ../filters/sepia.o -o SerialFiltering.out

serial-main.o: ../filters/def.h serial-main.cpp
	g++ -std=c++11 -c serial-main.cpp  -o serial-main.o

bitmap.o: ../filters/def.h ../filters/bitmap.cpp
	g++ -std=c++11 -c ../filters/bitmap.cpp -o ../filters/bitmap.o

sepia.o : ../filters/def.h ../filters/sepia.cpp
	g++ -std=c++11 -c ../filters/sepia.cpp -o ../filters/sepia.o

clean:
	rm *.o
	rm *.out
