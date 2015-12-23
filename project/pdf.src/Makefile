TARGET=pdftest

LINK=-lz

STD=-std=c99

CFILE=pdftest.c

all:$(TARGET)

$(TARGET):$(CFILE)
	gcc $(STD) $(CFILE) -o $(TARGET) $(LINK)

clean:
	rm $(TARGET) output2.txt & clear
