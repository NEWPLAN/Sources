# script for compiling the RSA.
# 
TARGET=rsa.exe
SRC=BigInt.cpp Rsa.cpp main.cpp
HEAD=BigInt.h Rsa.h
OBJS=BigInt.o Rsa.o main.o
CPP=g++
CPPFLAGS+= -Wall

all:$(TARGET)
	@echo "make done"


$(TARGET):$(OBJS)
	$(CPP) $^ -o $@
.PHONY:clean

%.o:%.cpp $(HEAD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGET) $(OBJS)
	@echo "all is clean"
