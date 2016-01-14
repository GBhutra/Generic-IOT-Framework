/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: generic
/*		File		: iot_common_dx.h
/*		Description	: Defines some common terminology across the project like macros, enums etc
/*		Author		: Ghanshyam
/*		Date		: 23rd October
/***********************************************************************/

#ifndef utility_h
#define utility_h

#include <iostream>
#include "message.h"

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

#define RESPONSE_TIMER_EXP_MSEC  	10000	
#define READ_SENSOR_VALUE_SEC		5	
#define SUSPEND_STATE_TIMEOUT_SEC	15								

										
typedef enum 
{
	SUCCESS = 0,
	ERROR = -1
}STATUS;

typedef enum 
{
	INIT = 0,
	ACTIVE = 1,
	SUSPEND = 2,
	INVALID
}STATE_T;

typedef struct Message{
	uint16_t ID;
	std::string val;
	Message() : ID(0) {};
	Message(uint16_t id,std::string msg) : ID{id},val{msg} {}; 
}*pMessage;


void convertMessageToBytes(Message& M,char* buff)	{
	std::memcpy(buff+BYTES_FOR_DEV_ID,&M.ID,BYTES_FOR_MSG_ID);
	strncpy(buff+BYTES_FOR_DEV_ID+BYTES_FOR_MSG_ID,M.val.c_str(),IOT_UDP_MAX_DATA_LENGTH-(BYTES_FOR_DEV_ID+BYTES_FOR_MSG_ID));
}

void putDeviceIdInMsg(uint16_t& devID,char* buff)	{
	std::memcpy(buff,&devID,BYTES_FOR_DEV_ID);
}

void getDeviceIdInMsg(uint16_t& devID,char* buff)	{
	std::memcpy(&devID,buff,BYTES_FOR_DEV_ID);
}

void convertBytesToMessage(Message& M,char* buff)	{
	std::memcpy(&M.ID,buff+BYTES_FOR_DEV_ID,BYTES_FOR_MSG_ID);
	M.val.assign(buff+BYTES_FOR_DEV_ID+BYTES_FOR_MSG_ID,IOT_UDP_MAX_DATA_LENGTH-(BYTES_FOR_DEV_ID+BYTES_FOR_MSG_ID));
}


#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \


#endif