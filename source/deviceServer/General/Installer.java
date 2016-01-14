/* 
 * This class is used to setup all external resources that is required for Device server to run
 */

package edu.tamu.cs.codesign.General;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class Installer {

	SysUtils utils = new SysUtils();
	
	public boolean runFirstTimeSetup() {
		
		if(!_DB_INSTALLER()) {
			utils.printDebug("DBInstaller failed!");
			return false;
		}
		return true;
		
	}
	
	public boolean _DB_INSTALLER() {
			Connection conn = null;
			Statement stmt = null;
			
			String JDBC_DRIVER = CONF.__JDBC_DRIVER;
			String DB_URL = CONF.__DB_URL;
			String USER = CONF.__SQLUSER;
			String PASS = CONF.__SQLPASS;
			String sql;
			String dbName = CONF.__SQLDBNAME;
			
			utils.print("Attempting to install Database Connectors ................");
			utils.printEvent("Regestering JDBC Driver");
			try {
				Class.forName(JDBC_DRIVER);
				utils.printStatus("OK");
			}
			catch(Exception e) {
				utils.printStatus("FAIL");
				return false;
			}
			
			utils.printEvent("Attempting connection to database");
			try {
				conn = DriverManager.getConnection(DB_URL, USER, PASS);
				utils.printStatus("OK");
			}
			catch (Exception e){
				e.printStackTrace();
				utils.printStatus("FAIL");
				return false;
			}
			utils.printEvent("Creating Structure");
		   try{
			  
		      stmt = conn.createStatement();
		      sql = "CREATE DATABASE "+dbName;
		      stmt.executeUpdate(sql);
		   }
		   catch(SQLException se) {
			   utils.printStatus("FAIL");
			   utils.print("\n** A previous installation is detected. \nPlease use setup --force to overwrite all existing installation.");
				 //se.printStackTrace();
				   return false;
		   }
		   try{
		      // CREATE DATABASE
		      
		      
		      //USE DATABASE
		      sql = "USE "+dbName;
		      stmt.executeUpdate(sql);
		      
		      // CREATE TABLE LAST SEEN
		      
		      sql = "CREATE TABLE LastSeen"+
		    		  "("+
		    		  		"DeviceID VARCHAR (20),"+
		    		  		"LastSeen VARCHAR (50)" +
		    		  ")";
		      
		      stmt.executeUpdate(sql); 
		      utils.printStatus("SUCCESS");
		   }catch(SQLException se){
			   utils.printStatus("FAIL");
			 //se.printStackTrace();
			   return false;
		       
		   }
		   finally{
		      //finally block used to close resources
		      try{
		         if(stmt!=null)
		            stmt.close();
		      }catch(SQLException se2){
		      }// nothing we can do
		      try{
		         if(conn!=null)
		            conn.close();
		      }catch(SQLException se){
		         se.printStackTrace();
		      }//end finally try
		   }//end try
		
		return true;
	}
}
