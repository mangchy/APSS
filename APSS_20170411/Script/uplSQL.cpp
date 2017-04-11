#language C++Script
#include "GlobalScripts.pas", "dg.cpp", "JJUtils.cpp", "GlobalJJ.cpp"

void uplSQL()
{
	//ShowMessage(sqlscript);
    
    //if (opend) ShowMessage("DB Connected : OK");  
    try
	{
		string namedb = "readSQL";
		string currdate = FormatDateTime("YYYYMMDD", Now());
		string sStart = frmScreen1.act_start.Text;
		string sEnd = FormatDateTime("YYYY-MM-DD HH:NN:SS" + ".000", Now());
		string mcid = frmScreen1.cbxMC.Text;
		string soid = frmScreen1.soid.Text;
		string serno = frmScreen1.serno.Text;
		string sec = DecodeSec(sStart);	 
			
		boolean opend;
		opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname); 
		
		string sqlquery = "SELECT SO_ID, FACTORY, RESOURCE_CD, ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE ";
				sqlquery += "FROM DATA_SO ";
				sqlquery += "WHERE SO_ID='" + soid + "'";
        DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		
			String s1 = DBFieldByName_String(namedb, "SO_ID");
			String s2 = DBFieldByName_String(namedb, "FACTORY");
			String s3 = DBFieldByName_String(namedb, "RESOURCE_CD");
			String s4 = DBFieldByName_String(namedb, "ZONE_CD");
			String s5 = DBFieldByName_String(namedb, "IPP_LINE_CD");
			String s6 = DBFieldByName_String(namedb, "MACHINE_CD");
			String s7 = DBFieldByName_String(namedb, "STATION_CD");
			String s8 = DBFieldByName_String(namedb, "STATION_TYPE");
			
		DBLast(namedb);
		//DBDisconnect(namedb, true);
		
		string sqlinsert = "INSERT INTO DATA_RST (RST_YMD, SO_ID, SER_NO, FACTORY, RESOURCE_CD, ";
		sqlinsert += "ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE, START_DATE, END_DATE, NET_WRK_SEC, ";
		sqlinsert += "PRS_QTY, CNT_QTY, REASON, INJECTOR_CD, REMARK, UPD_USER, UPD_YMD) ";
	    sqlinsert += "VALUES ('" + currdate +"', '" + soid + "', '" + serno + "', '" + s2 + "', '" + s3 + "', '" + s4 + "', ";
		sqlinsert += "'" + s5 + "', '" + s6 + "', '" + s7 + "', '" + s8 + "', " ;
		sqlinsert += "'" + sStart + "', '" + sEnd + "', '" + sec + "', '9', '1', '00', '#1', '', '" + mcid + "', '" + sEnd + "')";
		//_TRACE(ICO_ERROR, sqlinsert);
		//ShowMessage(sqlinsert);
		//boolean opend;
		//opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname); 
		//string namedb = "writeSQL";
        DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlinsert);
		DBScriptExecute(namedb);
		
		
		DBDisconnect(namedb, true);
		//DBScriptClear(namedb);
		//ShowMessage("saved");		
	}
	except	
	{
		ShowMessage(ExceptionMessage);
	}
	
}


{
	
}