#Author : Ghanshyam

###### Native compiler params #####
CC = clang++
CCFLAGS = -std=c++11 -w

###### Source Files #####
CPP = main.cpp
CPP += Common/PracticalSocket.cpp

###### Include Files #####
INCLUDES = -I Common/

###### Executable Names #####
TARGET_OSX = iot_end_dev
TARGET_RPI = iot_end_dev_rpi


###### RPI2 compiler params #####
CC_RPI = /usr/local/linaro/arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++ 
CCFLAGS_RPI = -std=c++11
#LIBS_RPI = -l /usr/local/linaro/arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.9.1/thread

###### Libraries #####
INCLUDES +=
LIBS = -pthread 

all: $(TARGET_OSX)

$(TARGET_OSX): 
	$(CC) $(CCFLAGS) $(CPP) $(INCLUDES) -o $(TARGET_OSX) $(LIBS)  

rpi: $(TARGET_RPI)

$(TARGET_RPI): 
	$(CC_RPI) $(CCFLAGS_RPI) $(CPP) $(INCLUDES) -o $(TARGET_RPI) $(LIBS_RPI) -pthread -lrt

clean:
	rm -f *.o $(TARGET_OSX) $(TARGET_RPI)
	