#!/bin/sh
# makefile written by newplan
SHELL = /bin/bash
TARGET=ictclas.a
TARGET2=libictclas.so
DEBUG=-g
SHARElib=-shared
INDEPENDE=-fPIC
OPTIMIZATION=-O3
COPY=cp
#WERROR=-Wall
Libs= ContextStat.o Dictionary.o DynamicArray.o NShortPath.o Queue.o Result.o \
		SegGraph.o Segment.o Span.o UnknowWord.o Utility.o
Cfile1= ContextStat.cpp Dictionary.cpp DynamicArray.cpp NShortPath.cpp Queue.cpp \
		Result.cpp SegGraph.cpp Segment.cpp Span.cpp UnknowWord.cpp Utility.cpp
HeadFiles= ContextStat.h Dictionary.h DynamicArray.h NShortPath.h Queue.h \
		Result.h SegGraph.h Segment.h Span.h UnknowWord.h Utility.h
CPP=g++
CC=gcc
AR=ar
LD=ld

all:$(TARGET) $(TARGET2)

$(TARGET): $(Libs)
	$(AR) -r $(TARGET) $(Libs) 
	@echo "create static libs done!"

$(TARGET2):$(Libs)
	$(LD) $(SHARElib) $(Libs) -o $(TARGET2)
	@echo "create dynamic libs done!"

%.o:%.cpp $(HeadFiles)
	$(CPP) $(INDEPENDE) $(DEBUG) $(WERROR) $(OPTIMIZATION) -c $< -o $@ 

install:$(TARGET2)
	$(COPY) $(TARGET2) ./../

uninstall:
	-rm ./../$(TARGET2)
	@echo "uninstall done"
	


.PHONY:clean
clean:
	rm $(Libs) $(TARGET) $(TARGET2) ; clear
	@echo -e "\033[31m\033[1m clean done! \033[0m"
