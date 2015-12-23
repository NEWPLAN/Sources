# makefile written by newplan
TARGET1=chinese.seg.txt
Cfile1=main.cpp
DEBUG=-g
IuputFiles=chinese.raw.txt
TARGET2=ictclas
LIBNeeds=Source/ictclas.a
CPP=g++
CC=gcc
NeedsFile=chinese.raw.txt


all:$(TARGET2)

test: $(NeedsFile) $(TARGET2)
	./$(TARGET2) < $(NeedsFile) > $(TARGET1)

$(TARGET2): $(Cfile1) $(LIBNeeds)
	$(CPP) $(DEBUG) -o $(TARGET2) $(Cfile1) $(LIBNeeds)

$(LIBNeeds):
	cd Source ; make

install:
	cd Source; make install

uninstall:
	cd Source; make uninstall
	make clean

.PHONY:clean
clean:
#	cd Source; make clean
	rm $(TARGET1) $(TARGET2) ;  echo "clean done!"
