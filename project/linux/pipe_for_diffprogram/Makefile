TARGET1=get
TARGET2=send

all:$(TARGET1) $(TARGET2) 

Cfile1=get.c
Cfile2=send.c

$(TARGET1):$(Cfile1)
	gcc $(Cfile1) -o $(TARGET1)
	
$(TARGET2):$(Cfile2)
	gcc $(Cfile2) -o $(TARGET2)

clean:
	rm $(TARGET1) $(TARGET2) & clear
