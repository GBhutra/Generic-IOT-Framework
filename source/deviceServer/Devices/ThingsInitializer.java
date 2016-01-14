package edu.tamu.cs.codesign.Devices;


import java.util.Vector;

import edu.tamu.cs.codesign.General.SysUtils;
import edu.tamu.cs.codesign.General.Systemctl;

public class ThingsInitializer {
	
	public  ThingsInitializer()
	{
		Systemctl systemctl = new Systemctl();
		SysUtils sysUtils = systemctl.getInstanceOfSysUtils();
		
		
		//Initalize fridge
		try {
			Vector<Fridge> fridge= new Vector();
			fridge.addElement(new Fridge(3472328296227681584L));
			fridge.addElement(new Fridge(3474580096041366832L));
			fridge.addElement(new Fridge(3474861571018077488L));
			fridge.addElement(new Fridge(3472609771204392240L));
			fridge.addElement(new Fridge(3473454196134524208L));
			fridge.addElement(new Fridge(3475143045994788144L));
			fridge.addElement(new Fridge(3474298621064656176L));
			fridge.addElement(new Fridge(3474017146087945520L));
			fridge.addElement(new Fridge(3473172721157813552L));
			fridge.addElement(new Fridge(3473735671111234864L));
			fridge.addElement(new Fridge(3472891246181102896L));
			
			
			sysUtils.printSystem("OK", "ThingsInitializer : myFridge Initalization");
			//fridge.sendData("hi!");
		}
		catch (Exception e){
			e.printStackTrace();
			sysUtils.printSystem("FAIL", "ThingsInitializer : myFridge Initalization");
		}
		// END
		
		//Initialize Car
		try {
		Car thingCar = new Car(3472328296227681585L);
		sysUtils.printSystem("OK", "ThingsInitializer : Car Reva Initalization");
		}
		catch (Exception e){
			sysUtils.printSystem("FAIL", "ThingsInitializer : Car Reva Initalization");
		}
		// END
		
		
		
		
		
	}

}
