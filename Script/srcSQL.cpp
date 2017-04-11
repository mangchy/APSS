#language C++Script
#include "GlobalScripts.pas", "dg.cpp", "JJUtils.cpp", "GlobalJJ.cpp", "RefreshSRC.cpp"


//=======================================================================================
//'ALL', 'MCA16','MCA17','MCA18'
String getMCMachine()
{
	String rlt = "";
	for(int i=1; i<frmScreen1.cbxMC.Items.Count-1; i++)
	{
		rlt += Format("'%s', ", [frmScreen1.cbxMC.Items.Strings[i]]);
	}
	rlt += Format("'%s'", [frmScreen1.cbxMC.Items.Strings[frmScreen1.cbxMC.Items.Count-1]]);
	return rlt;
}


//=======================================================================================
void srcSQL()
{	
	try
	{
		string namedb 	= "readSQL";
		string versionid = frmScreen1.cbxVersionID.Text;
		string versionidPrev = "";
		if(frmScreen1.cbxVersionID.Items.Count > 1) versionidPrev = frmScreen1.cbxVersionID.Items.Strings[1]; 
		string zone 	= frmScreen1.lbZone.Caption;
		string mcs 		= getMCMachine();

		//ShowMessage(versionidPrev);
		boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
		
		/*string sqlquery = "";	
			   sqlquery += "SELECT A.SO_ID, A.VERSION_ID, A.RESOURCE_CD, A.STATION_CD, A.STATION_TYPE, A.MCS_1_CD, A.COLOR_NAME, A.ZONE_CD, A.MACHINE_CD, A.SORT_KEY, ";
			   sqlquery += "A.START_DATE, A.END_DATE, A.MOLD_ID, ";
			   sqlquery += "A.CNT_QTY, A.OSND_CNT_QTY, A.PRS_QTY, ";
			   sqlquery += "B.RST_CNT, B.RST_OSND_CNT, ";
			   sqlquery += "B.RST_START_DATE, B.RST_END_DATE ";
		  sqlquery += "FROM DATA_SO A ";
			   sqlquery += "LEFT  OUTER JOIN ( ";
								sqlquery += "SELECT SO_ID, ";
									   sqlquery += "SUM(CASE REASON WHEN '00' THEN CNT_QTY ELSE 0 END) AS RST_CNT, ";
									   sqlquery += "SUM(CASE REASON WHEN '00' THEN 0 ELSE CNT_QTY END) AS RST_OSND_CNT, ";
									   sqlquery += "MIN(START_DATE) AS RST_START_DATE, ";
									   sqlquery += "MAX(END_DATE) AS RST_END_DATE ";
								  sqlquery += "FROM DATA_RST ";
								 sqlquery += "WHERE VERSION_ID = '" + versionid + "'"; //IPI-170211-M-001' ";
								   sqlquery += "AND ZONE_CD = '" + zone + "'"; //Z003' ";
								   sqlquery += "AND MACHINE_CD IN (" + mcs + ") ";//'MCA16','MCA17','MCA18') ";
								 sqlquery += "GROUP BY SO_ID ";
								sqlquery += ") B   ON A.SO_ID = B.SO_ID ";
		 sqlquery += "WHERE A.VERSION_ID = '" + versionid + "'"; //IPI-170211-M-001' ";
		   sqlquery += "AND A.ZONE_CD    = '" + zone + "'"; //Z003' ";
		   sqlquery += "AND A.MACHINE_CD IN (" + mcs + ") ";//'MCA16','MCA17','MCA18') ";
		   if(frmScreen1_4.cbxQuery.Checked == false) sqlquery += "AND (A.CNT_QTY + ISNULL(A.OSND_CNT_QTY, 0)) > (ISNULL(B.RST_CNT, 0) + ISNULL(B.RST_OSND_CNT, 0)) ";
		 sqlquery += "ORDER BY A.SORT_KEY, A.STATION_CD,  A.SO_ID, A.STATION_TYPE" ; //A.VERSION_ID, A.RESOURCE_CD, A.SORT_KEY ";
		 */
		 
		string sqlquery = "";	
			sqlquery += "SELECT * FROM (";
			sqlquery += "SELECT A.SO_ID, A.VERSION_ID, A.RESOURCE_CD, A.STATION_CD, A.STATION_TYPE, A.COLOR_NAME AS MCS_COLOR, A.ZONE_CD, A.MACHINE_CD, A.SORT_KEY, A.HH, ";
			sqlquery += "A.START_DATE, A.END_DATE, RTRIM(A.MOLD_CD) + ' ' + RTRIM(A.MOLD_SIZE_CD) AS MOLD_ID, ";
			sqlquery += "A.CNT_QTY, A.OSND_CNT_QTY, A.OSND_PRS_QTY, A.PRS_QTY, ";
			sqlquery += "B.RST_CNT, B.RST_OSND_CNT, ";
			sqlquery += "B.RST_START_DATE, B.RST_END_DATE ";
			sqlquery += "FROM DATA_SO  A ";
			sqlquery += "LEFT  OUTER  JOIN ( ";
                        sqlquery += "SELECT SO_ID, ";
                               sqlquery += "SUM(CASE REASON WHEN  '00' THEN CNT_QTY ELSE 0 END) AS RST_CNT, ";
                               sqlquery += "SUM(CASE REASON WHEN  '00' THEN 0 ELSE CNT_QTY END) AS RST_OSND_CNT, ";
                               sqlquery += "MIN(START_DATE) AS RST_START_DATE, ";
                               sqlquery += "MAX(END_DATE) AS RST_END_DATE ";
                          sqlquery += "FROM DATA_RST ";
                         sqlquery += "WHERE VERSION_ID  = '" + versionidPrev + "' ";//IPI-170209-M-001'  ";/* 전일*/
                           sqlquery += "AND ZONE_CD  = '" + zone + "'";//Z003' ";
                           sqlquery += "AND MACHINE_CD  IN (" + mcs + ") ";//'('MCA16','MCA17','MCA18')
                         sqlquery += "GROUP BY  SO_ID ";
                        sqlquery += ") B    ON A.SO_ID = B.SO_ID ";
			sqlquery += "WHERE A.VERSION_ID = '" + versionidPrev + "' ";//IPI-170209-M-001' ";
			   sqlquery += "AND A.ZONE_CD    = '" + zone + "' ";//Z003'
			   sqlquery += "AND A.MACHINE_CD IN (" + mcs + ") ";//('MCA16','MCA17','MCA18')
			   sqlquery += "AND (A.CNT_QTY + ISNULL(A.OSND_CNT_QTY,0)) > (ISNULL(B.RST_CNT,0) + ISNULL(B.RST_OSND_CNT,0)) ";
			sqlquery += "UNION ";
			sqlquery += "SELECT A.SO_ID, A.VERSION_ID, A.RESOURCE_CD, A.STATION_CD, A.STATION_TYPE, A.COLOR_NAME AS MCS_COLOR, A.ZONE_CD, A.MACHINE_CD, A.SORT_KEY, A.HH, ";
				   sqlquery += "A.START_DATE, A.END_DATE, RTRIM(A.MOLD_CD) + ' ' + RTRIM(A.MOLD_SIZE_CD) AS MOLD_ID, ";
				   sqlquery += "A.CNT_QTY, A.OSND_CNT_QTY, A.OSND_PRS_QTY, A.PRS_QTY, ";
				   sqlquery += "B.RST_CNT, B.RST_OSND_CNT, ";
				   sqlquery += "B.RST_START_DATE, B.RST_END_DATE ";
			  sqlquery += "FROM DATA_SO  A ";
				   sqlquery += "LEFT  OUTER  JOIN ( ";
									sqlquery += "SELECT SO_ID, ";
										   sqlquery += "SUM(CASE REASON WHEN  '00' THEN CNT_QTY ELSE 0 END) AS RST_CNT, ";
										   sqlquery += "SUM(CASE REASON WHEN  '00' THEN 0 ELSE CNT_QTY END) AS RST_OSND_CNT, ";
										   sqlquery += "MIN(START_DATE) AS RST_START_DATE, ";
										   sqlquery += "MAX(END_DATE) AS RST_END_DATE ";
									  sqlquery += "FROM DATA_RST ";
									 sqlquery += "WHERE VERSION_ID  = '" + versionid + "' ";//IPI-170211-M-001'  /* 금일*/
									   sqlquery += "AND ZONE_CD  = '" + zone + "'";//Z003'
									   sqlquery += "AND MACHINE_CD  IN (" + mcs + ") ";//('MCA16','MCA17','MCA18')
									 sqlquery += "GROUP BY  SO_ID ";
									sqlquery += ") B    ON A.SO_ID = B.SO_ID ";
			sqlquery += "WHERE A.VERSION_ID = '" + versionid + "' ";//IPI-170211-M-001'
			   sqlquery += "AND A.ZONE_CD    = '" + zone + "'";//Z003'
			   sqlquery += "AND A.MACHINE_CD IN (" + mcs + ") ";//('MCA16','MCA17','MCA18')
			sqlquery += ") K ";
			//sqlquery += "ORDER BY VERSION_ID, RESOURCE_CD,  SORT_KEY ";
			//sqlquery += "ORDER BY SORT_KEY, STATION_CD,  SO_ID, STATION_TYPE" ; 
			sqlquery += "STATION_CD, VERSION_ID, RESOURCE_CD, SORT_KEY" ; 
		
		//ShowMessage(sqlquery);
		 
		DBScriptClear(namedb);
		DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		int cnt = DBRecordCount(namedb);
		
		frmScreen1.dhGrid1.UpdateStart(1);
		frmScreen1.dhGrid1.Clear();
		for(int i=0; i<cnt; i++)
		{
			String s1  = DBFieldByName_String(namedb, "SO_ID");
			String s2  = DBFieldByName_String(namedb, "RESOURCE_CD");
			String station_cd = DBFieldByName_String(namedb, "STATION_CD");
			String s3  = DBFieldByName_String(namedb, "STATION_TYPE");
			String s4  = DBFieldByName_String(namedb, "MOLD_ID");
			//String s6  = DBFieldByName_String(namedb, "MCS_1_CD");
			String s7  = DBFieldByName_String(namedb, "MCS_COLOR");
			String s8  = DBFieldByName_String(namedb, "START_DATE");
			String s9  = DBFieldByName_String(namedb, "END_DATE");
			String s10 = DBFieldByName_String(namedb, "CNT_QTY");
			String sPrsQty = DBFieldByName_String(namedb, "PRS_QTY");
			String s11 = DBFieldByName_String(namedb, "RST_CNT");
			String s12 = DBFieldByName_String(namedb, "OSND_PRS_QTY");
			String s13 = DBFieldByName_String(namedb, "OSND_CNT_QTY");			
			String s14 = DBFieldByName_String(namedb, "RST_START_DATE");
			String s15 = DBFieldByName_String(namedb, "RST_END_DATE");			
			String sort_key   = DBFieldByName_String(namedb, "SORT_KEY");//13 
			String machine_cd = DBFieldByName_String(namedb, "MACHINE_CD");			
			
			int inorplncnt = StrToIntDef(s10, 0);
			int inoractcnt = StrToIntDef(s11, 0);
			int iosdplncnt = StrToIntDef(s12, 0);
			int iosdactcnt = StrToIntDef(s13, 0);
			
			TColor clrRow = clWhite;
			if(inorplncnt + iosdplncnt == inoractcnt + iosdactcnt)
			{
				SetDebug(Format("completed work : Grid row(%d), %d,%d, %d, %d", [i, inorplncnt, iosdplncnt, inoractcnt, iosdactcnt]), COLOR_WORK_FINISH);
				clrRow = COLOR_WORK_FINISH;
			}
		String sInjector_cd;
		if(s3 == "L") sInjector_cd = "01";
		else          sInjector_cd = "02";
			int row = dataGrid(
				s2, 						//1
				s4, 						//2
				s7, 						//s6 + " " + s7, 				//3
				Copy(s8, 9, Length(s8)), 	//4
				Copy(s9, 9, Length(s9)), 	//5
				inorplncnt, 		//6
				inoractcnt, 		//7
				iosdplncnt, 		//8
				iosdactcnt, 
				sInjector_cd, 
				Copy(s14, 9, Length(s14)),
				Copy(s15, 9, Length(s15)), //RST_END_DATE
				s1, //SO_ID
				"", 
				sort_key, 
				s3, 
				station_cd,
				s4,
				s7,
				machine_cd,
				sPrsQty,
				clrRow,
				clBlack, 
				false);
			
			DBNext(namedb);			
		} 
			
		DBDisconnect(namedb, true); 

		frmScreen1.dhGrid1.UpdateStart(0);
		TimerRepaintGrid.Enabled = true;
		
		frmScreen1.lbResult.Caption = IntToStr(cnt); 
        
		//ShowMessage("sql end");
        //check LP
        frmScreen1_2.prgCheck.MaxValue  = 100;
        frmScreen1_2.prgCheck.UserValue = 0;
        
        frmScreen1_2.Show();
		
		SetUpdateTagTime();
        TimerCheck.Enabled = true;		
		//TimerRefresh.Enabled = true;
	}
	except
	{
		ShowMessage(ExceptionMessage);
	}
}

{
	
}