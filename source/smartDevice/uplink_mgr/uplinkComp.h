/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: Uplink component
/*		File		: uplinkComp.h
/*		Description	: Implements a TCP client connection class is used by the things Class to send data to Server
/*		Author		: Ghanshyam
/*		Date		: 3rd November
/***********************************************************************/

#ifndef uplink_mgr_h
#define uplink_mgr_h

#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <bitset>
#include <arpa/inet.h>
#include "PracticalSocket.h"
#include "messages.h"
#include "utility.h"
#include "things.h"

class CThings;

class  CUplinkComp  {
public:
    friend uint64_t UP_convert16to64(uint16_t num);
    friend uint16_t UP_convert64to16(uint64_t num);
    CUplinkComp() = default;
    CUplinkComp(CThings* thingPointer) : m_ptrThing(thingPointer) {
        DBG_LOG("In the parameterized constructor of the CuplinkComp");
        m_bConnectionStatus = false;
    };
    STATUS storeMessageInFromQueue(Message);
    STATUS storeMessageInToQueue(Message);
    std::thread TCPClient();
    uint16_t ConvertMsgToServerBytes(char* buf,Message& m);
    bool getConnecttionStatus();
    ~CUplinkComp();
private:
    void getMessageFromString(char*,Message&);
    void getSizeFromString(char*,uint16_t&);
    void getDevIDFromString(char*,uint16_t&);
    void StartTCPClient();
    std::queue<Message> m_QToServer;
    std::queue<Message> m_QFromServer;
    bool m_bConnectionStatus;
    uint16_t m_iPortNo;
    CThings* m_ptrThing;
};

#endif
