/***********************************************************************/
/*		Project 	: CSCE 622 Co-Design 
/*		Component	: generic
/*		File		: messages.h
/*		Description	: Defines the message IDs for communication between smart device and Device servers
/*		Author		: Ghanshyam
/*		Date		: 23rd October
/***********************************************************************/


#define SDX_DS_BYTES_FOR_SIZE 		2
#define SDX_DS_BYTES_FOR_MSG_ID 	2
#define SDX_DS_BYTES_FOR_DEV_ID 	8	

/* The follwing message ID are used between a smart device and */
/*---------Server Message IDs-------------------*/
#define PK_SESSION_CREATE_END_DEV_REQ		12341	
#define PK_SESSION_CREATE_END_DEV_RES		12342	

#define PK_SESSION_CREATE_SMART_DEV_REQ		12343	
#define PK_SESSION_CREATE_SMART_DEV_RES		12344	

#define PK_DEVICE_STATUS_REQ				12345
#define PK_DEVICE_STATUS_RES				12346

#define PK_USER_MESSAGE_REQ					12347
#define PK_USER_MESSAGE_RES					12348
/*-------------- Server Message IDs end----------*/
#define SDX_DX_BYTES_FOR_DEV_ID 	2
#define SDX_DX_BYTES_FOR_PORT_NO	2 
#define SDX_DX_BYTES_FOR_MSG_ID		2 

/*-------------- End Device Message IDs----------*/

#define DX_SDX_BYTES_FOR_MSG_ID 	2
#define DX_SDX_BYTES_FOR_DEV_ID 	2	


#define PK_REGISTER_END_DEVICE_REQ 01
#define PK_REGISTER_END_DEVICE_RES 02

#define PK_UPDATE_VALUE_END_DEVICE_REQ 03
#define PK_UPDATE_VALUE_END_DEVICE_RES 04

/*
msg_size(2)|12341(2)|DevID(8)|Payload(max=1011B)
DevID=3472328296227681584
*/


