#language C++Script

#include "GlobalScripts.pas", "LPComm.cpp", "GlobalJJ.cpp"


//====================================================================================
void DownloadWork()
{      
    int grid_row  = gDownloadData[gDownloadedIdx];

	if(grid_row < 0)
	{
		/*/if(gDownloadedIdx == gDownloadNum)
		{
			frmScreen1_1.Close;
			TimerDownloadWork.Enabled = false;

			TimerGetAct.Enabled = true;
		}
		return;*/
		String ss = "";
		for(int i=0; i<STATION_NUM; i++)
		{
			ss += Format("%d, ", [gDownloadData[i]]);
		}
		//ShowMessage(Format("download work start : %d, %d, %s", [gDownloadedIdx, gDownloadNum, ss]));
		SetDebug(Format("download work start : %d, %d, %s", [gDownloadedIdx, gDownloadNum, ss]));
	}

	String station   	= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_STATION);
	String nor_plncnt   = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORPLNCNT);
	String osd_plncnt   = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDPLNCNT);
	String nor_actcnt   = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORACTCNT);
	String osd_actcnt   = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDACTCNT);
	String moldsize  	= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_MOLDSIZE);
	String moldcolor 	= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_COLOR);
	String so_id 		= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_SO_ID);
	String side 		= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_SHOESSIDE);
	String version_id	= frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_VERSION_ID);
	
	int istation 	= StrToInt(station) - 1;
	int inor_plncnt = StrToInt(nor_plncnt);
	int iosd_plncnt = StrToInt(osd_plncnt);
    int inor_actcnt = StrToInt(nor_actcnt);
	int iosd_actcnt = StrToInt(osd_actcnt);
    string shift 	= "A";//--later calculate from start_date
	Variant vso_id 	= StrToInt(so_id);
	
	int iside = 0;
	if(side == "L") iside = 0;
	else            iside = 1;
    	
	int iTagNum = Int(istation/4);
	int tagAddr = gTagWrite[iTagNum];//4 -> 1 2 3 4, 5 6 7 8, 9 10 11 12, 13 14 15 16
    
	//ShowMessage("Send");
	SetDebug(Format("Download Order : %d, %d, %s, %s, %s, %s", [grid_row, iTagNum, version_id, so_id, moldsize, moldcolor]));
	LP_SendData(tagAddr, istation, iside, moldsize, moldcolor, shift, inor_plncnt, inor_plncnt-inor_actcnt, iosd_plncnt, iosd_plncnt-iosd_actcnt, vso_id);

//ShowMessage(Format("download : %d", [grid_row]));
	//change grid row color -> yellow(progressing work meaning)	
	for(int col=0; col<COLUMN_MAX; col++)
	{
		frmScreen1.dhGrid1.SetCelldataColor2(grid_row, col, frmScreen1.dhGrid1.GetCellData(grid_row, col), false, clYellow, clBlack);
	}
	frmScreen1.dhGrid1.SetCelldataColor2(grid_row, COLUMN_MAX, frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_MAX), true, clYellow, clBlack);
	
    gDownloadedIdx++;
	frmScreen1_1.dhProgress1.UserValue = gDownloadedIdx;
    
    frmScreen1_1.dhLabel1.Caption = Format("%d/%d, Station : %d, Side : %s", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue, istation, side]);
	
	//ShowMessage(Format("download work : %d, %d", [gDownloadedIdx, gDownloadNum]));
	if(gDownloadedIdx == gDownloadNum)
	{
		int iLP = LP_NUM;//MACHINE_NUM*
		int shift2 = DecodeShift2();
		for(i=0; i<iLP; i++)
		{
			LP_ShiftData(gTagWrite[i], shift2);
		}
		
		gLPSOIDZeroNum = UpdateSOID();
		SetUpdateTagTime();
		
		frmScreen1_1.Close;
        TimerDownloadWork.Enabled = false;
		
		TimerGetAct.Enabled = true;
	}
}

//====================================================================================
{
}