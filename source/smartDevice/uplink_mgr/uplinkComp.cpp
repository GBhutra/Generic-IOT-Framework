/***********************************************************************/
/*		Project 	   	 : CSCE 622 Co-Design 
/*		Component	   	 : Uplink Component
/*		File		       : uplinkComp.cpp
/*		Description	 	 : This file defines the things class which should be inherited by a 
/*		Author		   	 : Ghanshyam
/*		Date		       : 4th December
/***********************************************************************/

#include "uplinkComp.h"

uint16_t UP_convert64to16(uint64_t num)  {
  uint16_t n = 34;
  char* it = reinterpret_cast<char*>(&num);
  std::memcpy(&n,it+6,2);
  return n;
}

uint64_t UP_convert16to64(uint16_t num)  {
  uint64_t n = 0;
  char* it = reinterpret_cast<char*>(&n);
  uint32_t num1=END_DEV_ID_CONST_EXT_4BYTE_1;
  uint16_t num2=END_DEV_ID_CONST_EXT_2BYTE_2;
  std::memcpy(it,&num1,4);
  std::memcpy(it+4,&num2,2);
  std::memcpy(it+6,&num,2);
  return n;
}

STATUS CUplinkComp::storeMessageInFromQueue(Message m)    {
  if (m_QFromServer.size()<MAX_UPLINK_QUEUE_SIZE)   {
    m_QFromServer.push(m);
    DBG_LOG("Message:"<<m.val<<" added to queue!!");
    return SUCCESS;
  }
  else
    return ERROR;
}

STATUS CUplinkComp::storeMessageInToQueue(Message msg)  {
  if (m_QToServer.size()<MAX_UPLINK_QUEUE_SIZE)   {
    m_QToServer.push(msg);
  return SUCCESS;
  }
  else
    return ERROR;
}

CUplinkComp::~CUplinkComp()	{
	DBG_LOG("Destructor of UplinkComp() for ThingID :");
}

uint16_t CUplinkComp::ConvertMsgToServerBytes(char* buf,Message& m) {
  
  uint16_t size = (SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID+SDX_DS_BYTES_FOR_DEV_ID+m.val.length()+1);
  uint16_t msgID = m.ID;
  //uint64_t DevId = 3472328296227681584;
  uint16_t DeviceId = m_ptrThing->getID();
  uint64_t DevId = UP_convert16to64(DeviceId);

  DBG_LOG_BLUE("Size="<<size<<"\tmsgID="<<msgID<<"\tDevID="<<DevId<<"\tValue:"<<m.val);
  DBG_LOG_RED("Size="<<sizeof(size)<<"\tmsgID="<<sizeof(msgID)<<"\tDevID="<<sizeof(DevId)<<"\tValue="<<m.val.length());
  std::bitset<16> b1(size);
  DBG_LOG_GREEN("BitPattern size\t:"<<b1)
  std::bitset<16> b2(msgID);
  DBG_LOG_GREEN("BitPattern msgID\t:"<<b2)
  std::bitset<64> b3(DevId);
  DBG_LOG_GREEN("BitPattern DevID\t:"<<b3)

  uint16_t ssize  = htons(size);
  uint16_t smsgID = htons(msgID);
  //uint64_t sDevId = htonll(DevId);
  uint64_t sDevId = htobe64(DevId);

  DBG_LOG_BLUE("Size="<<ssize<<"\tmsgID="<<smsgID<<"\tDevID="<<sDevId<<"\tValue:"<<m.val);
  DBG_LOG_RED("Size="<<sizeof(size)<<"\tmsgID="<<sizeof(msgID)<<"\tDevID="<<sizeof(DevId)<<"\tValue="<<m.val.length());
  std::bitset<16> b4(ssize);
  DBG_LOG_GREEN("BitPattern size\t:"<<b4)
  std::bitset<16> b5(smsgID);
  DBG_LOG_GREEN("BitPattern msgID\t:"<<b5)
  std::bitset<64> b6(sDevId);
  DBG_LOG_GREEN("BitPattern DevID\t:"<<b6)

  //Size
  std::memcpy(buf,&ssize,SDX_DS_BYTES_FOR_SIZE);
  //MSG_ID
  std::memcpy(buf+SDX_DS_BYTES_FOR_SIZE,&smsgID,SDX_DS_BYTES_FOR_MSG_ID);
  //DEVID
  std::memcpy(buf+SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID,&sDevId,SDX_DS_BYTES_FOR_DEV_ID);
  //Payload
  strncpy((buf+(SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID+SDX_DS_BYTES_FOR_DEV_ID)),m.val.c_str(),m.val.length());
  
  buf[SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID+SDX_DS_BYTES_FOR_DEV_ID+m.val.length()] = '\n';
  return size;
}

void CUplinkComp::StartTCPClient()  {
  DBG_LOG("Starting TCPClient !!")
  char cBuffer[IOT_MAX_MSG_SIZE];
  try {
    TCPSocket sock(DEVICE_SERVER_IP_ADDRESS,DEVICE_SERVER_PORT_NO);
  	DBG_LOG("Socket Created !! About to send registration packet!");
    Message reg{PK_SESSION_CREATE_END_DEV_REQ,"Please Register !"};
    bzero(cBuffer, IOT_MAX_MSG_SIZE);
    uint16_t len=ConvertMsgToServerBytes(cBuffer,reg); 
    sock.send(cBuffer, len);
    bzero(cBuffer, IOT_MAX_MSG_SIZE);
    uint16_t iBytesRcvd = 0,iTotalBytesRcvd=0,iBytesExpected = 2;              // Bytes read on each recv()
    while (iTotalBytesRcvd < IOT_MAX_MSG_SIZE && iBytesExpected>iTotalBytesRcvd) {
      if ((iBytesRcvd = (sock.recv(cBuffer+iTotalBytesRcvd, IOT_MAX_MSG_SIZE))) <= 0) {
        DBG_ERROR("Unable to read");
      }
      iTotalBytesRcvd += iBytesRcvd;
      DBG_LOG_YELLOW("No of Bytes received="<<iBytesRcvd);
      if (2<iTotalBytesRcvd && 2==iBytesExpected) {
        getSizeFromString(cBuffer,iBytesExpected);
      }
      DBG_LOG_YELLOW("No of Bytes Expected="<<iBytesExpected);
    }
    if (0!=iBytesExpected)  {
      DBG_LOG_RED("Message Received Size="<<iBytesExpected);
      Message M;
      getMessageFromString(cBuffer,M);
      uint16_t thingId;
      getDevIDFromString(cBuffer,thingId);
      if (PK_SESSION_CREATE_END_DEV_RES==M.ID && "ACK"==M.val && m_ptrThing->getID()==thingId)  {
        m_bConnectionStatus = true;
        DBG_LOG_GREEN("Session Established !!");
        while (true)  {
          if (!m_QToServer.empty())   {
            DBG_LOG("Found a message in m_QToServer!! m_QToServer.size="<<m_QToServer.size());   
            Message m = m_QToServer.front(); 
            DBG_LOG("Sending Message ID:"<<m.ID<<"\t val"<<m.val);
            bzero(cBuffer, IOT_MAX_MSG_SIZE);
            uint16_t len;
            len=ConvertMsgToServerBytes(cBuffer,m); 
            sock.send(cBuffer, len);
            m_QToServer.pop();
            DBG_LOG("Length of message sent:"<<len);
          }
          else {
            DBG_LOG("No messages in m_QToServer!!");   
          }
          bzero(cBuffer, IOT_MAX_MSG_SIZE);
          uint16_t bytesReceived = 0;              // Bytes read on each recv()
          uint16_t totalBytesReceived = 0;         // Total bytes read
          uint16_t noBytesExpected = 2;
          while (totalBytesReceived < IOT_MAX_MSG_SIZE && noBytesExpected>totalBytesReceived) {
            if ((bytesReceived = (sock.recv_withTimeOut(cBuffer+totalBytesReceived, IOT_MAX_MSG_SIZE,15000))) <= 0) {
              DBG_ERROR("Unable to read");
            }
            totalBytesReceived += bytesReceived;
            DBG_LOG_YELLOW("No of Bytes received="<<bytesReceived);
            if (2<totalBytesReceived && 2==noBytesExpected) {
              getSizeFromString(cBuffer,noBytesExpected);
            }
            DBG_LOG_YELLOW("No of Bytes Expected="<<noBytesExpected);
          }
          if (0!=noBytesExpected)  {
            DBG_LOG_RED("Message Received Size="<<iBytesExpected);
            Message M;
            getMessageFromString(cBuffer,M);   
            //storeMessageInFromQueue(M);
            m_ptrThing->HandleNotification(M); 
            //DBG_LOG_RED("Size of the Queue="<<m_QFromServer.size());
          }
        }
      }
      else 
        DBG_ERROR("Invalid Packet Received. Session creation failed!!");
        DBG_ERROR("ID="<<M.ID<<"\tVal="<<M.val<<"\tthingID"<<thingId);
    }
  }
  catch(SocketException &e) {
    cerr << e.what() << endl;
    DBG_ERROR("Connection Terminated!");
  }
}

std::thread CUplinkComp::TCPClient ()  {
  return std::thread([=]{ StartTCPClient();  });
}

void CUplinkComp::getSizeFromString(char* str, uint16_t& size) {
  std::memcpy(&size,str,SDX_DS_BYTES_FOR_SIZE);
    size = ntohs(size);
}

void CUplinkComp::getDevIDFromString(char* str, uint16_t& ids)  {
  uint64_t id;
  std::memcpy(&id,str+SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID,SDX_DS_BYTES_FOR_DEV_ID);
  id = be64toh(id);
  ids = UP_convert64to16(id);
}

void CUplinkComp::getMessageFromString(char* str,Message& m) {
  std::memcpy(&m.ID,str+SDX_DS_BYTES_FOR_SIZE,SDX_DS_BYTES_FOR_MSG_ID);
  m.ID = ntohs(m.ID);
  uint16_t size; 
  getSizeFromString(str,size);
  m.val.assign(str+SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID+SDX_DS_BYTES_FOR_DEV_ID,size-(SDX_DS_BYTES_FOR_SIZE+SDX_DS_BYTES_FOR_MSG_ID+SDX_DS_BYTES_FOR_DEV_ID));
}

bool CUplinkComp::getConnecttionStatus()  {
  return m_bConnectionStatus;
}

