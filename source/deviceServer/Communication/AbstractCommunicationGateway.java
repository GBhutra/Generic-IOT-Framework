/*
 * Type			: Abstract Class
 * Description	: Abstract Class for Communication Gateway
 * @author		: Siddhant Rath
 * @DateCreated	: 23 NOV 2015 
 */

package edu.tamu.cs.codesign.Communication;

public abstract class AbstractCommunicationGateway {
	/* _SNGL_STATE 
	 * = 1 : RUN
	 * = 5 : GRACEFULL SHUTDOWN
	 * = 0 : KILL 
	 */
	protected static volatile int _SGNL_STATE;
	
	
	private static int CommunicationPort;
	
	protected abstract void communicationGateway(); 
	
	public void setPort(int CommunicationPort) {
		AbstractCommunicationGateway.CommunicationPort = CommunicationPort;
	}
	public int getPort() {
		return CommunicationPort;
	}
	

}
