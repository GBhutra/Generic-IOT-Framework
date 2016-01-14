package edu.tamu.cs.codesign.Communication;

/*
 * DEFINES ALL THE PACKET TYPE THAT IS USED BY THE SYSTEM
 * If a Packet ID is not mapped with a Packet Type, it will simply be discarded by the system.
 * RX packets are checked at communication handler.
 * 	**** CONVENTION ***
 * 		ALL 
 * 			RX PACKETS EVEN
 * 			TX PACKETS ODD
 * RX are packets that are received by device server
 * TX are packets that flow out of device sever 
 * 
 * ACKS and NACKS
 * Acknowledgement and Negative acknowledgement are created dynamically. 
 * SERVER_ACK 		Payload = |2: ACK to which packet id|N:Message if any|
 * SERVER NACK 		Payload = |2: NACK to which packet ID|N:Message if Any|
 */


public enum PacketType {
	
	UNKNOWN											(00000),
	
	/* RX */
	PK_SESSION_CREATE_END_DEV_REQ					(12341), //05
	PK_SESSION_CREATE_SMART_DEV_REQ					(12351), //07
	PK_DEVICE_STATUS_REQ							(12345), //09
	PK_USER_MESSAGE_STD_REQ							(12340), //04
	PK_USER_MESSAGE_DTS_RES							(12349), //
	PK_SESSION_DESTROY_END_DEV_REQ					(12343), //07
	
	
	/*TX*/
	PK_SESSION_CREATE_END_DEV_RES					(12342), //06
	PK_SESSION_CREATE_SMART_DEV_RES					(12344), //08
	PK_DEVICE_STATUS_RES							(12346), //10
	PK_USER_MESSAGE_STD_RES							(12348), //12
	PK_USER_MESSAGE_DTS_REQ							(12350), //14
	PK_SESSION_DESTROY_END_DEV_RES					(12352); //16
	
	
	private final int PacketTypeID;
	private PacketType(final int PacketTypeID){
		this.PacketTypeID = PacketTypeID;
		
	}
	public int getPacketTypeID() {
		return this.PacketTypeID;
	}
	public static PacketType getPacketType(int packetTypeID)
	{
	for(PacketType p: PacketType.values()) {
	    if(p.PacketTypeID == packetTypeID) {
	      return p;
	    }
	  }
	  return UNKNOWN;
	}
	
	
}

