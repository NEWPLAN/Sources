ADDFile=Test_result.txt
all:test

test: cDemoOK.cpp NLPIR.h
	 g++ -g cDemoOK.cpp  ./libNLPIR.so  -m64  -O3 -DOS_LINUX -o test

.PHONY:clean
clean:
	rm test $(ADDFile); clear


