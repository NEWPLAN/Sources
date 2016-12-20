TARGET1=server
TARGET2=client

PARAS=-pthread

all:$(TARGET1) $(TARGET2) 

Cfile1=server.c
Cfile2=client.c

$(TARGET1):$(Cfile1)
	gcc  $(Cfile1) -o $(TARGET1) $(PARAS)
	
$(TARGET2):$(Cfile2)
	gcc $(Cfile2) -o $(TARGET2) $(PARAS)

clean:
	rm $(TARGET1) $(TARGET2) & clear
