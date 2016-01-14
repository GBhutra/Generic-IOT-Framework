package edu.tamu.cs.codesign.Devices;

import edu.tamu.cs.codesign.FrameworkExceptions.*;

public class Fridge extends Things{

	
	Fridge(long deviceID)
	{
		super(deviceID);
	}
	

	@Override
	public boolean onDataReceive(String data) {
		System.out.println("From Fridge: onDataReceive() Data Received");
		try {
		sendData("I am chilled!!");
		}
		catch (DeviceOfflineException e){
			e.printStackTrace();
		}
		return true;
	}
	
	public void someOtherFunctionOfFridge()
	{
		System.out.println("Hello");
	}

}
