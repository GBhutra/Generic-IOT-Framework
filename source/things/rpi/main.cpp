#include <iostream>    
#include <chrono>
#include <thread>       
#include <ctime>     
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include "iot_common_dx.h"

int main(int argc, char *argv[])	{
	if (argc != 4) {   // Test for correct number of arguments
    	cerr << "Usage: " << argv[0] << "<Server><ServerPort><DeviceID><GpIOPinNum>\n";
    	exit(1);
	}
	uint16_t iPortNo = atoi(argv[2]);
	uint16_t iDeviceID = atoi(argv[3]);
	std::string servIPAddress(argv[1]); 
  DBG_LOG_GREEN("DeviceID:"<<iDeviceID<<"\tPortNo:"<<iPortNo<<"\tIPAddress:"<<servIPAddress);
 
  STATE_T state = INIT;
  UDPSocket sock;

  while(true) {
    switch(state) {
      case INIT:
      {
        DBG_LOG("-------INIT STATE START-------");
        try {
          char buffer[IOT_UDP_MAX_DATA_LENGTH],resBuffer[IOT_UDP_MAX_DATA_LENGTH];
          Message reg{END_DEV_REG_REQ,"please register"};
          convertMessageToBytes(reg,buffer);
          putDeviceIdInMsg(iDeviceID,buffer);
          DBG_LOG(" Registration request sent "<<buffer);
          sock.sendTo(buffer, IOT_UDP_MAX_DATA_LENGTH, servIPAddress, iPortNo);
          uint16_t devID;
          Message test;
          getDeviceIdInMsg(devID,buffer);
          convertBytesToMessage(test,buffer);
          DBG_LOG("DeviceID="<<devID);
          DBG_LOG("MsgID="<<test.ID<<" value"<<test.val);
          bzero(resBuffer,IOT_UDP_MAX_DATA_LENGTH);
          if ( -1 == sock.recv_withTimeOut(resBuffer, IOT_UDP_MAX_DATA_LENGTH,RESPONSE_TIMER_EXP_MSEC)) {
            DBG_ERROR("Registration Timer Expired!! going to suspend state !");
            state = SUSPEND;
          }
          else {
            uint16_t retDevID;
            getDeviceIdInMsg(retDevID,resBuffer);
            if (retDevID==iDeviceID)  {
              Message reg_res;
              convertBytesToMessage(reg_res,resBuffer);
              if (END_DEV_REG_RES==reg_res.ID)  {
                DBG_LOG("End device registration successfull!! Moving to Active State");
                state = ACTIVE;
              }
              else  
                DBG_LOG("MSG ID invalid"<<reg_res.ID);
            }
            else  {
              DBG_LOG("DeviceID mismatch"<<retDevID);
              state = SUSPEND;
            }
          }
        } catch (SocketException &e) {
          cerr << e.what() << endl;
          state = SUSPEND;
        }
        DBG_LOG("-------INIT STATE END  -------");
      }
      break;
      case ACTIVE:
      {
        DBG_LOG("-------ACTIVE STATE START  -------");
        //Read Sensor Value 
        std::clock_t start = std::clock();
        char buffer[IOT_UDP_MAX_DATA_LENGTH],resBuffer[IOT_UDP_MAX_DATA_LENGTH];
        Message value{END_DEV_VALUE_UPDT_REQ,"Value Update"};
        convertMessageToBytes(value,buffer);
        putDeviceIdInMsg(iDeviceID,buffer);
        DBG_LOG(" Value Update Request Sent "<<buffer);
        sock.sendTo(buffer, IOT_UDP_MAX_DATA_LENGTH, servIPAddress, iPortNo);
        bzero(resBuffer,IOT_UDP_MAX_DATA_LENGTH);
        if ( -1 == sock.recv_withTimeOut(resBuffer, IOT_UDP_MAX_DATA_LENGTH,RESPONSE_TIMER_EXP_MSEC)) {
          DBG_ERROR("Response Timer Expired!! going to suspend state !");
          state = SUSPEND;
        }
        // acknowledge response!!
        while (READ_SENSOR_VALUE_SEC>( std::clock() - start ) / (double) CLOCKS_PER_SEC);
        DBG_LOG("-------ACTIVE STATE END  -------");
      }
      break;
      case SUSPEND:
      {
        DBG_LOG("-------SUSPEND STATE START  -------");
        std::clock_t start = std::clock();
        while (SUSPEND_STATE_TIMEOUT_SEC>( std::clock() - start ) / (double) CLOCKS_PER_SEC);
        state = INIT;
        DBG_LOG("-------SUSPEND STATE END  -------");
      }
      break;
      default:
        DBG_ERROR("-------INVALID STATE START  -------");
      break;
    }
  }
}
