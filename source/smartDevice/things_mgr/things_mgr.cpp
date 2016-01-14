/***********************************************************************/
/*		Project 	  : CSCE 622 Co-Design 
/*		Component	  : Things manager
/*		File		    : things_mgr.cpp
/*		Description	: Implments the functionality on reception of the messages. 
                    Keeps track of the things connected.
/*		Author		  : Ghanshyam
/*		Date		    : 28th October
/***********************************************************************/

#include "things_mgr.h"

uint16_t convert64to16(uint64_t num)  {
  uint16_t n = 34;
  char* it = reinterpret_cast<char*>(&num);
  std::memcpy(&n,it+6,2);
  return n;
}

uint64_t convert16to64(uint16_t num)  {
  uint64_t n = 0;
  char* it = reinterpret_cast<char*>(&n);
  uint32_t num1=END_DEV_ID_CONST_EXT_4BYTE_1;
  uint16_t num2=END_DEV_ID_CONST_EXT_2BYTE_2;
  std::memcpy(it,&num1,4);
  std::memcpy(it+4,&num2,2);
  std::memcpy(it+6,&num,2);
  return n;
}

void convertToMessageForDX(char* message,std::string ip, uint16_t port, uint16_t deviceId, uint16_t msgId, std::string msg) {
  std::memcpy(message,&port,SDX_DX_BYTES_FOR_PORT_NO);\
  strncpy(message+SDX_DX_BYTES_FOR_PORT_NO,ip.c_str(),CHARS_IN_IPADDRESS);
  std::memcpy(message+SDX_DX_BYTES_FOR_PORT_NO+CHARS_IN_IPADDRESS, &deviceId, SDX_DX_BYTES_FOR_DEV_ID);\
  std::memcpy(message+SDX_DX_BYTES_FOR_PORT_NO+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID, &msgId, SDX_DX_BYTES_FOR_MSG_ID);\
  strncpy(message+SDX_DX_BYTES_FOR_PORT_NO+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID+SDX_DX_BYTES_FOR_MSG_ID ,msg.c_str(),END_DEVICE_PACKET_LENGTH-(SDX_DX_BYTES_FOR_DEV_ID+SDX_DX_BYTES_FOR_MSG_ID));
}

CThingsMgr* CThingsMgr::m_pInstance = NULL;

CThingsMgr* CThingsMgr::getInstance() {
  if (NULL==m_pInstance)  {
    m_pInstance = new CThingsMgr();
  }
  return m_pInstance;
}

CThingsMgr::~CThingsMgr() {
  if (NULL!=m_pInstance)
    delete m_pInstance;
}

STATUS CThingsMgr::RegisterThing(CIPThing& newThing)  {
  DBG_LOG("Newthing Registration invoked CIPThing ID:"<<newThing.getID());
  auto it = std::find(m_vecThingId.begin(),m_vecThingId.end(),newThing.getID());
  if (it == m_vecThingId.end()) {
    DBG_LOG("ID not found in the table! Adding the device!");
    CIPThing* thing = &newThing;
    DBG_LOG_YELLOW_BOLD("The Things Id being registered is:"<<thing->getID());
    auto check = m_mapIPThings.insert({newThing.getID(),&newThing});
    if(false==check.second)  {
      return ERROR;
    }
    else  {
      m_vecThingId.push_back(newThing.getID());
      DBG_LOG_YELLOW_BOLD("IP thing with id"<<newThing.getID()<<" registered! TotalNoThings="<<m_vecThingId.size()<<" Total No. IP Devices="<<m_mapIPThings.size());
      return SUCCESS;
    }   
  }
  else  {
    DBG_LOG("ID Already present in the table! ERROR TotalNoThings="<<m_vecThingId.size()<<" Total No. IP Devices="<<m_mapIPThings.size());
    return ERROR;
  }
}

STATUS CThingsMgr::RegisterThing(CSerialThing& newThing)  {
  DBG_LOG("Newthing Registration invoked CSerialThing ID:"<<newThing.getID());
  auto it = std::find(m_vecThingId.begin(),m_vecThingId.end(),newThing.getID());
  if (it == m_vecThingId.end()) {
    DBG_LOG("ID not found in the table! Adding the device!");
    auto check = m_mapSerialThings.insert({newThing.getID(),&newThing});
    if(false==check.second)  {
      return ERROR;
    }
    else  {
      m_vecThingId.push_back(newThing.getID());
      return SUCCESS;
    }   
  }
  else  {
    DBG_LOG("ID Already present in the table! ERROR ");
    return ERROR;
  }
}

STATUS CThingsMgr::RegisterThing(CBLEThing& newThing)  {
  DBG_LOG("Newthing Registration invoked CBLEThing ID:"<<newThing.getID());
  auto it = std::find(m_vecThingId.begin(),m_vecThingId.end(),newThing.getID());
  if (it == m_vecThingId.end()) {
    DBG_LOG("ID not found in the table! Adding the device!");
    auto check = m_mapBLEThings.insert({newThing.getID(),&newThing});
    if(false==check.second)  {
      return ERROR;
    }
    else  {
      m_vecThingId.push_back(newThing.getID());
      return SUCCESS;
    }   
  }
  else  {
    DBG_LOG("ID Already present in the table! ERROR ");
    return ERROR;
  }
}

STATUS CThingsMgr::AddIPThingToList(CIPThing& newThing) {
  DBG_LOG("In the function AddNewIPThing");
}

int CThingsMgr::getDevCountOnIPaddr(const std::string ipAddress)  {
  
    DBG_LOG("In the function getIPthingByIpAddress");
  
    int count = 0;
  for(auto it : m_mapIPThings)  {
    if (ipAddress == (it.second)->getIPAddress()) {
      count++;
    }
  }
  return count;
}

auto CThingsMgr::getThingConnType(uint16_t id) -> ConnectionType {
  DBG_LOG("In the function getThingConnType:"<<id);
  bool isDevConnected = false;
  for (auto it = m_vecThingId.begin();it!=m_vecThingId.end();it++)  {
    if (id == *it){
      DBG_LOG("The device found in vector");
      isDevConnected = true;
    }
  }
  if (isDevConnected)   {
    auto it_ip = m_mapIPThings.find(id);
    if (it_ip == m_mapIPThings.end()) {
      DBG_LOG("The device found in vector Type id IP");
      return eInternet;
    }
    auto it_se = m_mapSerialThings.find(id);
    if (it_se == m_mapSerialThings.end()) {
      DBG_LOG("The device found in vector Type id Serial");
      return eSerial ;
    }
    auto it_bl = m_mapBLEThings.find(id);
    if (it_bl == m_mapBLEThings.end())
      return eBLE;
    return eMaxConType;
  }
  else  {
    DBG_LOG("The device NOT found in vector");
    std::for_each(m_vecThingId.begin(),m_vecThingId.end(),[](uint16_t i){ std::cout<<" "<<i; });
    return eMaxConType;
  }
}

auto CThingsMgr::getIPThingById(uint16_t id) -> CIPThing*  {
  auto it = m_mapIPThings.find(id);
  if (it == m_mapIPThings.end())  {
    DBG_LOG("UNABLE TO LOCATE DEVICE IN VECTOR  !!");
    return NULL;
  }
  else  {
    DBG_LOG("DEVICE found in table !!");
    return it->second;
  }
}
auto CThingsMgr::getSerialThingById(uint16_t id) -> CSerialThing*  {
  DBG_LOG("In the function getSerialThingById:"<<id);
  if (eSerial == getThingConnType(id))  {
    auto it = m_mapSerialThings.find(id);
    return it->second;
  }
  return NULL;
}
auto CThingsMgr::getBLEThingById(uint16_t id) -> CBLEThing*  {  
  DBG_LOG("In the function getBLEThingById:"<<id);  
  if (eBLE == getThingConnType(id))  {
    auto it = m_mapBLEThings.find(id);
    return it->second;
  }
  return NULL;
}

STATUS CThingsMgr::WriteToQueue(std::string qName,const char* msg)  {
  DBG_LOG_BLUE("WriteToQueue q:"<<qName<<" message:"<<msg);
  mqd_t mqD;
  mqD = mq_open(qName.c_str(), O_RDWR);
  if ((mqd_t)-1 == mqD)  {
    DBG_ERROR("Message queue open ERROR! "<<stderr<<" "<<mqD);
    return ERROR;
  }
  std::string st(msg);
  DBG_LOG("message into the queue is:"<<st<<" size:"<<st.length());
  int status = mq_send(mqD,st.c_str(),(END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+2),1);
  if (0 > status) {
    DBG_ERROR("Message queue send ERROR!"<<errno<<" Error:"<<strerror(errno));
    return ERROR;
  } 
  return SUCCESS;
}

void CThingsMgr::HandleIPQueues() {
  DBG_LOG_YELLOW("Entry");
  char buffer[END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID];
  std::string q = IP_UP_QUEUE_NAME;
  mqd_t mq;
  mq = mq_open(q.c_str(), O_RDWR);
  if ((mqd_t)-1 == mq)  {
    DBG_ERROR(stderr<<" "<<mq);
    return;
  }
  do {
    ssize_t bytes_read;
    bytes_read = mq_receive(mq, buffer, END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID, NULL);
    CHECK(bytes_read >= 0);
    buffer[bytes_read] = '\0';
    std::string ipAddr,msg;
    uint16_t portNo,msgID;
    uint16_t deviceId;
    DBG_LOG_GREEN("Message from Queue:"<<buffer);

    EXTRACT_MSG(buffer,ipAddr,portNo,deviceId,msgID,msg);
    DBG_LOG_YELLOW("IP:"<<ipAddr<<"\tPort:"<<portNo<<"\tID:"<<deviceId);
    DBG_LOG_RED("MsgID:"<<msgID<<" Message:"<<msg);
    if (PK_REGISTER_END_DEVICE_REQ==msgID)  {
      DBG_LOG_YELLOW("Registration request received from thingID"<<deviceId);
      CIPThing* thing = getIPThingById(deviceId);
      if (NULL == thing)  {
        DBG_WARN("Unidentified DeviceID !!");
        STATUS st = WriteToQueue(IP_DOWN_QUEUE_NAME,buffer);
        if (ERROR == st)
          DBG_ERROR("Write to message Q failed!!");
      }
      else  {
        //char msgBuf[END_DEVICE_PACKET_LENGTH+CHARS_IN_IPADDRESS+SDX_DX_BYTES_FOR_DEV_ID];
        uint16_t resMsgID = PK_REGISTER_END_DEVICE_RES;
        std::stringstream ss;
        ss<<portNo<<" "<<ipAddr<<" "<<deviceId<<" "<<PK_REGISTER_END_DEVICE_RES<<" "<<msg;
        std::string msgBuf = ss.str();
        //convertToMessageForDX(msgBuf,ipAddr,portNo,deviceId,resMsgID,msg);
        STATUS st = WriteToQueue(IP_DOWN_QUEUE_NAME,msgBuf.c_str());
        if (ERROR == st)
          DBG_ERROR("Write to message Q failed!!");
      }
    }
    else {
      CIPThing* thing = getIPThingById(deviceId);
      if (NULL == thing)  {
        DBG_WARN("Unidentified DeviceID !!");
        STATUS st = WriteToQueue(IP_DOWN_QUEUE_NAME,buffer);
        if (ERROR == st)
          DBG_ERROR("Write to message Q failed!!");
      }
      else  {
        STATUS st = thing->HandleEvent({msgID,msg});
        std::stringstream ss;
        ss<<portNo<<" "<<ipAddr<<" "<<deviceId<<" "<<msgID+1;
        if (ERROR == st)  
          ss<<" "<<"NACK";
        else
          ss<<" "<<"ACK";
        std::string msgBuf = ss.str();
        STATUS stq = WriteToQueue(IP_DOWN_QUEUE_NAME,msgBuf.c_str());
        if (ERROR == stq)
          DBG_ERROR("Write to message Q failed!!");
      }
    }
  }while(true);
  if ((mqd_t)-1 == mq_close(mq))
    DBG_ERROR(stderr<<" "<<mq);
}
/*
This is where everything comes together!! The magic happens
*/
void CThingsMgr::StartIOTServices() {
  //if (ERROR == this->CreateQueue("/udp_std"))  {
    //DBG_ERROR("Queue Creation Failed!"); 
  //}
  DBG_LOG_GREEN("No. Devices registered="<<m_vecThingId.size());
  for_each(m_vecThingId.begin(),m_vecThingId.end(),[](uint64_t i){DBG_LOG_GREEN(" "<<i);});
  DBG_LOG_GREEN("No. IP Devices registered="<<m_mapIPThings.size());
  std::vector<std::thread> vec_TCPClientThreads(m_mapIPThings.size()+m_mapBLEThings.size()+m_mapSerialThings.size());
  int iThreadIndex = 0;
  
  for (auto iter=m_mapIPThings.begin();iter!=m_mapIPThings.end();++iter)  {
    vec_TCPClientThreads[iThreadIndex] = iter->second->m_ptrCUplink->TCPClient();
    iThreadIndex++;
  }
  for (auto iter=m_mapSerialThings.begin();iter!=m_mapSerialThings.end();++iter)  {
    vec_TCPClientThreads[iThreadIndex] = iter->second->m_ptrCUplink->TCPClient();
    iThreadIndex++;
  }
  for (auto iter=m_mapBLEThings.begin();iter!=m_mapBLEThings.end();++iter)  {
    vec_TCPClientThreads[iThreadIndex] = iter->second->m_ptrCUplink->TCPClient();
    iThreadIndex++;
  }
  
  std::thread thIP(&CThingsMgr::HandleIPQueues,m_pInstance);
  if(thIP.joinable())
    thIP.join();
  
  for (int i =0;i<vec_TCPClientThreads.size();i++)  {
    if (vec_TCPClientThreads[i].joinable())
      vec_TCPClientThreads[i].join();
  }
}

#if 0
int main()  {
  DBG_LOG("Hello World!! ");
  CThingsMgr* thingsMgr = CThingsMgr::getInstance();
  return 0;
}
void CThingsMgr::HandleIPEvent(const std::string& message)  {
  
    std::thread::id this_id = std::this_thread::get_id();

    DBG_LOG("In HandleIPEvent of things_mgr!! in thread:"<<this_id);
  std::string ipAddr,portNo,deviceId,msgID,msg;
  std::stringstream sstrMessage(message);
  getline(sstrMessage,ipAddr,'|');
  getline(sstrMessage,portNo,'|');
  getline(sstrMessage,deviceId,'|');
  getline(sstrMessage,msgID,'|');
  getline(sstrMessage,msg);

  
    DBG_LOG("\n\n::::::New Message Received in Smart device::::::");
    DBG_LOG("IP Address : "<<ipAddr);
    DBG_LOG("Port No : "<<portNo);
    DBG_LOG("Device ID : "<<deviceId);
    DBG_LOG("Message ID : "<<msgID);
    DBG_LOG("Message : "<<msg);


  CIPThing* ipthing = getIPthingByIpAddress(ipAddr);
  if(NULL==ipthing) {
    DBG_LOG("\nNo Device in the list Exists!!... Adding ");
    CIPThing newIpThing(ipAddr,portNo,deviceId);
    STATUS temp = AddNewIPThing(newIpThing);
    temp==SUCCESS ? DBG_LOG("successfully added the new device!") : DBG_LOG("Unsuccessful!!");
  }
  else  {
    ipthing->HandleEvent(msg);
    //std::thread t(&CIPThing::HandleEvent,ipthing,msg);
    //t.detach();
  }
}

void CThingsMgr::StartCLI() {
  
    DBG_LOG("Debug Mode: Inside StartCLI()");
    bool bCLI = true;
    int iChoice = 0;
    while(bCLI) {
      for(int i=0;i<50;i++)
        DBG_LOG(std::endl;
      DBG_LOG("::::::::::::::::: Welcome to the CLI for Things Mgr ::::::::::::::::: ");
      DBG_LOG("          -> MENU <-   ");
      DBG_LOG("1. Number of Devices connected ");
      DBG_LOG("2. Number of IP Addresss connected ");
      DBG_LOG("3. Device Server IP Address ");
      DBG_LOG("4. Display connected Devices ");
      DBG_LOG("5. Exit ");
      std::cin>> iChoice;
      if (iChoice<1 || iChoice>5) {
        DBG_LOG("Invalid choice :"<<iChoice<<" !!");
        DBG_LOG("Press enter to start again!");
        std::cin>>iChoice;
      }
      else  {
        switch (iChoice)
        {
          case 4:
            printAllConnectedDevices();
          break;
          case 5:
            bCLI = false;
          break;
          default:
            DBG_LOG("\nSorry !! Choice not defined yet!! ");
            DBG_LOG("Press enter to start again!");
            std::cin>>iChoice;
          break;
        }
      }

    }  
}

void CThingsMgr::printAllConnectedDevices() {
  
    DBG_LOG("Debug Mode: printAllConnectedDevices");  
    int i = 1;
  if (0<ptr_ipThings.size())
    for (std::vector<CIPThing*>::iterator it=ptr_ipThings.begin();it!=ptr_ipThings.end();it++)  {
      DBG_LOG(" "<<i<<". "<<(*it)->getIPAddress()<<"\t"<<(*it)->getPortNo()<<"\t"<<(*it)->getDeviceID();
      DBG_LOG("\tConnected Status:"<<(*it)->getDConnectedStatus());
      i++;  
    }
  else
    DBG_LOG("No IP Devices connected yet !!");
  DBG_LOG("Press enter to return to Menu");
  std::cin>>i; 
}
void CThingsMgr::HandleQueue (const std::string queueName)  {
    /*
    std::thread th[MAX_THREADS_THNGSMGR];
    bool freeThread[MAX_THREADS_THNGSMGR];
    for (int i=0;i< MAX_THREADS_THNGSMGR; i++)
      freeThread[i]=false; 
    */
#if 0
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

  do {
      ssize_t bytes_read;
      bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
      CHECK(bytes_read >= 0);
      buffer[bytes_read] = '\0';
      std::string message = buffer;
      std::thread th(&CThingsMgr::HandleIPEvent,message);
      HandleIPEvent(message);
      th.detach();
  } while (true);
  
}

STATUS CThingsMgr::AddNewIPThing(const CIPThing ipthing) {
  auto start = ptr_ipThings.begin();
  auto end = ptr_ipThings.end();
  while(start!=end) {   
    if (ipthing.getIPAddress()==(*start)->getIPAddress())
      return ERROR;
    start++;
  } 

  CIPThing* newThing = new CIPThing(ipthing);
  ptr_ipThings.push_back(newThing);
  DBG_LOG("The number of devices Connected:"<<ptr_ipThings.size());
  return SUCCESS;
}

CIPThing* CThingsMgr::getIPthingByIpAddress(const std::string ipaddr) {
  auto start = ptr_ipThings.begin();
  auto end = ptr_ipThings.end();
  while(start!=end) {   
    if (ipaddr==(*start)->getIPAddress())
      return (*start);
    start++;
  } 
  return NULL;
}

int main()
{
	DBG_LOG("Hello World");
	DBG_LOG("Initializing the things manager");
  CThingsMgr things_mgr;
  std::thread th[MAX_THREADS_THNGSMGR];
  bool freeThread[MAX_THREADS_THNGSMGR];
  for (int i=0;i<MAX_THREADS_THNGSMGR;i++)
    freeThread[i] = true;
  mqd_t mq;
  struct mq_attr attr;
  char buffer[MAX_SIZE + 1];

  /* initialize the queue attributes */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;
  std::thread cli (&CThingsMgr::StartCLI,&things_mgr);
  /* create the message queue */
  mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

  do {
    ssize_t bytes_read;
    bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
    CHECK(bytes_read >= 0);
    buffer[bytes_read] = '\0';
    std::string message = buffer;
    int iJobAssigned = -1;
    for (int i=0;i<MAX_THREADS_THNGSMGR;i++)  {
      if (true==freeThread[i] && -1==iJobAssigned)  {
        th[i] = std::thread(&CThingsMgr::HandleIPEvent,&things_mgr,message);
        freeThread[i] = false;
        iJobAssigned = i;
        break;
      }  
    }
    
      if(iJobAssigned<=-1)
        DBG_LOG("\nWarning : No Free threads !! ");
      else
        DBG_LOG("Job Assigned to "<<iJobAssigned<<" thread with ID:"<<th[iJobAssigned].get_id());
    for (int i=(MAX_THREADS_THNGSMGR-1);i>=0;i--) {
      if(th[i].joinable())  {
        th[i].join();
        freeThread[i] = true;
        
          DBG_LOG("Things Manager main(): Thread["<<i<<"] = free");
      }
    }
    //HandleIPEvent(message);   
  } while (true); 
  if (cli.joinable());
    cli.join();
	DBG_LOG("deleted the Things Manager");
	return 0;
}

#endif

#endif

