package edu.tamu.cs.codesign.General;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;


public class DB {
	
	Connection conn = null;
	Statement stmt = null;
	String JDBC_DRIVER = CONF.__JDBC_DRIVER;
	String DB_URL = CONF.__DB_URL;
	String IOT_DB_URL = CONF.__IOT_DB_URL;
	String USER = CONF.__SQLUSER;
	String PASS = CONF.__SQLPASS;
	String sql;
	String dbName = CONF.__SQLDBNAME;
	
	private Connection instOfConn = null;
	public DB() {
		
	}
	
	public Connection connect(String url) {
		try {
			Class.forName(JDBC_DRIVER);
		}
		catch(Exception e) {
			return null;
		}
		
		try {
			conn = DriverManager.getConnection(url, USER, PASS);

		}
		catch (Exception e){
			System.out.println(e.toString());
			e.printStackTrace();
			return null;
		}
		return conn;
	}
	
	public boolean checkConnection() {
		if(connect(IOT_DB_URL) != null) return true;
		return false;
	}
	
	public boolean runSQL(String query, String... vals) {
		if(instOfConn==null) instOfConn = connect(IOT_DB_URL);
		
		try {
		switch(query) {
		/*
		 * Raw packet log. varargs, device ID, Packet Type, Payload
		 */
		case "raw-packet-log":	
				stmt = conn.createStatement();
				sql = "INSERT INTO Raw_Packets "
						+ "(packet_type, device_id, payload) "
						+ "VALUES (?,?,?)";
				PreparedStatement preparedStatement = conn.prepareStatement(sql);
				for(int i=0; i<vals.length; i++) 
					preparedStatement.setString(i+1,vals[i]);
				preparedStatement.execute();
			break;
		case "session-create":
				sql="UPDATE endDevice SET status_id = 1 WHERE dev_id = ?";
				preparedStatement = conn.prepareStatement(sql);
				preparedStatement.setString(1, vals[0]);
				preparedStatement.executeUpdate();
				
			break;
		case "session-terminate":
			sql="UPDATE endDevice SET status_id = 2 WHERE dev_id = ?";
			preparedStatement = conn.prepareStatement(sql);
			preparedStatement.setString(1, vals[0]);
			preparedStatement.executeUpdate();
			break;
		}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	

}
