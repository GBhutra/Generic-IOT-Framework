/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: Things manager
/*		File		: things_mgr.h
/*		Description	: Implments the functionality on reception of the messages. 
					  Keeps track of the things connected.
/*		Author		: Ghanshyam
/*		Date		: 28th October
/***********************************************************************/

#ifndef things_mgr_h
#define things_mgr_h

#include <iostream>
#include <cstdlib>
#include <thread>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <mqueue.h>
#include <sstream>	
#include <thread>				  
#include "utility.h"
#include "things.h"					  

/*
The following class Things manager is a singleton pattern class
This component is responsible for invoking the the handleEvent and HandleNotification function of the
the things object. It maintains the list of devices connected in the cluster.
*/
class CThingsMgr	{
public:
	friend uint64_t convert16to64(uint16_t num);
	friend uint16_t convert64to16(uint64_t num);
	friend void convertToMessageForDX(char* message,std::string ip, uint16_t port, uint16_t deviceId, uint16_t msgId, std::string msg);
	/*
	The default assignment operator and the copy constructor are explicitly deleted
	to maintain only one instance of the things manager throughout the lifecycle of the program.
	*/
	CThingsMgr(const CThingsMgr&) = delete;
	CThingsMgr& operator=(const CThingsMgr&) = delete;
	/*
	The things manager should not be instantiated more than once in a program hence singleton design pattern is followed
	to implement it the getInstance function the instance of the thingsManager can be accessed.
	 */
	static CThingsMgr* getInstance();
	void StartIOTServices();
	STATUS RegisterThing(CIPThing&);
	STATUS RegisterThing(CSerialThing&);
	STATUS RegisterThing(CBLEThing&);
	
	~CThingsMgr();
private:
	CThingsMgr()	{	DBG_LOG("In default constructor of the things manager\n");	}
	auto getThingConnType(uint16_t) -> ConnectionType;
	int getDevCountOnIPaddr(const std::string ipAddress);


	auto getIPThingById(uint16_t) -> CIPThing*;
	auto getSerialThingById(uint16_t) -> CSerialThing*;
	auto getBLEThingById(uint16_t) -> CBLEThing*;

	STATUS AddIPThingToList(CIPThing& newThing);
	STATUS AddSerialThingToList(CSerialThing& newThing);
	STATUS AddBLEThingToList(CBLEThing& newThing);

	STATUS WriteToQueue(std::string,const char*);

	void HandleIPQueues();
	STATUS CreateQueue(std::string);

	void StartCLI();
	void printAllConnectedDevices();

	static CThingsMgr* m_pInstance;
	std::map<uint16_t,CIPThing*> m_mapIPThings;
	std::map<uint16_t,CSerialThing*> m_mapSerialThings;
	std::map<uint16_t,CBLEThing*> m_mapBLEThings;
	std::vector<uint16_t> m_vecThingId;
};

#endif