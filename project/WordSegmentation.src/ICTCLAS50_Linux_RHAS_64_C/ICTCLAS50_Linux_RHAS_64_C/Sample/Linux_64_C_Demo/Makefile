ADDFile=Test_result.txt

all:test test1 test2 test3

test: cDemoOK.cpp ICTCLAS50.h
	 g++ -g cDemoOK.cpp  ./libICTCLAS50.so  -m64  -O3 -DOS_LINUX -o test

test1: cDemoOK1.cpp ICTCLAS50.h
	 g++ -g cDemoOK1.cpp  ./libICTCLAS50.so  -m64  -O3 -DOS_LINUX -o test1

test2: cDemoOK2.cpp ICTCLAS50.h
	 g++ -g cDemoOK2.cpp  ./libICTCLAS50.so  -m64  -O3 -DOS_LINUX -o test2

test3: cDemoOK3.cpp ICTCLAS50.h
	 g++ -g cDemoOK3.cpp  ./libICTCLAS50.so  -m64  -O3 -DOS_LINUX -o test3

.PHONY:clean
clean:
	rm test test1 test2 test3 $(ADDFile); clear


