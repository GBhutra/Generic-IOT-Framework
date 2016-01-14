package edu.tamu.cs.codesign.General;

import java.nio.ByteBuffer;

public class SysUtils {
	
	
	/*
	 * Related to tokenization of strings to different data format
	 */
	
	/*
	 * Input a string of length four and return int value
	 * For example ORIGINAL ENCODING 			0x62, 0x63
	 * 						VISIBLE STRING  	bc
	 * 						Returned Short VAL 	25187
	 */
	
	public short getShortFromStringBytes(String s) throws IllegalArgumentException
	{
		if(s.length()!=2) throw new IllegalArgumentException("Length of String Should be 2");
		byte[] byteArray = s.getBytes();
		return  java.nio.ByteBuffer.wrap(byteArray).getShort();
		/*char[] charArray = s.toCharArray();
		short _short;
		_short=(short) charArray[0];
		_short=(short) (_short << 8);
		_short=(short) (_short|charArray[1]);
		return _short;*/
	}
	
	/*
	 * Input a string of length four and return int value
	 * For example ORIGINAL ENCODING 			0x62, 0x63, 0x64, 0x65
	 * 						VISIBLE STRING  	bcde
	 * 						Returned INT VAL 	1650680933
	 */
	
	public int getIntFromStringBytes(String s) throws IllegalArgumentException
	{
		if(s.length()!=4) throw new IllegalArgumentException("Length of String Should be 4");
		byte[] byteArray = s.getBytes();
		return  java.nio.ByteBuffer.wrap(byteArray).getInt();
	}
	

	/*
	 * Input a string of length eight and return long value
	 * For example ORIGINAL ENCODING 			0x62, 0x63, 0x64, 0x65, 0x62, 0x63, 0x64, 0x65
	 * 						VISIBLE STRING  	bcdebcde
	 * 						Returned LONG VAL 	
	 */
	
	public long getLongFromStringBytes(String s) throws IllegalArgumentException
	{
		if(s.length()!=8) throw new IllegalArgumentException("Length of String Should be 8");
		byte[] byteArray = s.getBytes();
		return  java.nio.ByteBuffer.wrap(byteArray).getLong();
	}
	
	/*
	 * Input a short value and return byte string of length 2
	 * For example 		
	 * 						INPUT Short VAL 	25187
	 * 						ENCODING 			0x62, 0x63
	 * 						OUTPUT  STRING  	bc
	 */
	public String getStringBytesFromShort(Short val) {
		byte[] bstream =  new byte[2];
	    ByteBuffer buffer = ByteBuffer.allocate(bstream.length);
	    buffer.putShort(val);
	    bstream = buffer.array();
		return new String(bstream);
		/*
		char _char[] = new char [2];
		_char[0] = (char) (val & 0x00FF);
		_char[1] = (char) ((val >>> 8 ) & 0x00FF);
		return new String(_char);*/
	}
	
	/*
	 * Input a int value and return byte string of length 2
	 * For example 		
	 * 						INPUT Integer VAL 	1650680933
	 * 						ENCODING 			0x62, 0x63, 0x64, 0x65
	 * 						OUTPUT  STRING  	bcde
	 */
	public String getStringBytesFromInt(int val) {
		byte[] bstream =  new byte[4];
	    ByteBuffer buffer = ByteBuffer.allocate(bstream.length);
	    buffer.putInt(val);
	    bstream = buffer.array();
		return new String(bstream);
	}
	
	
	/*
	 * Input a long value and return byte string of length 2
	 * For example 		
	 * 						INPUT Integer VAL 	
	 * 						ENCODING 			0x62, 0x63, 0x64, 0x65, 0x62, 0x63, 0x64, 0x65
	 * 						OUTPUT  STRING  	bcdebcde
	 */
	public String getStringBytesFromLong(long val) {
		byte[] bstream =  new byte[8];
	    ByteBuffer buffer = ByteBuffer.allocate(bstream.length);
	    buffer.putLong(val);
	    bstream = buffer.array();
		return new String(bstream);
	}
	
	
	
	

	/*
	 * Related to debug prints
	 */
	
	private static boolean debugEnabled=false;
	
	public  void DebugEnable() {
		debugEnabled = true;
		System.out.println("Debug Prints are Enabled");
	}
	
	private boolean isDebugEnabled() {
		return debugEnabled;
	}
	
	public synchronized void printDebug(String s1) {
		if(isDebugEnabled()) {
			System.out.println("[DEBUG] "+s1);
			
		}
	}
	
	public synchronized void printSystem(String status, String message) {
		message = String.format("%-60s", message);
		message = message + "[" + status.toUpperCase() + "]";
		if(status.equalsIgnoreCase("FAIL")) { System.err.println(message); } else { System.out.println(message);}	

	}
	public synchronized void printEvent(String message) {
		message = String.format("%-60s", message);
		System.out.print(message);

	}
	public synchronized void printStatus(String status) {
		if(status.equalsIgnoreCase("FAIL")) { System.err.println("[FAIL]"); } else { System.out.print("["+status.toUpperCase()+"]\n");}	

	}
	
	public synchronized void print(String message) {
		message = String.format("%-60s", message);
		System.out.println(message);
		
	}
	
	
	

}
