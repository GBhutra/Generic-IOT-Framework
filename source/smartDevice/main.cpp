#include "things_mgr.h"
#include "utility.h"
#include "MyThings.h"
#include "uplinkComp.h"
#include <cassert>

#define DEBUG(x) std::cout(x)<<std::endl;

int main()	{
	DBG_LOG_GREEN("########################################################\n");
	DBG_LOG_BLUE("\tSoftware Name:\t"<<BOLD<<"CSCE-617 Co-Design Project IOT Infrastructure");
	DBG_LOG_BLUE("\tVersion:\t"<<BOLD<<"Ver 1.0");
	DBG_LOG_BLUE("\tCompile date:\t"<<BOLD<<__DATE__);
	DBG_LOG_BLUE("\tCompile time:\t"<<BOLD<<__TIME__<<"\n");
	DBG_LOG_GREEN("########################################################");
	CThingsMgr* thingsMgr = CThingsMgr::getInstance();
	MyFridge* Fridge1 = new MyFridge(12336);
	MyFridge* Fridge2 = new MyFridge(12337);
	MyFridge* Fridge3 = new MyFridge(12338);
	MyFridge* Fridge4 = new MyFridge(12339);
	MyFridge* Fridge5 = new MyFridge(12340);
	MyFridge* Fridge6 = new MyFridge(12341);
	MyFridge* Fridge7 = new MyFridge(12342);
	MyFridge* Fridge8 = new MyFridge(12343);
	MyFridge* Fridge9 = new MyFridge(12344);
	MyFridge* Fridge10 = new MyFridge(12345);
	MyFridge* Fridge11 = new MyFridge(12346);
	MyFridge* Fridge12 = new MyFridge(12347);
	//Fridge1->m_ptrCUplink->storeMessageInFromQueue({100,"Ghanshyam"});

	thingsMgr->RegisterThing(*Fridge1);
	thingsMgr->RegisterThing(*Fridge2);
	thingsMgr->RegisterThing(*Fridge3);
	thingsMgr->RegisterThing(*Fridge4);
	thingsMgr->RegisterThing(*Fridge5);
	thingsMgr->RegisterThing(*Fridge6);
	thingsMgr->RegisterThing(*Fridge7);
	thingsMgr->RegisterThing(*Fridge8);
	thingsMgr->RegisterThing(*Fridge9);
	thingsMgr->RegisterThing(*Fridge10);
	thingsMgr->RegisterThing(*Fridge11);
	thingsMgr->StartIOTServices();
	return 0;
}