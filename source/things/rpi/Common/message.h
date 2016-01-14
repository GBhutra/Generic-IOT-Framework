/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: generic
/*		File		: messages.h
/*		Description	: Defines the message IDs for communication between smart device and End Device
/*		Author		: Saket
/*		Date		: 9th December
/***********************************************************************/

#define IOT_UDP_MAX_DATA_LENGTH		256

#define BYTES_FOR_MSG_ID 		2
#define BYTES_FOR_DEV_ID 		2

#define END_DEV_REG_REQ 		01
#define END_DEV_REG_RES			02

#define END_DEV_VALUE_UPDT_REQ	03
#define END_DEV_VALUE_UPDT_RES	04