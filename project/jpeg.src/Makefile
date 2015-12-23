TARGET=exif_test
TARGET2=test
TARGET3=dis_test

LINK=-lexif

CFILE=exif_test.c
CFILE2=test.c
CFILE3=test3.c

all:$(TARGET) $(TARGET2) $(TARGET3)

$(TARGET):$(CFILE)
	gcc $(CFILE) -o $(TARGET) $(LINK)
$(TARGET2):$(CFILE2)
	gcc $(CFILE2) -o $(TARGET2) $(LINK)
$(TARGET3):$(CFILE3)
	gcc $(CFILE3) -o $(TARGET3) $(LINK)
clean:
	rm $(TARGET) $(TARGET2) $(TARGET3) 1.jpg write-exif.jpg & clear
