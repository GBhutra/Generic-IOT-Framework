package edu.tamu.cs.codesign.Communication;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import edu.tamu.cs.codesign.General.SysUtils;
import edu.tamu.cs.codesign.General.Systemctl;

public class CommunicationGateway extends AbstractCommunicationGateway {
	
	public void setState(int state) {
		_SGNL_STATE = state;
	}
	public void _init() {
		setState(1);
		setPort(54325);
		communicationGateway();
		
	}

	@Override
	protected void communicationGateway() {
		Systemctl systemctl = new Systemctl();
		SysUtils utils = systemctl.getInstanceOfSysUtils();
		ServerSocket serverSocket = null;
        Socket socket = null;
        ExecutorService threadPoolConnectedClients = Executors.newCachedThreadPool();	

        try {
            serverSocket = new ServerSocket(getPort());
            utils.printSystem("OK", "Starting Communication Gateway");
        } catch (IOException e) {
        	utils.printSystem("FAIL", "Starting Communication Gateway");
        	utils.printDebug("CLASS: CommunicationGateway -Cannot bind specified port");
            //e.printStackTrace();

        }
        while (_SGNL_STATE == 1) {
            try {
                socket = serverSocket.accept();
            	} catch (IOException e) {
                System.err.println("I/O error: " + e);
            	}
            	try{
            		// New Thread for Handling data :
            		threadPoolConnectedClients.execute(new CommunicationHandeler(socket));
            	}
            	catch (Exception e)
            	{
            		System.err.println("threadPoolConnectedClients: " + e);
            	}
        }
        utils.printEvent("Stopping Communication Gateway");
        threadPoolConnectedClients.shutdownNow();
        utils.printStatus("OK");
        

	}
}
