/***********************************************************************/
/*    Project     : CSCE 622 Co-Design 
/*    Component   : Uplink manager
/*    File        : uplink_mgr.cpp
/*    Description : Implements a TCP Server connection class is used by the things Class to send data to Server
/*    Author      : Ghanshyam
/*    Date        : 3rd November
/***********************************************************************/

#include "uplinkComp.h"
#include <cstdint>
#include <bitset>

void AddMessages(CUplinkComp* up)	{
	DBG_LOG("Startint the thread for writing messages !! :D");
	for (int i=0;i<10;i++)	{
		sleep(5);
		Message M;
		M.ID = i;
		M.val = "This is MessageNo";
		up->storeMessageInToQueue(M);
	}
}

uint16_t ConvertToServerFormat(char* buf,Message& m)    {
  uint16_t size = (BYTES_FOR_SIZE+BYTES_FOR_MSG_ID+BYTES_FOR_DEV_ID+m.val.length()+1);
  uint16_t msgID = m.ID;
  //uint64_t DevId = 3472328296227681584;
  uint64_t DevId = m_ptrThing->getID()
  DBG_LOG_BLUE("Size="<<size<<"\tmsgID="<<msgID<<"\tDevID="<<DevId<<"\tValue:"<<m.val);
  DBG_LOG_RED("Size="<<sizeof(size)<<"\tmsgID="<<sizeof(msgID)<<"\tDevID="<<sizeof(DevId)<<"\tValue="<<m.val.length());
  std::bitset<16> b1(size);
  DBG_LOG_GREEN("BitPattern size\t:"<<b1)
  std::bitset<16> b2(msgID);
  DBG_LOG_GREEN("BitPattern msgID\t:"<<b2)
  std::bitset<64> b3(DevId);
  DBG_LOG_GREEN("BitPattern DevID\t:"<<b3)

  int16_t ssize  = htons(size);
  int16_t smsgID = htons(msgID);
  int64_t sDevId = htonll(DevId);

  DBG_LOG_BLUE("Size="<<ssize<<"\tmsgID="<<smsgID<<"\tDevID="<<sDevId<<"\tValue:"<<m.val);
  DBG_LOG_RED("Size="<<sizeof(size)<<"\tmsgID="<<sizeof(msgID)<<"\tDevID="<<sizeof(DevId)<<"\tValue="<<m.val.length());
  std::bitset<16> b4(ssize);
  DBG_LOG_GREEN("BitPattern size\t:"<<b4)
  std::bitset<16> b5(smsgID);
  DBG_LOG_GREEN("BitPattern msgID\t:"<<b5)
  std::bitset<64> b6(sDevId);
  DBG_LOG_GREEN("BitPattern DevID\t:"<<b6)
  //Size
  std::memcpy(buf,&size,BYTES_FOR_SIZE);
  //MSG_ID
  std::memcpy(buf+BYTES_FOR_SIZE,&m.ID,BYTES_FOR_MSG_ID);
  //DEVID
  std::memcpy(buf+BYTES_FOR_SIZE+BYTES_FOR_MSG_ID,&m.ID,BYTES_FOR_DEV_ID);
  //Payload
  strncpy((buf+(BYTES_FOR_SIZE+BYTES_FOR_MSG_ID+BYTES_FOR_DEV_ID)),m.val.c_str(),m.val.length());
  //std::memcpy(buf+BYTES_FOR_SIZE+BYTES_FOR_MSG_ID+BYTES_FOR_DEV_ID,m.val.c_str(),m.val.length());

  buf[BYTES_FOR_SIZE+BYTES_FOR_MSG_ID+BYTES_FOR_DEV_ID+m.val.length()] = '\n';

  char* it;
  it = buf+BYTES_FOR_SIZE+BYTES_FOR_MSG_ID+BYTES_FOR_DEV_ID;
  for(int i=0;i<m.val.length();i++) {
    std::cout<<*it;
    it++;
  }
  std::cout<<std::endl;
  return size;
}


int main()  {
  std::cout<<"Hello world"<<std::endl;
  CUplinkComp* uplink = new CUplinkComp();
  Message M{12341,"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345"};
  Message M1{12341,"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456"};
  Message M2{12341,"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567"};
  Message M3{12341,"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678"};
  Message M4{12341,"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"};
  uplink->storeMessageInToQueue(M);
  uplink->storeMessageInToQueue(M1);
  uplink->storeMessageInToQueue(M2);
  uplink->storeMessageInToQueue(M3);
  uplink->storeMessageInToQueue(M4);
  std::thread tcpC = uplink->TCPClient();
  //std::thread msg (AddMessages,uplink);
  if (tcpC.joinable())
  	tcpC.join();
  DBG_LOG("TCPClient started!!")
}

