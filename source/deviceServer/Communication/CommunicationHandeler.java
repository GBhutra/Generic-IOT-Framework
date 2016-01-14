package edu.tamu.cs.codesign.Communication;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


import edu.tamu.cs.codesign.Devices.Things;
import edu.tamu.cs.codesign.FrameworkExceptions.InvalidPacketException;
import edu.tamu.cs.codesign.General.DB;
import edu.tamu.cs.codesign.General.SysUtils;
import edu.tamu.cs.codesign.General.Systemctl;

public class CommunicationHandeler extends Thread {
	/*
	 * Communication Handler is the TCP connection between Device Server and Smart Device.
	 * Smart Device initiates individual TCP connections for all the end devices connected to it
	 * No of end devices connected = No of threads/instances of this class
	 * The object to this class is spawned by CommunicationGateway in parallel.
	 * A reference to this object is given to the thing class via thing.  
	 */
	
	protected Socket socket;
	protected long deviceID;
	Boolean session = false;
	Boolean alive = true;
	
	/*
	 * General References
	 */
	Systemctl systemctl;
	SysUtils util;
	DeviceManager deviceManager;
	PacketStructure packetStructure;
	Things thing;
	InputStream inputStream = null;
    BufferedReader bufferedReaderInput = null;
    PrintWriter printWriterOut =null;
    DB dbase = null;
    
    
    
   
	/*
	 * Constructor
	 */
	public CommunicationHandeler(Socket smartDeviceSocket) {
		this.socket = smartDeviceSocket;
		systemctl = new Systemctl();
		util = systemctl.getInstanceOfSysUtils();
		deviceManager = systemctl.getInstanceOfDeviceManager();
		packetStructure = new PacketStructure();
		dbase = new DB();
		thing = null;
		}
	
	
	/*
	 * This is used by things objects to send data to end device.
	 * TODO: Convert this to packet structure and add proper header information
	 */
	public void sendData(String payload) {
		PacketStructure packetStructure= new PacketStructure();
     	TokenizedPacket userPacket = packetStructure.createTokenizedPacket(PacketType.PK_USER_MESSAGE_DTS_REQ,
     			thing.getIdentity().getDeviceID(), payload);
     	printWriterOut.println(packetStructure.deTokenizePacket(userPacket));
	}
	
	
	/*
	 * (non-Javadoc)
	 * @see java.lang.Thread#run()
	 * Execution Starts From Here
	 */
	@Override
	public void run() {
		
		
		
        try {
        	// TCP Connected
        	util.printDebug("TCP Connected: "+ socket.getRemoteSocketAddress());
        	/*
        	 * Create Streams
        	 */
            inputStream = socket.getInputStream();
            bufferedReaderInput = new BufferedReader(new InputStreamReader(inputStream));
            printWriterOut = new PrintWriter(socket.getOutputStream(), true);
            
            
        } catch (IOException e) {
        	/*
        	 * This will rarely fail.
        	 * If creating streams fail nothing can be done further.
        	 * Kill the thread
        	 */
            return;
        }
        
        
        String line = null;
        while (!printWriterOut.checkError() && alive)
        {
        	try {
        		line = bufferedReaderInput.readLine();
        		if(line == null) break;
        		
        		/*
        		 * TEMP!!! TO DISPLAY INCOMING BYTE STREAM
        		 */
        		System.out.println("RECEIEVED: "+line);
        		byte[] bstream = line.getBytes();
        		StringBuilder sb = new StringBuilder();
        		System.out.print("<<< ");
        		for (Byte b : bstream)
        			sb.append(String.format("%02X ", b));
        		System.out.println(sb+"\n");
        		
        		/*
        		 * Our headers are of length PacketStructure.headerSize, If a received packet has length
        		 * less than that we can simply discard it.
        		 * TODO: Log line for records
        		 */
        		if(line.length() < PacketStructure.headerSize){
        			util.printDebug("Invalid Header received from "+ socket.getRemoteSocketAddress());
    				printWriterOut.println("NAK|INVALID HEADER");
    				throw new InvalidPacketException("Invalid Headers");
        		}
        		
        		/*
        		 *  Tokenize the packet 
        		 */
        		
        		TokenizedPacket packet = packetStructure.tokenizePacket(line);
        		dbase.runSQL("raw-packet-log", packet.packetType().name(), Long.toString(packet.deviceID()), packet.payload());
        		System.out.println(packet.toString());
        		
        		/*
        		 *  Check if it is a valid packet type
        		 */
        		if(packet.packetType() == PacketType.UNKNOWN){
        			util.printDebug("Unknown Packet Type received from "+ socket.getRemoteSocketAddress());
        			PacketStructure packetStructure= new PacketStructure();
                 	TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES,
                 			packet.deviceID(), "NAK|UNKNOWN PACKET TYPE");
                 	printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
    				throw new InvalidPacketException("Unknown Packet");
        		}
        		
        		/*
        		 *  SESSION HANDLE
        		 */
        		if(packet.packetType() == PacketType.PK_SESSION_CREATE_END_DEV_REQ) {			
        			/*
        			 *  check if it is an active device from device manager, obtain reference to things object
        			 *  if the method returns null, it means no such device exist and you can exit safely.
        			 */
        			
        			if(deviceManager.getStdEndDeviceThingObj(packet.deviceID()) == null) {
        				util.printDebug("Closing TCP Connection to an unidentified device at"+ socket.getRemoteSocketAddress());
        				PacketStructure packetStructure= new PacketStructure();
                     	TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES,
                     			packet.deviceID(), "NAK|UNIDENTIFIED DEVICE");
                     	printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
        				throw new IOException("Unidentified Device");
        				}
        			
        			
        			
        			/*
        			 * Now try checking reference to Communication Handler object from things object
        			 * If this is not null, a session exists already. you can exit throwing session already exists
        			 * !session is added because the same device of this session can again send a create session request
        			 * in that case we do not close the connection
        			 */
        			else if(deviceManager.getStdEndDeviceThingObj(packet.deviceID()).checkExistanceOfHandleIncomingDataObj() && !session) {
        				 util.printDebug("Multipe Session Request by "+ packet.deviceID() +" at "+ socket.getRemoteSocketAddress());
        				 PacketStructure packetStructure= new PacketStructure();
                         TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES,
                      			packet.deviceID(), "NAK|MULTIPLE SESSION REQUESTED");
                      	 printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
         				 throw new IOException("MULTIPLE Session Request");
        			 }
        			else {
        			 /*
        			  * Check if Session exists and there is request for another session.
        			  * !THIS IS A FILTER! Ideally Smart device should not recreate session
        			  * TODO: Reply with proper packet structure
        			  */
        			if(session){
        				/*
        				 * If the same device is requesting for another session for the same device
        				 * throw a Invalid packet exception ( Note Device is not disconnected ) 
        				 */
        				if(this.deviceID == packet.deviceID()) {
        					PacketStructure packetStructure= new PacketStructure();
                         	TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES,
                         			packet.deviceID(), "NAK|SESSION ALREADY EXISTS");
                         	printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
        					throw new InvalidPacketException("DUPLICATE SESSION REQUEST");
        				}
        				
        				/*
        				 * If a endDevice session is trying to create a session for another device
        				 * @TEMPC: This is simply a SECURITY BREACH from  Ghanshyam's end .Kick him and terminate session :D
        				 *  
        				 */
        				util.printDebug(packet.deviceID() + " tried overwriting session of " + this.deviceID);
        				PacketStructure packetStructure= new PacketStructure();
                     	TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES,
                     			packet.deviceID(), "NAK|SESSION OVERWRITE REQUESTED|THIS IS ILLEGAL");
                     	printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
        				releaseHook();
        				throw new IOException("ILLEGAL SESSION OPERATION");
        				
        			}
        			/*
        			 * Everything is fine ! Create a session :)
        			 */
        			 thing = deviceManager.getStdEndDeviceThingObj(packet.deviceID());
        			 this.deviceID = packet.deviceID();
        			 this.session =true;
        			 thing.getIdentity().setThingIP(socket.getRemoteSocketAddress().toString());
                  	 thing.setHandleIncomingDataObj(this);
                  	 // !!!!! TEMP HANDLE
                  	 PacketStructure packetStructure= new PacketStructure();
                  	 TokenizedPacket ackPacket = packetStructure.createTokenizedPacket(PacketType.PK_SESSION_CREATE_END_DEV_RES, thing.getIdentity().getDeviceID(), "ACK");
        			 printWriterOut.println(packetStructure.deTokenizePacket(ackPacket));
        			 util.printDebug("SESSION CREATED FOR DEVICE "+this.deviceID);
             		 dbase.runSQL("session-create", Long.toString(packet.deviceID()));

        			 
        			 
        			}
        		}
        		if(packet.packetType() != PacketType.PK_SESSION_CREATE_END_DEV_REQ) {
        		/*
        		 * Session creation is managed, now further codes must be executed only if proper session exist
        		 * check hook status 
        		 * if hook fails, it means another device is being injected to this device session. This can be a flaw
        		 * at smart device level or because of a sniffed network 
        		 */
        		if(!session) {
        			PacketStructure packetStructure= new PacketStructure();
                     TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(packet.packetType(),
                     		packet.deviceID(), "NAK|NO SESSION EXISTS");
                     printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
    				throw new InvalidPacketException("NO SESSION EXISTS");
        			}
        			
        		if(!isHooked(packet.deviceID())) {
        			util.printDebug("Device Injection Detcted : Injected "+ packet.deviceID() +" at "+ socket.getRemoteSocketAddress());
        			PacketStructure packetStructure= new PacketStructure();
                 	TokenizedPacket nakPacket = packetStructure.createTokenizedPacket(packet.packetType(),
                 			packet.deviceID(), "NAK|ILLEGAL DEVICE INJECTION");
                 	printWriterOut.println(packetStructure.deTokenizePacket(nakPacket));
    				 
        		}
        		else {
        			/*
            		 * This part is protected by existence of a session
            		 * Forward -> The packet to handlePacket() 
            		 */
        			if(handlePacket(packet)){
        				
            			printWriterOut.println("ACK");
            		}
            		else
            		{
            			
            		printWriterOut.println("NACK | Packet Could Not be Handled");
            		
            		}
        			
        		}
        		}  		
        	}
        	catch (InvalidPacketException ie) {
        		util.printDebug("InvalidPacketException : " + ie.getMessage());
        	}
        	/*catch (NullPointerException ne) {
        		/* 
        		 * Unknown disconnection. 
        		 * Lets close and release hook;
        		 
        		util.printDebug("Client Remotely Disconnected (Unknown): "+ socket.getRemoteSocketAddress());
        		releaseHook();
        		
        	}*/
        	catch (Exception e)
        	{	
        		e.printStackTrace();
        		printWriterOut.println("NAK|INVALID HEADERS RECEIVED");
                printWriterOut.flush();
                util.printDebug("Closed Connection : "+ socket.getRemoteSocketAddress());
        		printWriterOut.flush();
                
                disconnect();
				//socket.close();
				return;
        	}
        		
            
        }
        try {
        	if(alive)util.printDebug("Client Remotely Disconnected: "+ socket.getRemoteSocketAddress());
			/*socket.close();
			releaseHook();*/
        	disconnect();
		} catch (Exception e) {
			//e.printStackTrace();
		}
        
        
        
		
	}
	/*
	 * This is the function that handles a packet.
	 * This function is Session Safe.
	 */
	private boolean handlePacket(TokenizedPacket packet)
	{
		util.printDebug(packet.toString());
		
		switch(packet.packetType()) {
		
		case PK_USER_MESSAGE_STD_REQ:
			/*
			 *  A USER MESSAGE IS RECEIVED
			 *  forward it to the thing's onDataReceive function
			 */
			thing.onDataReceive(packet.payload());
			return true;
			
		case PK_SESSION_DESTROY_END_DEV_REQ:
			/*
			 * 
			 */
			disconnect();
			return true;
			
			
		default:
			break;
		}
		return false;
	}
	
	
	/*
	 * Returns true if the device is hooked to the present thread
	 */
	private boolean isHooked(long deviceID) {
		if(this.session && this.deviceID == deviceID)
			return true;
		return false;
	}
	
	public void releaseHook() {
		this.session =false;
		if(thing!=null) {
    		util.printDebug("~ Released " + thing.getIdentity().getDeviceID());
    		thing.setHandleIncomingDataObj(null);
    		dbase.runSQL("session-terminate", Long.toString(thing.getIdentity().getDeviceID()));
    	}
		
	}
	
	public void disconnect(){
		releaseHook();
		try {
			socket.close();
			util.printDebug("SESSION DISCONNECTED");
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
		/*
		try { 
            util.printDebug("<<< Received "+socket.getRemoteSocketAddress()+" DATA: "+line);
             if(line.contains("myFridge"))
             {
             	thing = deviceManager.getStdEndDeviceThingObj("myFridge");
             	thing.getIdentity().setThingIP(socket.getRemoteSocketAddress().toString());
             	thing.setHandleIncomingDataObj(this);
             	thing.onDataReceive(line);
             	
             	return true;
             }
             if(line.contains("Reva"))
             {
             	thing = deviceManager.getStdEndDeviceThingObj("Reva");
             	thing.getIdentity().setThingIP(socket.getRemoteSocketAddress().toString());
             	thing.onDataReceive(line);
             	return true;
             }
             return false;
         } catch (Exception e) {
            util.printDebug("HandleIncomingData.handleMessage() Failed ");
            e.printStackTrace();
             return false;
         }
		
	}*/

}
