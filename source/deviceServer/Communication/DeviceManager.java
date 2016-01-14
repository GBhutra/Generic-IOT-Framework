package edu.tamu.cs.codesign.Communication;
import edu.tamu.cs.codesign.Devices.Things;
import edu.tamu.cs.codesign.FrameworkExceptions.NoSuchDeviceException;

import java.util.Vector;



public class DeviceManager {
	/* 
	 * PROFILE:  Standard End Device
	 * This volatile Vector Device Identity is used to hold objects of all standard end devices.
	 * Note that this is the one repository that has identity to all available standard end devices
	 * If a reference is required, it must be taken only from this vector using the below defined functions.
	 */
	public static volatile Vector<DeviceIdentity> stdEndDevices = new Vector<DeviceIdentity>();
	
	
	/*
	 * PROFILE:  Standard End Device
	 * Takes an DeviceIdentity and places a reference to stdEndDevice vector
	 * Each device has a DeviceIdentity that holds a deviceID and its object of type things  
	 */
	public synchronized void addStdEndDeviceIdentity(DeviceIdentity deviceIdentity) {
		stdEndDevices.add(deviceIdentity);
	}
	
	/*
	 * PROFILE:  Standard End Device
	 * searches in stdEndDevice vector to find identity of a device by matching
	 * its deviceID.
	 * THROWS NoSuchDevice Exception if not found.
	 * TODO: return null or throw an exception ?
	 */
	public synchronized DeviceIdentity getStdDeviceIdentity(long deviceID) throws NoSuchDeviceException{
		for (DeviceIdentity deviceIdentity : stdEndDevices) {
			
			if(deviceIdentity.getDeviceID() == deviceID) { 
				return deviceIdentity; 
				}
		}
		throw new NoSuchDeviceException("DeviceManager->getStdDeviceIdentity(: No such end device found");
	}
	/*
	 * PROFILE: Standard End device
	 * A wrapper function to fetch the 'Things' object
	 * NOTE: This is just a wrapper function to provide simplicity. This doesnot add any additional 
	 * functionality to Device Manager Class 
	 */
	public synchronized Things getStdEndDeviceThingObj(long deviceID) throws NoSuchDeviceException{
		for (DeviceIdentity deviceIdentity : stdEndDevices) {
			if(deviceIdentity.getDeviceID() == deviceID)
			{ 
				return deviceIdentity.getThingObj();
			}
		}
		throw new NoSuchDeviceException("DeviceManager->getStdEndDeviceThingsObj: No such end device found");	
	}


}
