package edu.tamu.cs.codesign.General;


import edu.tamu.cs.codesign.Communication.CommunicationGateway;
import edu.tamu.cs.codesign.Communication.DeviceManager;
import edu.tamu.cs.codesign.Devices.ThingsInitializer;

public class Systemctl {
	
	public static DeviceManager deviceManager;
	public static CommunicationGateway CG;
	public static SysUtils utils;
	
	public DeviceManager getInstanceOfDeviceManager() {
		return deviceManager;
	}
	
	public SysUtils getInstanceOfSysUtils() {
		return utils;
	}

	public static void main(String[] args) {
		
		/* INITALIZE BASIC UTILS */
		utils = new SysUtils();
		utils.DebugEnable();
		
		
		
		
		/*PASS THRU SETUP */
		try {
			if(args[0].equalsIgnoreCase("SETUP")) {
				utils.print("---------- DEVICE SERVER INSTALLER -----------");
				utils.print("Running First Time Setup .........");
				Installer installer = new Installer();
				if(installer.runFirstTimeSetup()) {
					utils.print("\n\n SUCCESS ");
				}
				else {
					utils.print("\n\n FAIL : !SYSTEM HALTED!");
				}
				System.exit(0);
			}
		}
		catch(Exception e){
			
		}
		
		/*
		 *  PASS THROUGH :   :  NO ARGS PROVIDED
		 *  START ROUTINE PROCEDURES
		 */
		
		/* CONFIGURE INTERNAL PARAMATERS */
		
		
		/**/
		utils.printSystem("OK", "Systemctl Initialization");
		
		/*
		 * INIT device Manager
		 */			
		  deviceManager = new DeviceManager();
		
		  /* !!!!! TEMP !!!!!!! */
	      
		  
		  /* !!!!! END OF TEMP */
		
		/* INIT COMMUNICATION GATEWAY */
		try {
		CG = new CommunicationGateway();
		Thread CGthread = new Thread(){
			    public void run(){
			    try{
			    CG._init();
			    }
			    catch(Exception e)  {
			    	System.exit(0);
			    }
			    }
		 };
		CGthread.start();
		}
		catch (Exception e) {
			
		}
		

		
		/* INIT THINGS INITIALIZER */
		
		try{
			ThingsInitializer thingsInitalizer = new ThingsInitializer();
		}
		catch(Exception e)
		{
			
		}
		
		/* Database Check */
		
		DB db = new DB();
		if(db.checkConnection()) {
			utils.printSystem("OK", "Database Connection");
		}
		else {
			utils.printSystem("FAIL", "Database Connection");
		}
			
		



		

	}

}
