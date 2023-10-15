GCC = g++
FLAGS = --std=c++11 -DNDEBUG -O3 $(shell pkg-config --cflags --libs /usr/local/Cellar/opencv@2/2.4.13.7_7/lib/pkgconfig/opencv.pc)

Main.out: RemoveBackground.o
	$(GCC) $(FLAGS) -o Main.out RemoveBackground.o

RemoveBackground.o: RemoveBackground.cpp
	$(GCC) -c $(FLAGS) -o RemoveBackground.o RemoveBackground.cpp

clean:
	rm *.o *.out