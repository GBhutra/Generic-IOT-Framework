package edu.tamu.cs.codesign.Devices;

import java.io.IOException;

import edu.tamu.cs.codesign.Communication.DeviceIdentity;
import edu.tamu.cs.codesign.Communication.DeviceManager;
import edu.tamu.cs.codesign.Communication.CommunicationHandeler;
import edu.tamu.cs.codesign.FrameworkExceptions.DeviceOfflineException;
import edu.tamu.cs.codesign.General.Systemctl;

public abstract class Things {
	/*
	 * The "Things class" is an abstract class that defines rules to model 
	 * a thing.
	 * This class is the owner of the following
	 * 		i) 	DeviceIdentity of the thing
	 * 		ii) A reference to CommunicationHandeler object
	 * 				This reference is set by CommunicationHandler when a 
	 * 				a new device gets connected.
	 * This class defines two major function that is used by user modeled
	 * things classes.
	 * 		i) 	SendData(String Data)
	 * 		ii)	onDataReceive()
	 */
	

	/*
	 * General References
	 */
	Systemctl systemctl = new Systemctl();
	DeviceManager deviceManager = systemctl.getInstanceOfDeviceManager();
	
	/*
	 * A reference to communication handler object build specifically for
	 * this thing.  
	 */
	private CommunicationHandeler communicationHandeler = null;
	
	/*
	 * Holds Identity of the device
	 * New memory is allocated here and is default constructible.
	 */
	private DeviceIdentity deviceIdentity;
	
	/*
	 * Constructor
	 */
	public Things(long deviceID)
	{
		/*
		 * Create Identity of Device
		 */
		deviceIdentity = new DeviceIdentity(deviceID, this);
		
		/*
		 * Register this device with Device Manager
		 */
		deviceManager.addStdEndDeviceIdentity(deviceIdentity);
	}
	
	/*
	 * Getter - Settter
	 */
	public DeviceIdentity getIdentity(){
		
		return deviceIdentity;
	}
	
	/*
	 * Setter : Communication Handler calls this method to set communication handler object 
	 */
	public void setHandleIncomingDataObj(CommunicationHandeler obj) {
		communicationHandeler = obj;
	}
	
	/*
	 * Returns true if Communication handler object is set.
	 * This is used by communication handler itself to avoid multiple sessions
	 */
	public boolean checkExistanceOfHandleIncomingDataObj()
	{
		if (communicationHandeler == null)
			return false;
		return true;
	}
	
	
	/*
	 * This method is called by user modeled things when it wants to send data to the end device
	 * via the smart device
	 */
	public void sendData(String data) throws DeviceOfflineException{
		if (communicationHandeler == null) {
			throw new DeviceOfflineException("Uplink has not been initated by end device");
		}
		communicationHandeler.sendData(data);
	}
	
	/*
	 * This is an abstract method. This method is run by communication handler when user data is
	 * received for this device.
	 * The user modeled class MUST implement this method.  	
	 */
	public abstract boolean onDataReceive(String data);
	

}
