#include "GlobalScripts.pas", "JJUtils.cpp", "LPComm.cpp"

string gSvr_type = ""; //"SQL SERVER";
string gSvr 	 = ""; //"(local)";
string gPort 	 = ""; //"1433";
string gUid 	 = ""; //"sa";
string gPwd 	 = ""; //"system" ;
string gDbname 	 = ""; //"IP_SS";
string gZone 	 = ""; //"Z001";
String gMachinePrefix = "";

int    gBarcodeStep 	= 0;//0-none, 1-machine
string gBarcodeMCA 		= "";
int    gBarcodeStation 	= 0;
int    gBarcodeSide 	= 0;


#define ALL_STATION  	24		//STATION 8 X 3 SET
#define ALL_ORDERS 	 	48		//EATCH STATION PAIR (L, R)

#define LP_NUM			2

#define STATION_NUM 	16		//LP 2ea ....test(L, R)
#define MACHINE_NUM		3

#define LP_TOTALNUM		LP_NUM * MACHINE_NUM


int 		gDownloadNum;
int 		gDownloadedIdx;
int 		gDownloadData[ALL_ORDERS];
Variant 	gSO_ID[ALL_ORDERS];
    

int 		gLActCnt[ALL_STATION];
int 		gLPlnCnt[ALL_STATION];
int 		gRActCnt[ALL_STATION];
int 		gRPlnCnt[ALL_STATION];

int			gWorkingRow[ALL_ORDERS];//1L,R,2L,R,......
int			gWorkingNormalPln[ALL_ORDERS];//1L,R,2L,R,......
int			gWorkingNormal[ALL_ORDERS];//1L,R,2L,R,......
int			gWorkingOSnDPln[ALL_ORDERS];//1L,R,2L,R,......
int			gWorkingOSnD[ALL_ORDERS];//1L,R,2L,R,......
int			gWorkingSOID[ALL_ORDERS];//

int			gCurrentStation = -1;
int			gCurrentSortKey = -1;

int			gQueryWithCompleted = 0;

int			grefreshDatabase = 0;//2:30 pm automatically  refresh VersionID and reload Database
int			gUseManualCmd    = 0;

//=============================================================GRID COLUMN
#define COLUMN_VERSION_ID	0
#define COLUMN_MOLDSIZE		1
#define COLUMN_MOLDCOLOR	2
#define COLUMN_NORPLNCNT	5
#define COLUMN_NORACTCNT	6
#define COLUMN_OSNDPLNCNT	7
#define COLUMN_OSNDACTCNT	8
#define COLUMN_INJECTOR		9
#define	COLUMN_SO_ID		12		//change column position so_id 17->12
#define	COLUMN_SORT_KEY		14
#define	COLUMN_SHOESSIDE	15
#define	COLUMN_STATION		16
#define	COLUMN_COLOR		18
#define	COLUMN_MACHINE		19
#define	COLUMN_PRS_QTY		20

#define COLUMN_MAX			20

//=============================================================Grid Row Color
#define COLOR_WORK_FINISH		clLime
#define COLOR_WORK_PROGRESS		clYellow
#define COLOR_WORK_NEXT			clAqua

//=============================================================Reason
#define REASON_NORMAL_COUNT	 "00"
#define REASON_OSND_COUNT	 "01"

//=============================================================
#define COLUMN_ACTDATESTART	10
#define COLUMN_ACTDATEEND	11

//=============================================================Alarm Type
#define ALARM_MOLDCHANGE	"001"
#define ALARM_ENDORDER		"002"
#define ALARM_DOWNTIME		"003"

#define ALARM_MOLDCHANGE_D	"Mold Change"
#define ALARM_ENDORDER_D	"End Order"
#define ALARM_DOWNTIME_D	"Down Time"

//=============================================================
#define DOOR_AUTO			0
#define DOOR_MANUAL			1


int 		gTagPRSQTY[ALL_ORDERS];
int 		gTagNorP[ALL_ORDERS];
int 		gTagNorA[ALL_ORDERS];
int 		gTagOsdP[ALL_ORDERS];
int 		gTagOsdA[ALL_ORDERS];
int 		gTagSOID[ALL_ORDERS];
int 		gTagMold[ALL_ORDERS];
int 		gTagColor[ALL_ORDERS];
int 		gTagTimeWrite[ALL_ORDERS];

int			gTagWrite[LP_TOTALNUM];//LP 2 * 3


int			gLPSOIDZeroNum;

TDateTime	gTagUpdateTime1[ALL_ORDERS];
TDateTime	gTagUpdateTime2[ALL_ORDERS];
TDateTime	gTagUpdateTime3[ALL_ORDERS];
TDateTime	gTagUpdateTimeSOID[ALL_ORDERS];

TDateTime	gStartWorkTime[ALL_ORDERS];
//TDateTime	gEndWorkTime[MACHINE_NUM];
//TDateTime	gTimeStamp;
TDateTime	gBarcodeTime; 


#define DOOR_NUM			24

int			gTagDoor[DOOR_NUM];//LP 1=4 door d/i
TDateTime	gTagUpdateTimeDoor[DOOR_NUM];
int			gDoorStatus[DOOR_NUM];

#define DOOR_OPEN			0
#define DOOR_CLOSE			1

string gRstStartDt 	= "";
string gRstEndDt 	= "";
string gAct 		= "";
string gOSD 		= "";

String gMoldID[STATION_NUM];	//barcode data = mold_id
String gDoorStatusDescript[2];// = {"Open", "Close"};
String gDoorAMDescript[2];// = {"Auto", "Manual"};

//=======================================================================================
void SetUpdateTagTime()
{
	int i;
	for(i=0; i<STATION_NUM; i++)
	{
		gTagUpdateTime1[i] = GetTagUpdateTime(gTagNorA[i]);
        gTagUpdateTime2[i] = GetTagUpdateTime(gTagOsdA[i]);   
		gTagUpdateTime3[i] = GetTagUpdateTime(gTagOsdP[i]);
	}
	
	for(i=0; i<8; i++)
	{
		gTagUpdateTimeDoor[i] = GetTagUpdateTime(gTagDoor[i]);
	}
	
}

//=======================================================================================
void SetDebug(String message, TColor aTextColor=clBlack)
{
	int iDebugRow = frmScreen1_3.dhGrid1.SetAddRow(false);
	frmScreen1_3.dhGrid1.SetCellData(iDebugRow, 0, DateTimeToStr(Now), false);
	frmScreen1_3.dhGrid1.SetCellDataColor(iDebugRow, 1, message, true, aTextColor);//Format("soid not found : %d, %d", [i,iSOID]), true);
	frmScreen1_3.dhGrid1.LastRow;
}

//=======================================================================================
int getMachineIndex(String aMc)
{
	//ShowMessage(Format("%d, %s", [frmScreen1.cbxMC.Items.Count, aMc]));
	for(int i=1; i<frmScreen1.cbxMC.Items.Count; i++)
	{
		if(frmScreen1.cbxMC.Items.Strings[i] == aMC) return i-1;//1 --> all machine
	}
	
	return -1;
}

//=======================================================================================
int getGridRowFromSOID(String tarMachine, int aOrderPos, int aSOID)
{
	//string s1 = "";
	//for(int i=1; i<frmScreen1.cbxMC.Items.Count; i++)
//	{
	//	s1 += frmScreen1.cbxMC.Items.Strings[i] + ", ";// == aMC) return i-1;//1 --> all machine
	//}
	//ShowMessage(s1);
	
	
	int row_cnt = frmScreen1.dhGrid1.GetRowCount;
	for(int grid_row=0; grid_row<row_cnt; grid_row++)
	{		
		String so_id 	= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_SO_ID);	
		String machine  = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_MACHINE);
	
		//int dbMCIndex = getMachineIndex(machine);
		//int targetMCIndex = Int(aOrderPos/16);//16 --> 1 mca = 2LP, 1LP=8 order   each machine have two LP,
		//if(dbMCIndex == targetMCIndex)
		if(machine == tarMachine)
		{
//ShowMessage(s1 + machine + ", " + IntToStr(dbMCIndex));	
			if(aSOID == StrToIntDef(so_id, -1)) return grid_row;
		}
	}
	SetDebug(Format("SOID=%d, not found soid : %d", [aSOID, aOrderPos]), clRed);
	return -1;
}


//=======================================================================================
int getGridRow(String tarMachine, int aOrderPos)
{
	//int imin_sort_key = 999999999;
	int imin_soid = 999999999;
	int row_cnt2 = frmScreen1.dhGrid1.GetRowCount;
	int row_result = -1;
	for(int grid_row2=0; grid_row2<row_cnt2; grid_row2++)
	{
		String station  = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_STATION);	
		String machine  = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_MACHINE);	
		
		int istation    = StrToInt(station) - 1;//db sation ... 01, 02, 03 ....
		int aGridStation = Int(aOrderPos/2);
		//int dbMCIndex 	= getMachineIndex(machine);
		//int targetMCIndex = Int(aOrderPos/16);//2 --> each machine have two LP.
		//if((dbMCIndex == targetMCIndex) && (aGridStation == istation))
		if((machine == tarMachine) && (aGridStation == istation))
		{
		//ShowMessage(Format("found %d, %d, %s", [grid_row2, dbMCIndex, machine]));
			String nor_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_NORPLNCNT);	
			String nor_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_NORACTCNT);	
			String osd_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_OSNDPLNCNT);	
			String osd_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_OSNDACTCNT);	

			int inorplncnt = StrToIntDef(nor_plncnt, 0);
			int inoractcnt = StrToIntDef(nor_actcnt, 0);
			int iosdplncnt = StrToIntDef(osd_plncnt, 0);
			int iosdactcnt = StrToIntDef(osd_actcnt, 0);
			
			int pln_sum = inorplncnt + iosdplncnt;
			int act_sum = inoractcnt + iosdactcnt;
		
			if(pln_sum == act_sum)//finish order
			{			
				continue;
			}
			
			String side    	= frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SHOESSIDE);	
			String soid 	= frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SO_ID);	
			String sort_key = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SORT_KEY);	
			int isoid   	= StrToInt(soid);
			int isort_key   = StrToInt(sort_key);
			int iSide = aOrderPos%2;
			if(((iSide == 0) && (side == "L")) || ((iSide == 1) && (side == "R")))
			{
				//ShowMessage(Format("found %d/%d, %d, %s, %d, %s, %d, %s, %d", [grid_row2, row_cnt2, dbMCIndex, machine, istation, side, iSide, sort_key, imin_sort_key]));
				
				if(imin_soid > isoid)
				{
					imin_soid = isoid;
					row_result = grid_row2;
				}
				else
				{
					break;
				}
				
				//if(imin_sort_key > isort_key) 
				//{
				//	imin_sort_key = isort_key;
				//	row_result = grid_row2;
				//}
				//else
				//{
				//	break;
				//}
			}	
		}
	}
	return row_result;
}

//=======================================================================================
int UpdateSOID()
{
	frmScreen1_4.dhGrid2.UpdateStart(1);
    frmScreen1_4.dhGrid2.Clear();
	int iSOIDZeroNum = 0;
	
	int row = frmScreen1_4.dhGrid2.SetAddRow(false);
	frmScreen1_4.dhGrid2.SetCellDataColor2(row, 0, "Machine", false, clBlack, clWhite);
	frmScreen1_4.dhGrid2.SetCellDataColor2(row, 1, frmScreen1_4.cbxMCA1.Text, false, clBlack, clWhite);
	frmScreen1_4.dhGrid2.SetCellDataColor2(row, 2, "", false, clBlack, clWhite);
	
	for(int i=0; i<STATION_NUM; i++)
    {
		int iSOID = GetTagValueI(gTagSOID[i]);	
        row = frmScreen1_4.dhGrid2.SetAddRow(false);
		String sStationName = IntToStr(Int(i/2) + 1);
		if(i%2 == 0) sStationName += "L";
		else         sStationName += "R";
        frmScreen1_4.dhGrid2.SetCellData(row, 0, sStationName, false);
		frmScreen1_4.dhGrid2.SetCellData(row, 1, Format("%d", [iSOID]), false);
		frmScreen1_4.dhGrid2.SetCellData(row, 2, "", false);
		frmScreen1_4.dhGrid2.SetCellData(row, 3, gMoldID[i], false);//mold_id=barcode
		
		if(iSOID == 0) iSOIDZeroNum++;
	}
	frmScreen1_4.dhGrid2.UpdateStart(0);    
	
	return iSOIDZeroNum;
}

//=======================================================================================
int getNextOrderRow(String aMachine, int aRow)
{
	String station 	  = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_STATION);
	String soid 	  = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_SO_ID);	
	String sort_key	  = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_SORT_KEY);
	String side 	  = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_SHOESSIDE);
	
	int istation  = StrToInt(station) - 1;
	int isoid 	  = StrToInt(soid);	
	int isort_key = StrToInt(sort_key);
	
	int row_cnt = frmScreen1.dhGrid1.GetRowCount;
	//for(int grid_row2=0; grid_row2<row_cnt; grid_row2++)
	for(int grid_row2=aRow+1; grid_row2<row_cnt; grid_row2++)
	{
		String station2	 = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_STATION);
		int istation2 = StrToInt(station2) - 1;	
		if(istation != istation2) continue;
		
		String machine  = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_MACHINE);	
		if(aMachine != machine) continue;
		
		String soid2 	 = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SO_ID);	
		String sort_key2 = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SORT_KEY);	
		String side2 	 = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_SHOESSIDE);	
	
		String nor_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_NORPLNCNT);	
		String nor_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_NORACTCNT);	
		String osd_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_OSNDPLNCNT);	
		String osd_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row2, COLUMN_OSNDACTCNT);	

		int isoid2 	   = StrToInt(soid2);	
		int isort_key2 = StrToInt(sort_key2);	
		int inorplncnt = StrToIntDef(nor_plncnt, 0);
		int inoractcnt = StrToIntDef(nor_actcnt, 0);
		int iosdplncnt = StrToIntDef(osd_plncnt, 0);
		int iosdactcnt = StrToIntDef(osd_actcnt, 0);

		int pln_sum = inorplncnt + iosdplncnt;
		int act_sum = inoractcnt + iosdactcnt;
			
		//if((isort_key < isort_key2) && (side == side2) && (pln_sum > act_sum))
		if((soid < soid2) && (side == side2) && (pln_sum > act_sum))
		{
			SetDebug(Format("#%d, SO_ID=%s, Next Order : Row=%d, SORT_Key=%s", [istation2+1, soid2, grid_row2, sort_key2]));	
			return grid_row2;
		}
	}

	return -1;
}



//=======================================================================================
/*/check Mold changing next order
void checkMoldChange(int aGridRow)
{
	String mold = frmScreen1.dhGrid1.GetCellData(aGridRow, COLUMN_MOLDSIZE);	
	String machine  = frmScreen1.dhGrid1.GetCellData(aGridRow, COLUMN_MACHINE);	
	int iNextRow = getNextOrderRow(machine, aGridRow);	
	if(iNextRow > -1)	
	{
		String mold2 = frmScreen1.dhGrid1.GetCellData(iNextRow, COLUMN_MOLDSIZE);	
		
		String smoldfront1 = Copy(mold, 1, 8);
		String smoldfront2 = Copy(mold2, 1, 8);
		
		if(smoldfront1 != smoldfront2)
		{
			int ilen1 = Length(mold) - 1;
			int ilen2 = Length(mold2) - 1;
			
			String smoldrear1 = Copy(mold, ilen1-3, ilen1);
			String smoldrear2 = Copy(mold2, ilen2-3, ilen2);
			
			
			//LP_SetMoldChingSatusON
		}		
	}
}*/


//==================================
int IsWorkingOrder(int aGridRow)
{
	int iGridRow;
	String soid  = frmScreen1.dhGrid1.GetCellData(aGridRow, COLUMN_SO_ID);
	int isoid2	= StrToIntDef(soid, 0);
	for(int i=0; i<STATION_NUM; i++)
	{
		iGridRow = gWorkingRow[i];
		if(iGridRow == -1) continue;
	
		int iSOID = GetTagValueI(gTagSOID[i]);	
		if(iSOID == isoid2) return 1;
	}
	
	return 0;
}


//==================================
String GetSelectedGrid()
{
    //String ss = "";
	String soid_sql = "";
        
    int iSel[1000]; 
	String sSO_ID[1000];
    iSel[0] = frmScreen1.dhGrid1.GetFirstSelectedRow();  
	SetDebug(Format("select 1 : %d", [iSel[0]]));
    //if(iSel[0] > -1)
    //{
		int iDownNum = 0;
        int iCnt = 1;
        while(iSel[iCnt-1] > -1)
        {   
			int grid_row = iSel[iCnt-1];
			
			SetDebug(Format("select 2 : %d, %d, %d", [iSel[0], grid_row, IsWorkingOrder(grid_row)]));
			if(IsWorkingOrder(grid_row) == 1)
			{
				iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
				iCnt++; 
				continue;
			}
			
			String nor_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORPLNCNT);	
			String nor_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORACTCNT);	
			String osd_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDPLNCNT);	
			String osd_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDACTCNT);	

			int inorplncnt = StrToIntDef(nor_plncnt, 0);
			int inoractcnt = StrToIntDef(nor_actcnt, 0);
			int iosdplncnt = StrToIntDef(osd_plncnt, 0);
			int iosdactcnt = StrToIntDef(osd_actcnt, 0);
			
			SetDebug(Format("select 3 : %d, %d", [inorplncnt + iosdplncnt, inoractcnt + iosdactcnt]));
			if(inorplncnt + iosdplncnt == inoractcnt + iosdactcnt) //finish order?
			{
				iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
				iCnt++; 
				continue;
			}
			
			String soid  = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_SO_ID);	
            sSO_ID[iCnt] = soid;
			//ss += Format("%d:%d,%s ", [iCnt-1, iSel[iCnt-1], soid]);
            soid_sql += Format("%s, ", [soid]);
			
            iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
            iCnt++;   
			iDownNum++;
        }           
            
        /*SetDebug(Format("Selected Row : %s and Finish order", [ss]));
		String sqlsoid = Copy(soid_sql, 1, Length(soid_sql)-2);
		insertFinishSQL(sqlsoid);
		
		ShowMessage(Format("Completed finish order : %s", [sqlsoid]));
		
		for(int i=0; i<iDownNum; i++)
		{
			//frmScreen1.dhGrid1.SetCelldataColor2();
		}*/
		
		//return soid_sql;
    //}
	
	return Copy(soid_sql, 1, Length(soid_sql) - 2);
}

void GetDebugactCount(int aRow, String aSub="")
{
	string soid = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_SO_ID);	
	string act_cnt = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_NORACTCNT);	
	SetDebug(Format("--------------Act Cnt : SOID=%s, Row=%d, Act=%s, %s", [soid, aRow, act_cnt, aSub]), clRed);
}

//=======================================================================================
//ready to next order, aStation => start 0
int checkFinish(int soid, int aRow, int aStation, int aAct, int aOSD)
{	
	if((aAct == 0) && (aOSD == 0)) 
	{		
		TimerGetAct.Enabled = false;
		
		SetColorRow(aRow, COLOR_WORK_FINISH);
		
		string sort_key = frmScreen1.dhGrid1.GetCellData(aRow, COLUMN_SORT_KEY);	
		int isort_key   = StrToIntDef(sort_key, 0);
		
		gWorkingRow[aStation] = -1;
		
		//next order download to LP....(station, current sort key)
		gCurrentStation = aStation;
		gCurrentSortKey = isort_key;

		SetDebug(Format("SOID=%d, completed order : station=%d, sortkey=%d", [soid, gCurrentStation+1, gCurrentSortKey]), clRed);
		
		frmScreen1_2.Show();
		TimerCheck.Enabled = true;  
		
		return 1;
	}
	
	return 0;
}

//=======================================================================================
void SetColorRow(int aRow, TColor aColor)
{
	for(int i=0; i<COLUMN_MAX; i++)
		frmScreen1.dhGrid1.SetCelldataColor2(aRow, i, frmScreen1.dhGrid1.GetCellData(aRow, i), false, aColor, clBlack);
}

//=======================================================================================
void UpdateOSnDPlnToDB(int soid, int osd_pln)
{
    try
	{
		string namedb 	= "readSQL";
		boolean opend 	= DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname); 
		
		string sqlquery = "UPDATE DATA_SO ";
				sqlquery += Format("SET OSND_PRS_QTY = %d*PRS_QTY/CNT_QTY, OSND_CNT_QTY = %d ", [osd_pln, osd_pln]);
				sqlquery += Format("WHERE SO_ID = %d ", [soid]);

		DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);

		DBDisconnect(namedb, true);
	}
	except	
	{
		ShowMessage(ExceptionMessage);
	}	
}


//=======================================================================================
//count_type 00-normal, 01-os&d
void InsertWorkCountToDB(int soid, int prs_qty, int workcnt, string zone_cd, string count_type, String aMachine, TDateTime aStartTime, TDateTime aEndTime)
{
    try
	{
		string namedb 		= "readSQL";
		string currdate 	= FormatDateTime("YYYYMMDD", Now());
		string sStartTime	= FormatDateTime("YYYY-MM-DD HH:NN:SS" + ".000", aStartTime);
		string sEndTime  	= FormatDateTime("YYYY-MM-DD HH:NN:SS" + ".000", aEndTime);
		string upd_usr		= aMachine;
		Extended iSec 		= SecondsBetween(aEndTime, aStartTime);//"00";//DecodeSec(aEndTime - sStartTime);	 
		String sec = Int64ToStr(iSec);
			
		boolean opend 	= DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname); 

		//===================================================1/3
		string sqlquery_ser_no = "SELECT MAX(SER_NO) AS SER_NO ";
				sqlquery_ser_no += "FROM DATA_RST ";
				sqlquery_ser_no += Format("WHERE SO_ID = %d ", [soid]);
		DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery_ser_no);
		DBScriptExecute(namedb);
		int cnt = DBRecordCount(namedb);
		String ser_no;
		if(cnt > 0) ser_no = DBFieldByName_String(namedb, "SER_NO");
		int iser_no = StrToIntDef(ser_no, -1) + 1;
		
		//===================================================2/3
		string sqlquery = "SELECT SO_ID, VERSION_ID, FACTORY, RESOURCE_CD, ZONE_CD, IPP_LINE_CD, MOLD_ID, MACHINE_CD, STATION_CD, STATION_TYPE ";
				sqlquery += "FROM DATA_SO ";
				sqlquery += Format("WHERE SO_ID='%d'", [soid]);
        DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);

		String s1 = DBFieldByName_String(namedb, "VERSION_ID");
		String s2 = DBFieldByName_String(namedb, "FACTORY");
		String s3 = DBFieldByName_String(namedb, "RESOURCE_CD");
		String s4 = DBFieldByName_String(namedb, "ZONE_CD");
		String s5 = DBFieldByName_String(namedb, "IPP_LINE_CD");
		String s6 = DBFieldByName_String(namedb, "MACHINE_CD");
		String s7 = DBFieldByName_String(namedb, "STATION_CD");
		String s8 = DBFieldByName_String(namedb, "STATION_TYPE");
		String sMoldID = DBFieldByName_String(namedb, "MOLD_ID");
			
		DBLast(namedb);
		
		String sInjector_cd;
		if(s8 == "L") sInjector_cd = "01";
		else          sInjector_cd = "02";
		
		//int 
		//===================================================3/3
		string sqlinsert = "INSERT INTO DATA_RST (RST_YMD, SO_ID, VERSION_ID, SER_NO, FACTORY, RESOURCE_CD, MOLD_ID, ";
		sqlinsert += "ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE, START_DATE, END_DATE, NET_WRK_SEC, ";
		sqlinsert += "PRS_QTY, CNT_QTY, REASON, INJECTOR_CD, REMARK, UPD_USER, UPD_YMD) ";
	    sqlinsert += "VALUES ('" + currdate +"', '" + IntToStr(soid) + "', '" + s1 + "', '" + IntToStr(iser_no) + "', '" + s2 + "', '" + s3 + "', '" + sMoldID + "', '" + s4 + "', ";
		sqlinsert += "'" + s5 + "', '" + s6 + "', '" + s7 + "', '" + s8 + "', " ;
		sqlinsert += "'" + sStartTime + "', '" + sEndTime + "', '" + sec + "', '" + IntToStr(prs_qty) + "', '" + IntToStr(workcnt) + "', '" + count_type + "', '" + sInjector_cd + "', '', '" + upd_usr + "', '" + sEndTime + "')";

        DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlinsert);
		DBScriptExecute(namedb);		
		
		DBDisconnect(namedb, true);
	}
	except	
	{
		//ShowMessage(ExceptionMessage);
		SetDebug(ExceptionMessage, clRed);
	}	
}


//=======================================================================================
//aSoid = "1234,5678,1235"
void insertFinishSQL(String aSoid)
{
	try
	{
		string namedb 	= "readSQL";		
		boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
	
		string sqlquery = "";	
			sqlquery += "INSERT INTO DATA_RST ";
		sqlquery += "(RST_YMD, SO_ID, SER_NO, ";
		 sqlquery += "VERSION_ID, FACTORY, RESOURCE_CD, ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE, ";
		 sqlquery += "START_DATE, END_DATE, NET_WRK_SEC, ";
		 sqlquery += "PRS_QTY, CNT_QTY, ";
		 sqlquery += "REASON, ";
		 sqlquery += "INJECTOR_CD, ";
		 sqlquery += "MOLD_ID, ";
		 sqlquery += "REMARK, ";
		 sqlquery += "UPD_USER, UPD_YMD) ";
		sqlquery += "SELECT CONVERT(CHAR(8), END_DATE, 112) AS RST_YMD, SO_ID, 1 AS SER_NO, ";
			   sqlquery += "VERSION_ID, FACTORY, RESOURCE_CD, ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE, ";
			   sqlquery += "START_DATE, END_DATE, DATEDIFF(second, START_DATE, END_DATE) AS NET_WRK_SEC, ";
			   sqlquery += "PRS_QTY, CNT_QTY, ";
			   sqlquery += "'00', ";/* 00:FOR NORMAL SHOP ORDER,  01:FOR OS&D */
			   sqlquery += "(CASE STATION_TYPE WHEN 'L' THEN '1' ELSE '2' END) AS INJECTOR_CD, ";
			   sqlquery += "RTRIM(MOLD_CD) + ' ' + RTRIM(MOLD_SIZE_CD) AS MOLD_ID, ";
			   sqlquery += "'USER APPLY', ";
			   sqlquery += "IPP_LINE_CD, GETDATE() ";
		  sqlquery += "FROM DATA_SO ";
		 sqlquery += "WHERE SO_ID IN (" + aSoid + ") ";
		sqlquery += "UNION ";
		sqlquery += "SELECT CONVERT(CHAR(8), END_DATE, 112) AS RST_YMD, SO_ID, 2 AS SER_NO, ";
			   sqlquery += "VERSION_ID, FACTORY, RESOURCE_CD, ZONE_CD, IPP_LINE_CD, MACHINE_CD, STATION_CD, STATION_TYPE, ";
			   sqlquery += "START_DATE, END_DATE, DATEDIFF(second, START_DATE, END_DATE) AS NET_WRK_SEC, ";
			   sqlquery += "OSND_PRS_QTY, OSND_CNT_QTY, ";
			   sqlquery += "'01', ";/* 00:FOR NORMAL SHOP ORDER,  01:FOR OS&D */
			   sqlquery += "(CASE STATION_TYPE WHEN 'L' THEN '1' ELSE '2' END) AS INJECTOR_CD, ";
			   sqlquery += "RTRIM(MOLD_CD) + ' ' + RTRIM(MOLD_SIZE_CD) AS MOLD_ID, ";
			   sqlquery += "'USER APPLY', ";
			   sqlquery += "IPP_LINE_CD, GETDATE() ";
		  sqlquery += "FROM DATA_SO ";
		 sqlquery += "WHERE SO_ID IN (" + aSoid + ") ";
		   sqlquery += "AND OSND_PRS_QTY > 0 ";
		   
		//ShowMessage(sqlquery);   
		
		DBScriptClear(namedb);
		DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		
		DBDisconnect(namedb, true); 
	}
	except
	{
		ShowMessage(sqlquery + "\r\n" + ExceptionMessage);
	}
}

//=======================================================================================
//Station => IPI-MCA17-01-L 
//Mold    => MS004767-JJME006006A
//aAutoManualMode  => remain count ? 0 -> Auto or Manual
void checkMoldChange(int aCurrentRow, int aAutoManualMode)
{
	int row_cnt = frmScreen1.dhGrid1.GetRowCount;
	
	String station   	= frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_STATION);
	String nor_plncnt   = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_NORPLNCNT);
	String osd_plncnt   = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_OSNDPLNCNT);
	String nor_actcnt   = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_NORACTCNT);
	String osd_actcnt   = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_OSNDACTCNT);
	
	int istation 	= StrToInt(station) - 1;
	int inor_plncnt = StrToInt(nor_plncnt);
	int iosd_plncnt = StrToInt(osd_plncnt);
    int inor_actcnt = StrToInt(nor_actcnt);
	int iosd_actcnt = StrToInt(osd_actcnt);

	int sum_pln = inor_plncnt + iosd_plncnt;
	int sum_act = inor_actcnt + iosd_actcnt;
	int remain_count = sum_pln - sum_act;
	
	if(remain_count > 3)//continue work
	{
		LP_SetAutoManual(istation, DOOR_AUTO, gDoorAMDescript[DOOR_AUTO]);
		SetDebug(Format("#%d, Continue work : row=%d, remain count=%d", [istation+1, aCurrentRow, remain_count]));
		return;
	}
	else//check mold type      <= 3
	{
		String version_id  	= frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_VERSION_ID);	//IPI_MCA17_01_L
		String mold_size   	= frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_MOLDSIZE);		//MS249037-1 11
	
		int remain_count2   = remain_count;
		for(int grid_row=aCurrentRow+1; grid_row<row_cnt; grid_row++)//find next order and check mold
		{		
			String version_id2 = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_VERSION_ID);			
			if(version_id != version_id2) 
			{				
				continue;//other station
			}
				
			String mold_size2  = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_MOLDSIZE);							
			if(mold_size == mold_size2)
			{				
				String nor_plncnt2 = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORPLNCNT);
				String osd_plncnt2 = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDPLNCNT);
				//String nor_actcnt2 = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORACTCNT);
				//String osd_actcnt2 = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDACTCNT);
				
				int inor_plncnt2 = StrToInt(nor_plncnt2);
				int iosd_plncnt2 = StrToInt(osd_plncnt2);
				//int inor_actcnt2 = StrToInt(nor_actcnt2);
				//int iosd_actcnt2 = StrToInt(osd_actcnt2);
			
				int sum_pln2 = inor_plncnt2 + iosd_plncnt2;
				remain_count2 += sum_pln2;
				if(remain_count2 > 3)
				{
					LP_SetAutoManual(istation, DOOR_AUTO, gDoorAMDescript[DOOR_AUTO]);
					return;
				}
				else// if(remain_count2 < 3)
				{
					continue;
				}
			}
			else//different mold
			{
				if(remain_count == 0)
				{
					//SetDebug(Format("#%d station, Door : set Auto, %d", [istation+1, aAutoManualMode]));	
					if(gUseManualCmd == 1) LP_SetAutoManual(istation, aAutoManualMode, gDoorAMDescript[aAutoManualMode]);
					return;
				}
				else if(remain_count2 > 3)
				{
					return;//none action
				}
				else if(remain_count2 == 3)//save to alarm table
				{
					String soid = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_SO_ID);
					
					SQLalarmInsert(soid, Now, ALARM_MOLDCHANGE, ALARM_MOLDCHANGE_D);
					return;
				}
				else
				{
					return;
				}
			}
		}
		
		if(remain_count2 == 3)//save to alarm table
		{
			LP_SetAutoManual(istation, DOOR_AUTO, gDoorAMDescript[DOOR_AUTO]);
			
			String soid2 = frmScreen1.dhGrid1.GetCellData(aCurrentRow, COLUMN_SO_ID);			
			SQLalarmInsert(soid2, Now, ALARM_MOLDCHANGE, ALARM_MOLDCHANGE_D);
		}
	}
}

//=======================================================================================
//barcode
int findMoldFromDataMold(String aMold_id, String aMold_cd, String aMold_size)
{
	int rlt = 0;
	try
	{
		string namedb = "readSQL";		
		boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
	
		string sqlquery = "";	
		
		sqlquery += "SELECT ISNULL(MAX(1),0) AS RESULT ";
			  sqlquery += "FROM DATA_MOLD ";
			 sqlquery += "WHERE MOLD_ID = UPPER('" + aMold_id + "') ";//MS003909-JJWO06T06TC') ";/* Scanned Upper case(MOLD BARCODE)(Will be recode DATA_RST table into as MOLD_ID) */
			   sqlquery += "AND MOLD_CD = '" + aMold_cd + "' ";//MS247775-1' ";                 /* Shop Order Mold Code: MOLD_CD */
			   sqlquery += "AND MOLD_MIN_SIZE = '" + aMold_size + "' ";//6T' ";                   /* Shop Order Mold Size Code: MOLD_SIZE_CD */
   	   
		//ShowMessage(sqlquery);   
		
		DBScriptClear(namedb);
		DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		
		int cnt = DBRecordCount(namedb);
		if(cnt > 0)
		{		
			String s1  = DBFieldByName_String(namedb, "RESULT");
			rlt = StrToIntDef(s1, 0);
		}
		
		DBDisconnect(namedb, true); 
	}
	except
	{
		ShowMessage(sqlquery + "\r\n" + ExceptionMessage);
	}
	
	return rlt;
}

//=======================================================================================
//aStartTime=Now
void SQLalarmInsert(String aSoid, TDateTime aStartTime, String aAlarmType, String aAlarmDescript)
{
	try
	{
		string namedb = "SQLAlarm";
		string currdate = FormatDateTime("YYYYMMDD", Now());
		string sStartTime = FormatDateTime("YYYY-MM-DD HH:NN:SS" + ".000", aStartTime);
		boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
		
		//===================================================1/3
		string sqlquery_RES = "SELECT SO_ID, RESOURCE_CD, PRS_QTY ";
				sqlquery_RES += "FROM DATA_SO ";
				sqlquery_RES += "WHERE SO_ID = '" + aSoid + "'";
		DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery_RES);
		DBScriptExecute(namedb);
		string sRES_CD = DBFieldByName_String(namedb, "RESOURCE_CD");
		string sPrsQTY = DBFieldByName_String(namedb, "PRS_QTY");
		//===================================================2/3
		string sqlquery_LASTROW = "SELECT COUNT(ALARM_YMD) AS TOTALROW ";
				sqlquery_LASTROW += "FROM DATA_ALARM";
		DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery_LASTROW);
		DBScriptExecute(namedb);
		int aLASTROW = strToIntDef(DBFieldByName_String(namedb, "TOTALROW"), 0);	
		int cntRow = aLASTROW + 1;
		//===================================================3/3
		string sqlquery = "";	
		
		sqlquery += "INSERT INTO DATA_ALARM ";
		sqlquery += "(ALARM_YMD, FROM_RES_CD, ALARM_SEQ, ALARM_TYPE, TO_RES_CD, BY_SO_ID, DESCRIPTION, START_DT, END_DT, STATUS_CD, ACTION_CD, REMARK) ";
		sqlquery += "VALUES ";
		sqlquery += "('" + currdate + "', '" + sRES_CD + "', '" + intToStr(cntRow) + "', '" + aAlarmType + "', 'MOLD-SHOP', '" + aSoid + "', '" + aAlarmDescript + "', '" + sStartTime + "', NULL, 'N', 'S', NULL)";
   	   
		//ShowMessage(sqlquery);   
		
		DBScriptClear(namedb);
		DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		
		DBDisconnect(namedb, true); 
		
		SetDebug(Format("SOID=%s, Alarm Inserted: %s, %s, %s, %s", [aSoid, currdate, aSoid, sRES_CD, sPrsQTY]));
	}
	except
	{
		ShowMessage(sqlquery + "\r\n" + ExceptionMessage);
	}
}


//=======================================================================================
//station -> start 1
void SQLActDtRead(int station, int soid)
{
	try
	{
		gRstStartDt = "";
		gRstEndDt   = "";
		string namedb = "SQLActDtRead";

		boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
		
		//===================================================1/3
		string sqlquery_ActDt = "SELECT SO_ID, MIN(START_DATE) AS RST_START_DATE, MAX(END_DATE) AS RST_END_DATE, ";
				sqlquery_ActDt += "SUM(CASE REASON WHEN  '00' THEN CNT_QTY ELSE 0 END) AS RST_CNT, ";
				sqlquery_ActDt += "SUM(CASE REASON WHEN  '00' THEN 0 ELSE CNT_QTY END) AS RST_OSND_CNT ";
				sqlquery_ActDt += "FROM DATA_RST ";
				sqlquery_ActDt += "WHERE SO_ID = " + IntToStr(soid) ;
				sqlquery_ActDt += "GROUP BY SO_ID";
		DBScriptClear(namedb);
        DBScriptSQL(namedb, sqlquery_ActDt);
		DBScriptExecute(namedb);
		gRstStartDt = DBFieldByName_String(namedb, "RST_START_DATE");
		gRstEndDt = DBFieldByName_String(namedb, "RST_END_DATE");
		gAct = DBFieldByName_String(namedb, "RST_CNT");
		gOSD = DBFieldByName_String(namedb, "RST_OSND_CNT");
		//===================================================2/3
		DBDisconnect(namedb, true); 
		//===================================================3/3
		
		SetDebug(Format("#%d, SOID=%d, Read Act Count From DB : %s, %s", [station, soid, gAct, gOSD]));
	}
	except
	{
		ShowMessage(sqlquery_ActDt + "\r\n" + ExceptionMessage);
	}
}


//=======================================================================================
{
	gDoorStatusDescript[0] = "Open";
	gDoorStatusDescript[1] = "Close";

	gDoorAMDescript[0] = "Auto";
	gDoorAMDescript[1] = "Manual";

	for(int i=0; i<ALL_ORDERS; i++)
    {
		gWorkingRow[i] = -1;
	}

	gTagNorP[0] = TagLNPln1; 
	gTagNorP[1] = TagRNPln1; 
	gTagNorP[2] = TagLNPln2; 
	gTagNorP[3] = TagRNPln2; 
	gTagNorP[4] = TagLNPln3; 
	gTagNorP[5] = TagRNPln3; 
	gTagNorP[6] = TagLNPln4; 
	gTagNorP[7] = TagRNPln4; 
	gTagNorP[8] = TagLNPln5; //TagLNPln1
	gTagNorP[9] = TagRNPln5; 
	gTagNorP[10] = TagLNPln6; 
	gTagNorP[11] = TagRNPln6; 
	gTagNorP[12] = TagLNPln7; 
	gTagNorP[13] = TagRNPln7; 
	gTagNorP[14] = TagLNPln8; 
	gTagNorP[15] = TagLNPln8; 
	
	gTagNorA[0] = TagLNAct1;
	gTagNorA[1] = TagRNAct1;
	gTagNorA[2] = TagLNAct2;
	gTagNorA[3] = TagRNAct2;
	gTagNorA[4] = TagLNAct3;
	gTagNorA[5] = TagRNAct3;
	gTagNorA[6] = TagLNAct4;
	gTagNorA[7] = TagRNAct4; 
	gTagNorA[8] = TagLNAct5;
	gTagNorA[9] = TagRNAct5;
	gTagNorA[10] = TagLNAct6;
	gTagNorA[11] = TagRNAct6;
	gTagNorA[12] = TagLNAct7;
	gTagNorA[13] = TagRNAct7;
	gTagNorA[14] = TagLNAct8;
	gTagNorA[15] = TagRNAct8;
							 
	gTagOsdA[0] = TagLOAct1;
	gTagOsdA[1] = TagROAct1;
	gTagOsdA[2] = TagLOAct2;
	gTagOsdA[3] = TagROAct2;
	gTagOsdA[4] = TagLOAct3;
	gTagOsdA[5] = TagROAct3;
	gTagOsdA[6] = TagLOAct4;
	gTagOsdA[7] = TagROAct4;
	gTagOsdA[8] = TagLOAct5;
	gTagOsdA[9] = TagROAct5;
	gTagOsdA[10] = TagLOAct6;
	gTagOsdA[11] = TagROAct6;
	gTagOsdA[12] = TagLOAct7;
	gTagOsdA[13] = TagROAct7;
	gTagOsdA[14] = TagLOAct8;
	gTagOsdA[15] = TagROAct8;

	gTagOsdP[0] = TagLOPln1;
	gTagOsdP[1] = TagROPln1;
	gTagOsdP[2] = TagLOPln2;
	gTagOsdP[3] = TagROPln2;
	gTagOsdP[4] = TagLOPln3;
	gTagOsdP[5] = TagROPln3;
	gTagOsdP[6] = TagLOPln4;
	gTagOsdP[7] = TagROPln4;
	gTagOsdP[8] = TagLOPln5;
	gTagOsdP[9] = TagROPln5;
	gTagOsdP[10] = TagLOPln6;
	gTagOsdP[11] = TagROPln6;
	gTagOsdP[12] = TagLOPln7;
	gTagOsdP[13] = TagROPln7;
	gTagOsdP[14] = TagLOPln8;
	gTagOsdP[15] = TagROPln8;

	gTagSOID[0] = TagLSOid1;
    gTagSOID[1] = TagRSOid1;
    gTagSOID[2] = TagLSOid2;
    gTagSOID[3] = TagRSOid2;
    gTagSOID[4] = TagLSOid3;
    gTagSOID[5] = TagRSOid3;
    gTagSOID[6] = TagLSOid4;
    gTagSOID[7] = TagRSOid4;
    gTagSOID[8] = TagLSOid5;
    gTagSOID[9] = TagRSOid5;
    gTagSOID[10] = TagLSOid6;
    gTagSOID[11] = TagRSOid6;
    gTagSOID[12] = TagLSOid7;
    gTagSOID[13] = TagRSOid7;
    gTagSOID[14] = TagLSOid8;
    gTagSOID[15] = TagRSOid8;

/*	gTagMold[0] = TagLSOid1;
    gTagMold[1] = TagRSOid1;
    gTagMold[2] = TagLSOid2;
    gTagMold[3] = TagRSOid2;
    gTagMold[4] = TagLSOid3;
    gTagMold[5] = TagRSOid3;
    gTagMold[6] = TagLSOid4;
    gTagMold[7] = TagRSOid4;
    gTagMold[8] = TagLSOid5;
    gTagMold[9] = TagRSOid5;
    gTagMold[10] = TagLSOid6;
    gTagMold[11] = TagRSOid6;
    gTagMold[12] = TagLSOid7;
    gTagMold[13] = TagRSOid7;
    gTagMold[14] = TagLSOid8;
    gTagMold[15] = TagRSOid8;*/
	
	gTagWrite[0] = WS1;
	gTagWrite[1] = WS2;
	
	gTagTimeWrite[0] = TagLP1Hour;
	gTagTimeWrite[1] = TagLP2Hour;
	
	
	gTagDoor[0] = TagDoor1;
	gTagDoor[1] = TagDoor2;
	gTagDoor[2] = TagDoor3;
	gTagDoor[3] = TagDoor4;
	
	gTagDoor[4] = TagDoor5;
	gTagDoor[5] = TagDoor6;
	gTagDoor[6] = TagDoor7;
	gTagDoor[7] = TagDoor8;
	
/*	
	int iTagNorP[STATION_NUM];
	int iTagOsdP[STATION_NUM];

	iTagNorP[0] = TagLNPln1;
	iTagNorP[1] = TagRNPln1;
	iTagNorP[2] = TagLNPln2;
	iTagNorP[3] = TagRNPln2;
	iTagNorP[4] = TagLNPln3;
	iTagNorP[5] = TagRNPln3;
	iTagNorP[6] = TagLNPln4;
	iTagNorP[7] = TagRNPln4;
	iTagNorP[8] = TagLNPln5;
	iTagNorP[9] = TagRNPln5;
	iTagNorP[10] = TagLNPln6;
	iTagNorP[11] = TagRNPln6;
	iTagNorP[12] = TagLNPln7;
	iTagNorP[13] = TagRNPln7;
	iTagNorP[14] = TagLNPln8;
	iTagNorP[15] = TagRNPln8;

	iTagOsdP[0] = TagLOPln1;
	iTagOsdP[1] = TagROPln1;
	iTagOsdP[2] = TagLOPln2;
	iTagOsdP[3] = TagROPln2;
	iTagOsdP[4] = TagLOPln3;
	iTagOsdP[5] = TagROPln3;
	iTagOsdP[6] = TagLOPln4;
	iTagOsdP[7] = TagROPln4;
	iTagOsdP[8] = TagLOPln5;
	iTagOsdP[9] = TagROPln5;
	iTagOsdP[10] = TagLOPln6;
	iTagOsdP[11] = TagROPln6;
	iTagOsdP[12] = TagLOPln7;
	iTagOsdP[13] = TagROPln7;
	iTagOsdP[14] = TagLOPln8;
	iTagOsdP[15] = TagROPln8;*/
}
//----------------------------------
{
	
}