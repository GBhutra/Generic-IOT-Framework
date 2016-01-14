#include "things.h"

class MyFridge : public virtual CIPThing 	{
public:
	MyFridge(uint16_t id) : CIPThing(id), CThings(id) {};
	MyFridge(uint16_t id,std::string ip) : CIPThing(id,ip), CThings(id) {};
	STATUS HandleEvent(Message msg)	{
		DBG_LOG("msg ID:"<<msg.ID<<"\tString:"<<msg.val);
		return SendDataToServer({12340,"Payload"});
	}
	STATUS HandleNotification(Message msg)	{
		DBG_LOG("msg ID:"<<msg.ID<<"\tString:"<<msg.val);
		SendDataToDevice(msg);
		return SUCCESS;
	}
};