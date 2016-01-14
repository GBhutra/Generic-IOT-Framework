#Author : Ghanshyam

###### Native compiler params #####
CC = clang++
CCFLAGS = -std=c++14 -w

###### Source Files #####
CPP = main.cpp
CPP += things/things.cpp
CPP += uplink_mgr/uplinkComp.cpp
CPP += common/PracticalSocket.cpp
CPP += things_mgr/things_mgr.cpp

###### Include Files #####
INCLUDES = -I common/
INCLUDES += -I things_mgr/
INCLUDES += -I things/
INCLUDES += -I uplink_mgr/

###### Executable Names #####
TARGET_OSX = iot_CoDesign_prj_test
TARGET_RPI = iot_CoDesign_prj


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
	