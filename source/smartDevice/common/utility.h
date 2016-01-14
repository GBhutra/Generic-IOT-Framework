/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: generic
/*		File		: utility.h
/*		Description	: Defines some common terminology across the project like macros, enums etc
/*		Author		: Ghanshyam
/*		Date		: 23rd October
/***********************************************************************/

#ifndef utility_h
#define utility_h

#include <iostream>
#include <string>
#include <cstring>
#include "messages.h"


/*Debugging Definitions*/
#define DBG_LVL 3

#define NOR  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define WHT  "\x1B[37m"

#define BOLD "\x1B[1m"
#define UNDL "\x1B[4m"

#if (DBG_LVL==3)
	#define DBG_LOG_RED(x) 			DBG_LOG(RED<<x<<NOR)
	#define DBG_LOG_RED_BOLD(x) 	DBG_LOG(RED<<BOLD<<x<<NOR)
	#define DBG_LOG_GREEN(x)		DBG_LOG(GRN<<x<<NOR)
	#define DBG_LOG_GREEN_BOLD(x)	DBG_LOG(GRN<<BOLD<<x<<NOR)
	#define DBG_LOG_YELLOW(x)		DBG_LOG(YEL<<x<<NOR)
	#define DBG_LOG_YELLOW_BOLD(x)	DBG_LOG(YEL<<BOLD<<x<<NOR)
	#define DBG_LOG_BLUE(x)			DBG_LOG(BLU<<x<<NOR)
	#define DBG_LOG_BLUE_BOLD(x)	DBG_LOG(BLU<<BOLD<<x<<NOR)
	#define DBG_LOG(x) 				std::cout<<"[" << __FILE__ << "]["<< __FUNCTION__\
									<< "()][Line " << __LINE__ << "] "\
									<<x<<std::endl;
	#define DBG_ERROR(x)			DBG_LOG_RED_BOLD(x)
	#define DBG_WARN(x)				DBG_LOG_YELLOW_BOLD(x)
#elif (DBG_LVL==2) 
	#define DBG_LOG(x) 
	#define DBG_LOG_RED(x) 			
	#define DBG_LOG_RED_BOLD(x) 	
	#define DBG_LOG_GREEN(x)		
	#define DBG_LOG_GREEN_BOLD(x)	
	#define DBG_LOG_YELLOW(x)		
	#define DBG_LOG_YELLOW_BOLD(x)	
	#define DBG_LOG_BLUE(x)			
	#define DBG_LOG_BLUE_BOLD(x)									
	#define DBG_ERROR(x)			std::cout<<"[" << __FILE__ << "]"<< __FUNCTION__\
									<< "()][Line " << __LINE__ << "] "\
									<<x<<std::endl;
	#define DBG_WARN(x)				DBG_ERROR(x)
#elif (DBG_LVL==1)
	#define DBG_LOG(x) 
	#define DBG_LOG_RED(x) 			
	#define DBG_LOG_RED_BOLD(x) 	
	#define DBG_LOG_GREEN(x)		
	#define DBG_LOG_GREEN_BOLD(x)	
	#define DBG_LOG_YELLOW(x)		
	#define DBG_LOG_YELLOW_BOLD(x)	
	#define DBG_LOG_BLUE(x)			
	#define DBG_LOG_BLUE_BOLD(x)	
	#define DBG_WARN(x)							
	#define DBG_ERROR(x)			std::cout<<"[" << __FILE__ << "]"<< __FUNCTION__\
									<< "()][Line " << __LINE__ << "] "\
									<<x<<std::endl;
#else
	#define DBG_LOG(x) 
	#define DBG_LOG_RED(x) 			
	#define DBG_LOG_RED_BOLD(x) 	
	#define DBG_LOG_GREEN(x)		
	#define DBG_LOG_GREEN_BOLD(x)	
	#define DBG_LOG_YELLOW(x)		
	#define DBG_LOG_YELLOW_BOLD(x)	
	#define DBG_LOG_BLUE(x)			
	#define DBG_LOG_BLUE_BOLD(x)
	#define DBG_WARN(x)
	#define DBG_ERROR(x)
#endif

/**/

								
#define IOT_MAX_MSG_SIZE 1024
#define MAX_THREADS_THNGSMGR 10
#define DEVICE_SERVER_IP_ADDRESS "192.168.168.137"
#define DEVICE_SERVER_PORT_NO 54325
//#define DEV_SERVER_ACK_WORD_LENGTH 3
#define DEV_SERVER_ACK_WORD "ACK"
#define MAX_UPLINK_QUEUE_SIZE 5
#define EXTRACT_MSG(message,ip,port,deviceId,msgId,msg)	do {\
											std::stringstream ss;\
											ss<<message;\
											ss>>port>>ip>>deviceId>>msgId>>msg;\
											} while (0);

#define END_DEVICE_PACKET_LENGTH 255
#define UDP_ACK_STRING "ACK"	
#define UDP_SERVER_PORTNO 8080
#define QUEUE_NAME  "/ipmqueue"
#define DELIMITER ' '
#define IP_UP_QUEUE_NAME 	"/ipmqueueUp"
#define IP_DOWN_QUEUE_NAME	"/ipmqueueDown"
#define CHARS_IN_IPADDRESS 15				
#define END_DEV_ID_CONST_EXT_4BYTE_1 808465712
#define END_DEV_ID_CONST_EXT_2BYTE_2 12336							
										
typedef enum 
{
	SUCCESS = 0,
	ERROR = -1
}STATUS;

typedef struct Message{
	uint16_t ID;
	std::string val;
	Message() : ID(0) {};
	Message(uint16_t id,std::string msg) : ID{id},val{msg} {}; 
}*pMessage;

typedef enum { 
	eInternet,
	eSerial,
	eBLE,
	eMaxConType
 }ConnectionType;

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \


#endif