#Author : Ghanshyam

###### Native compiler params #####
CC = clang++
CCFLAGS = -g -std=c++11 

###### Source Files #####
CPP = uplink_mgr.cpp
CPP += ../things/things.cpp
CPP += uplinkComp.cpp
CPP += ../common/PracticalSocket.cpp

###### Include Files #####
INCLUDES = -I ../common/ -I ../things/


###### Executable Names #####
TARGET_OSX = uplink_mgr
TARGET_RPI = uplink_mgr_rpi.so


###### RPI2 compiler params #####
CC_RPI = /usr/local/linaro/arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++ -g
CCFLAGS_RPI = -std=c++0x -Wall
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
	