/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: Downlink manager
/*		File		: downlink_mgr.cpp
/*		Description	: Implements a udp/Serial port/BLE connections and takes collects the events in the end device into a queue
/*		Author		: Ghanshyam
/*		Date		: 23rd October
/***********************************************************************/

#include "downlink_mgr.h"
int main()
{
	DBG_LOG_BLUE("########################################################\n");
  DBG_LOG_YELLOW("\tSoftware Name:\t"<<BOLD<<"CSCE-617 Co-Design Project IOT Infrastructure");
  DBG_LOG_YELLOW("\tComponent:\t"<<BOLD<<"DownLink Manager");
  DBG_LOG_YELLOW("\tVersion:\t"<<BOLD<<"Ver 1.0");
  DBG_LOG_YELLOW("\tCompile date:\t"<<BOLD<<__DATE__);
  DBG_LOG_YELLOW("\tCompile time:\t"<<BOLD<<__TIME__<<"\n");
  DBG_LOG_BLUE("########################################################");
  CDownlink_UDP stdCon(IP_UP_QUEUE_NAME,IP_DOWN_QUEUE_NAME,8080);
  std::thread th1(&CDownlink_UDP::StartService,&stdCon);
	if(th1.joinable())
		th1.join();
	return 0;
}
