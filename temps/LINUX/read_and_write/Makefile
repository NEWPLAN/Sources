TARGET1=copy_with_fread
TARGET2=copy_with_read

all:$(TARGET1) $(TARGET2) 

Cfile1=copy_with_fread.c
Cfile2=copy_with_fread.c

$(TARGET1):$(Cfile1)
	gcc $(Cfile1) -o $(TARGET1)
	
$(TARGET2):$(Cfile2)
	gcc $(Cfile2) -o $(TARGET2)

clean:
	rm $(TARGET1) $(TARGET2) & clear
