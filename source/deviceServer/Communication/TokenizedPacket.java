package edu.tamu.cs.codesign.Communication;

public class TokenizedPacket {
	private short Size;
	private PacketType packetType;
	private long DeviceID;
	private String Payload;
	
	/*
	 * Parameterized constructor to create a new Tokenized Packet
	 */
	TokenizedPacket(PacketType packetType, long DeviceID, String Payload) {

		this.Size = (short) (Payload.length()+PacketStructure.headerSize);
		this.packetType = packetType;
		this.DeviceID = DeviceID;
		this.Payload = Payload;
		
	}
	
	
	/*
	 * Getters
	 */
	
	public short size() { return Size; }
	public PacketType packetType() { return packetType; }
	public long deviceID() { return DeviceID; }
	public String payload() { return Payload; }


	/* (non-Javadoc)
	 * Converts a tokenized packet to a string format
	 * @warning This should only be used for display purposes
	 */
	public String toString() {
		return  ("Size=" + Size +
				 " Packet Type=" + packetType.name() +
				 " Device ID=" + DeviceID +
				 " Payload=" + Payload);
	}
}
