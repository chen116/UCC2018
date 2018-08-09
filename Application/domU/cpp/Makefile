

all: vid_proc motion

vid_proc: clean
	gcc -c comm.c -lxenstore
	g++ -c pacer.cpp -lhb-shared 
	g++ -c vid_proc.cpp  `pkg-config --libs opencv` -std=c++11 
	g++ comm.o pacer.o vid_proc.o -o vid_proc -lhb-shared -lxenstore  `pkg-config --libs opencv` -std=c++11

example: 
	gcc -c comm.c -lxenstore
	g++ -c pacer.cpp -lhb-shared 
	g++ -c example.cpp 
	g++ comm.o pacer.o example.o -o example -lhb-shared -lxenstore


motion: clean 
	gcc -c comm.c -lxenstore
	g++ -c pacer.cpp -lhb-shared 
	g++ -c motion.cpp  `pkg-config --libs opencv` -std=c++11 
	g++ comm.o pacer.o motion.o -o motion -lhb-shared -lxenstore  `pkg-config --libs opencv` -std=c++11


clean:
	-rm -f vid_proc motion example *.o 
