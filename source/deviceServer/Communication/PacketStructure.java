package edu.tamu.cs.codesign.Communication;

import edu.tamu.cs.codesign.General.SysUtils;
import edu.tamu.cs.codesign.General.Systemctl;

public final class PacketStructure {
	
	public static final int headerSize = 12;
	
	Systemctl systemctl = new Systemctl();
	SysUtils utils = systemctl.getInstanceOfSysUtils();
	
	
	
	/**
	 * @description This method tokenizes a string packet to a TokenizedPacket  
	 * @param stringPacket
	 * @return TokenizedPacket
	 */
	public synchronized TokenizedPacket tokenizePacket(String stringPacket) {
		/*
		 *  Extract Size
		 *  BYTE 0 and BYTE 1 
		 */
			short size = utils.getShortFromStringBytes(stringPacket.substring(0, 2));
			System.out.println("[] size parsed = "+size);
				
		/*
		 *  Extract Packet Type
		 *  BYTE 2 BYTE 3
		 *  PacketType is defined as an enum.
		 *  All defined packets are register in enum PackeType with its corresponding ID
		 *  This is for convenience.
		 */
			short packetTypeID = utils.getShortFromStringBytes(stringPacket.substring(2, 4));
			PacketType packetType = PacketType.getPacketType(packetTypeID);	
		
		/*
		 *  Extract Device ID
		 *  BYTE 4 BYTE 5 BYTE 6 BYTE 7 BYTE 8 BYTE 9 BYTE 10 BYTE 11
		 */
			long deviceID = utils.getLongFromStringBytes(stringPacket.substring(4, 12));

		/*
		 *  Extract PayLoad
		 *  BYTE 12 to END of Packet
		 */			
			String payload = stringPacket.substring(12);
	
		/*
		 * Form TokenizedPacket
		 */
			return new TokenizedPacket(packetType, deviceID, payload);
			
	}
	
	/*
	 * TODO: Create Packet
	 */
	public synchronized TokenizedPacket createTokenizedPacket(PacketType packetType, long deviceID, String payload) {
		return new TokenizedPacket(packetType, deviceID, payload);
	}
	/*
	 * TODO: public String deTokenizePacket(TokenizedPacket packet) 
	 * 
	 */
	public synchronized String deTokenizePacket(TokenizedPacket tokenizedPacket) {
		System.out.println(tokenizedPacket.toString());
		
		String DeviceID = utils.getStringBytesFromLong(tokenizedPacket.deviceID());
		//System.out.println("DevID: Sidze="+DeviceID.length()+" Packet="+DeviceID);
		
		String Payload = tokenizedPacket.payload();
		//System.out.println("Payload: Sidze="+Payload.length()+" Packet="+Payload);

		String PacketTypeID = utils.getStringBytesFromShort((short)tokenizedPacket.packetType().getPacketTypeID());
		//System.out.println("PacketTypeID: Sidze="+PacketTypeID.length()+" Packet="+PacketTypeID);

		String Size = utils.getStringBytesFromShort(tokenizedPacket.size());
		//System.out.println("Size: Sidze="+Size.length()+" Packet="+Size);

		String s = Size+PacketTypeID+DeviceID+Payload;
		System.out.println("SENT: "+s);
	
		System.out.print(">>> ");

		byte[] bstream = s.getBytes();
		
		StringBuilder sb = new StringBuilder();
		
		for (Byte b : bstream)
			sb.append(String.format("%02X ", b));
		System.out.println(sb+"\n");
		
		return s;
	}
	
	

}
