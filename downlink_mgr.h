/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: Downlink manager
/*		File		: downlink_mgr.h
/*		Description	: Implements a udp/Serial port/BLE connections and takes collects the events in the end device into a queue
/*		Author		: Ghanshyam
/*		Date		: 23rd October
/***********************************************************************/

#ifndef downlink_mgr_h
#define downlink_mgr_h

#include <iostream>
#include <cstdlib>
#include <thread>
#include <mqueue.h>
#include <stdlib.h>
#include <sstream>
#include <errno.h>
#include <bitset>
#include <arpa/inet.h>
#include "utility.h"
#include "PracticalSocket.h"



class CDownlinkComp	{
public:
	CDownlinkComp(std::string mqU,std::string mqD) : m_strUQName(mqU), m_strDQName(mqD) {};
	~CDownlinkComp();
	virtual void StartService() = 0;
protected:
	STATUS CreateQueues();
	STATUS WriteToQueue(std::string);
  bool m_bIsQCreated = false;
	mqd_t mqUP;
  mqd_t mqDown;
	const std::string m_strUQName;
  const std::string m_strDQName;
};

STATUS CDownlinkComp::CreateQueues()	{
	DBG_LOG_BLUE("CreateQueues : "<<m_strUQName<<","<<m_strDQName);
	if(m_strUQName.length()==0 || m_strDQName.length()==0)	{
		return ERROR;
	}
	struct mq_attr attrU;
  	attrU.mq_flags = 0;
  	attrU.mq_maxmsg = 10;
  	attrU.mq_msgsize = END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+2;
  	attrU.mq_curmsgs = 0;
  	mqUP = mq_open(m_strUQName.c_str(), O_CREAT | O_RDWR, 0666, &attrU);
  	if((mqd_t)-1 == mqUP)  {
   		DBG_ERROR(stderr<<"UpQueue Error :"<<mqUP)
   		return ERROR;
  	}
   DBG_LOG_BLUE("CreateQueues UpQueue Created Successfully!!"); 
  struct mq_attr attrD;
    attrD.mq_flags = 0;
    attrD.mq_maxmsg = 10;
    attrD.mq_msgsize = END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+2;
    attrD.mq_curmsgs = 0;
    mqDown = mq_open(m_strDQName.c_str(), O_CREAT | O_RDWR, 0666, &attrD);
    if((mqd_t)-1 == mqDown)  {
      DBG_ERROR(stderr<<"Down Queue Error:"<<mqDown)
      return ERROR;
    }
    DBG_LOG_BLUE("CreateQueues DownQueue Created Successfully!!");
  	m_bIsQCreated = true;
  	return SUCCESS;
}

STATUS CDownlinkComp::WriteToQueue(std::string msg)	{
	DBG_LOG_BLUE("WriteToQueue message:"<<msg);
  mqUP = mq_open(m_strUQName.c_str(), O_WRONLY);
  if ((mqd_t)-1 == mqUP)	{
    DBG_ERROR("Message queue open ERROR! "<<stderr<<" "<<mqUP);
  	return ERROR;
  }
  DBG_LOG("message into the queue is:"<<msg<<"\tSize:"<<msg.length());
  int status = mq_send(mqUP,msg.c_str(),msg.length()+1,1);
  if (0 > status)	{
    DBG_ERROR("Message queue send ERROR!"<<errno<<" Error:"<<strerror(errno));
  	return ERROR;
  }	
  return SUCCESS;
}

CDownlinkComp::~CDownlinkComp()	{
	DBG_LOG_BLUE("Destructor CDownlinkComp");
}

class CDownlink_UDP	: public CDownlinkComp	{
public:
	CDownlink_UDP(std::string mqU,std::string mqD,uint16_t portNo) : CDownlinkComp(mqU,mqD), m_iPortNo(portNo) {};
	CDownlink_UDP() = delete;
	~CDownlink_UDP();
	void StartService();
  STATUS ReadFromQueueToBuf();
private:
	uint16_t m_iPortNo;
  char m_Cbuffer[END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID];
};

CDownlink_UDP::~CDownlink_UDP()	{
	DBG_LOG_BLUE("CDownlink_UDP :"<<m_iPortNo);
}

STATUS CDownlink_UDP::ReadFromQueueToBuf() {
  mqDown = mq_open(m_strDQName.c_str(), O_RDWR);
  if ((mqd_t)-1 == mqDown)  {
    DBG_ERROR(stderr<<"MessageQ Read Error "<<mqDown);
    return ERROR;
  }
  bzero(m_Cbuffer,END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID);
  ssize_t bytes_read;
  bytes_read = mq_receive(mqDown,m_Cbuffer, END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID, NULL);
  CHECK(bytes_read >= 0);
  return SUCCESS;
}

void CDownlink_UDP::StartService()	{
	try {
  	  UDPSocket sock(m_iPortNo);
  	  DBG_LOG("Starting service on portNo:"<<m_iPortNo);
  	  char message[END_DEVICE_PACKET_LENGTH];  
      STATUS st = CreateQueues();
      if (ERROR==st)
        DBG_ERROR("Error creating queues !!");
  	  int recvMsgSize;                  // Size of received message
      std::string clientAddress;
  	  uint16_t clientPort;        // Port of datagram source
  	  while(true) {  
        DBG_LOG_YELLOW("UDP Server Started !!");
        bzero(message,END_DEVICE_PACKET_LENGTH);
  	    recvMsgSize = sock.recvFrom(message, END_DEVICE_PACKET_LENGTH, clientAddress,clientPort);
  	    DBG_LOG_GREEN("Received from:"<<clientAddress<<",sourcePort:"<<clientPort<<"\tMessage:"<<message+4);
        uint16_t devID,MsgID;
        std::memcpy(&devID,message,2);
        std::memcpy(&MsgID,message+2,2);
        DBG_LOG("Device ID="<<devID<<"\tMessageID="<<MsgID);
        std::stringstream ss;
        ss<<clientPort<<" "<<clientAddress<<" "<<devID<<" "<<MsgID<<" "<<message+4;
        std::string str = ss.str();
        std::bitset<16> b1(clientPort);
        DBG_LOG_GREEN("BitPattern clientPort\t:"<<b1);
        if (ERROR == WriteToQueue(str))
        {  DBG_ERROR("Write to Queue failed !!"); }
        if (ERROR == ReadFromQueueToBuf())
        {  DBG_ERROR("Read from Queue failed !!"); }
        DBG_LOG("MessageFromQueue:"<<m_Cbuffer);
        clientAddress.clear();
        uint16_t portNo;
        std::string msg;
        EXTRACT_MSG(m_Cbuffer,clientAddress,portNo,devID,MsgID,msg);

        DBG_LOG_YELLOW("ClientAddress:"<<clientAddress<<"\tport:"<<clientPort);
        bzero(message,END_DEVICE_PACKET_LENGTH);
        std::memcpy(message,&devID,sizeof(devID));
        std::memcpy(message+2,&MsgID,sizeof(MsgID));
        strncpy(message+4,msg.c_str(),msg.length()+1);
  	    sock.sendTo(message, END_DEVICE_PACKET_LENGTH, clientAddress, clientPort);
  	  }
  	} catch (SocketException &e) {
  		DBG_ERROR(e.what());
  	}
}
#endif
