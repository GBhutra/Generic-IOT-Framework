package edu.tamu.cs.codesign.Devices;

import edu.tamu.cs.codesign.FrameworkExceptions.*;
public class Home extends Things{

	public Home(long deviceID) {
		super(deviceID);
	}
	
	@Override
	public boolean onDataReceive(String data) {
		System.out.println("From Home: onDataReceive() Data Received");
		try {
		sendData("ACK FROM HOME");
		}
		catch (DeviceOfflineException e){
			e.printStackTrace();
		}
		return true;
	}

}
