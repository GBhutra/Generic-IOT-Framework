package edu.tamu.cs.codesign.Communication;

import edu.tamu.cs.codesign.Devices.Things;

public class DeviceIdentity {
	
	/*
	 * This holds the device identity for kinds of devices
	 * Things thing_obj : Holds the constructed things object. Whenever the things object needs to be
	 * Refereed it is accesses by getThingsObj(). Do NOT create more than one instance of a thing object.
	 * 
	 */
	
	/*
	 * The Thing Object
	 * This is automatically set via constructor
	 */
	private Things thing_obj;
	
	/*
	 * deviceID: This is the unique identification of a device in the entire framework.
	 * This is of type short and is a mandatory @param.
	 * This is set globally by user
	 */
	private long deviceID;
	
	/*
	 * String thing_name
	 * An Alias for the thing
	 */
	private String thing_name;
	
	/*
	 * IP and port and socket of the physical device
	 * Format : 000.000.000.000/00000
	 * This is automatically set by the device manager
	 */
	private String IP;
	
	
	/*
	 *  Constructor
	 *  deviceID and thing_obj are required to construct an identity
	 */
	
	public DeviceIdentity(long deviceID, Things thing_obj) {
		this.deviceID = deviceID;
		this.thing_obj = thing_obj;
	}
	public void setThingName(String thing_name) {
		this.thing_name = thing_name;

	}
	
	/*
	 * Getters and setters
	 */
	public Things getThingObj() { return thing_obj; }
	public String getThingName() { return thing_name; }
	public long getDeviceID() { return deviceID; }
	public void setThingIP(String IP){ this.IP = IP;}
	public String getThingIP() { return this.IP; }
	

}
