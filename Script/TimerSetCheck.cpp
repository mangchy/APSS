#language C++Script
#include "GlobalScripts.pas", "GlobalJJ.cpp"


//=======================================================================================
void SaveOrder(int aOrderPos, int arow, int asoid)
{
	String nor_plncnt = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_NORPLNCNT);	
	String nor_actcnt = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_NORACTCNT);	
	String osd_plncnt = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_OSNDPLNCNT);	
	String osd_actcnt = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_OSNDACTCNT);	
	String station 	  = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_STATION);	
	String side 	  = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_SHOESSIDE);	
	String prs_qty 	  = frmScreen1.dhGrid1.GetCellData(arow, COLUMN_PRS_QTY);	
	
	int iprs_qty = StrToIntDef(prs_qty, 0);
	int inorplncnt = StrToIntDef(nor_plncnt, 0);
	int inoractcnt = StrToIntDef(nor_actcnt, 0);
	int iosdplncnt = StrToIntDef(osd_plncnt, 0);
	int iosdactcnt = StrToIntDef(osd_actcnt, 0);
	int istation   = StrToIntDef(station, -1);
	
	gWorkingNormalPln[aOrderPos]	= inorplncnt;
	gWorkingNormal[aOrderPos] 		= inoractcnt;
	gWorkingOSnDPln[aOrderPos]   	= iosdplncnt;
	gWorkingOSnD[aOrderPos]   		= iosdactcnt;
	gWorkingRow[aOrderPos] 			= arow;
	gWorkingSOID[aOrderPos]			= asoid;
	gTagPRSQTY[aOrderPos]			= iprs_qty;
}

//=======================================================================================
//LP -> SCADA
void TimerSetCheck()
{	
	TimerCheck.Enabled = false;  
	
	//ShowMessage(Format("timer set check : %d", [ALL_ORDERS]));
	for(int io=0; io<ALL_ORDERS; io++)
	{
		gDownloadData[io] = -1;
		gWorkingRow[io] = -1;
	}
	
	frmScreen1_2.prgCheck.UserValue += 100;         
    gDownloadNum   = 0;
	gDownloadedIdx = 0;
    
	String sMCAs[3];
	sMCAs[0] = frmScreen1_4.cbxMCA1.Text;
	sMCAs[1] = frmScreen1_4.cbxMCA2.Text;
	sMCAs[2] = frmScreen1_4.cbxMCA3.Text;	
	
	try
	{
		frmScreen1.dhGrid1.UpdateStart(1);	
		
		SetDebug(Format("check next order : station=%d, sortkey=%d", [gCurrentStation, gCurrentSortKey]), clRed);
		if((gCurrentStation == -1) && (gCurrentSortKey == -1))
		{
			SetDebug(Format("check next order 2 : station=%d, sortkey=%d", [gCurrentStation, gCurrentSortKey]), clRed);
			for(int i=0; i<STATION_NUM; i++)
			{	
				String sMachineName = sMCAs[Int(i/STATION_NUM)];
				int iSOID = GetTagValueI(gTagSOID[i]);	
				int iRow  = getGridRowFromSOID(sMachineName, i, iSOID);
				//ShowMessage(Format("%d, %d", [iSOID, iRow]));
				if(iRow > -1) //find LP's SOID from Database(grid data)
				{
					TDateTime dt1 = GetTagUpdateTime(gTagNorA[i]);
					TDateTime dt2 = GetTagUpdateTime(gTagOsdA[i]);
					
					if(dt1 == gTagUpdateTime1[i]) 
					{
						SetDebug(Format("Checking order 1! %d, %d", [iRow, iSOID]));
						TimerCheck.Enabled = true;	
						frmScreen1.dhGrid1.UpdateStart(0);
						//setColorRow(iRow, COLOR_WORK_PROGRESS);
						return;
					}
					if(dt2 == gTagUpdateTime2[i]) 
					{
						SetDebug(Format("Checking order 2! %d, %d", [iRow, iSOID]));
						TimerCheck.Enabled = true;	
						frmScreen1.dhGrid1.UpdateStart(0);
						//setColorRow(iRow, COLOR_WORK_PROGRESS);
						return;
					}
				
					SaveOrder(i, iRow, iSOID);
					
					int iAct  = GetTagValueI(gTagNorA[i]);
					int iOSD  = GetTagValueI(gTagOsdA[i]);   

					//ShowMessage(Format("%d, %d, %d", [iRow, iAct, iOSD]));
					if((iAct == 0) && (iOSD == 0))//finish order - change row color -> lime
					{
						SetColorRow(iRow, COLOR_WORK_FINISH);

						int iNextRow = getNextOrderRow(sMachineName, iRow);				
						if(iNextRow > -1)
						{
							String soid2 = frmScreen1.dhGrid1.GetCellData(iNextRow, COLUMN_SO_ID);	
							int isoid3	= StrToIntDef(soid2, 0);
							SaveOrder(i, iNextRow, isoid3);
							
							SetColorRow(iNextRow, COLOR_WORK_NEXT);

							gDownloadData[gDownloadNum] = iNextRow;
							gDownloadNum++;
						}
					}
					else //progressing order -> clYellow
					{				
						setColorRow(iRow, COLOR_WORK_PROGRESS);
					}
				}
				else//NOT FOUND LP's SOID in grid, example Init LP status or SOID address's data = 0
				{			
					SetDebug(Format("SO_ID not found : %d, %d", [i,iSOID]));
					
					iRow  = getGridRow(sMachineName, i);
					if(iRow > -1)
					{
						String soid = frmScreen1.dhGrid1.GetCellData(iRow, COLUMN_SO_ID);	
						int isoid42	= StrToIntDef(soid, 0);
						SaveOrder(i, iRow, isoid42);
						
						SetColorRow(iRow, COLOR_WORK_NEXT);

						gDownloadData[gDownloadNum] = iRow;
						gDownloadNum++;
					}
				}
			}       
		}
		else
		{
			SetDebug(Format("check next order 3 : station=%d, sortkey=%d", [gCurrentStation, gCurrentSortKey]), clRed);
			
			sMachineName = sMCAs[Int(gCurrentStation/STATION_NUM)];
			int iSOID2 = GetTagValueI(gTagSOID[gCurrentStation]);	
			int iRow2  = getGridRowFromSOID(sMachineName, gCurrentStation, iSOID2);
			//ShowMessage(Format("%d, %d", [iSOID2, iRow2]));
			if(iRow2 > -1) //find LP's SOID from Database(grid data)
			{
				TDateTime dt12 = GetTagUpdateTime(gTagNorA[gCurrentStation]);
				TDateTime dt22 = GetTagUpdateTime(gTagOsdA[gCurrentStation]);
				
				if(dt12 == gTagUpdateTime1[gCurrentStation]) 
				{
					SetDebug(Format("Checking order 1! %d, %d", [iRow2, iSOID2]));
					TimerCheck.Enabled = true;	
					frmScreen1.dhGrid1.UpdateStart(0);
					//setColorRow(iRow2, COLOR_WORK_PROGRESS);
					return;
				}
				if(dt22 == gTagUpdateTime2[gCurrentStation]) 
				{
					SetDebug(Format("Checking order 2! %d, %d", [iRow2, iSOID2]));
					TimerCheck.Enabled = true;	
					frmScreen1.dhGrid1.UpdateStart(0);
					//setColorRow(iRow2, COLOR_WORK_PROGRESS);
					return;
				}
			
				SaveOrder(gCurrentStation, iRow2, iSOID2);
				
				int iAct2  = GetTagValueI(gTagNorA[gCurrentStation]);
				int iOSD2  = GetTagValueI(gTagOsdA[gCurrentStation]);   

				//ShowMessage(Format("%d, %d, %d", [iRow2, iAct2, iOSD2]));
				if((iAct2 == 0) && (iOSD2 == 0))//finish order - change row color -> lime
				{
					SetColorRow(iRow2, COLOR_WORK_FINISH);

					int iNextRow2 = getNextOrderRow(sMachineName, iRow2);				
					if(iNextRow2 > -1)
					{
						String soid22 = frmScreen1.dhGrid1.GetCellData(iNextRow2, COLUMN_SO_ID);	
						int iSOID23	= StrToIntDef(soid22, 0);
						SaveOrder(i, iNextRow2, iSOID23);
						
						SetColorRow(iNextRow2, COLOR_WORK_NEXT);

						gDownloadData[gDownloadNum] = iNextRow2;
						gDownloadNum++;
					}
				}
				else //progressing order -> clYellow
				{				
					setColorRow(iRow2, COLOR_WORK_PROGRESS);
				}
			}
			else//NOT FOUND LP's SOID in grid, example Init LP status or SOID address's data = 0
			{			
				SetDebug(Format("SO_ID not found : %d, %d", [gCurrentStation,iSOID2]));
				
				iRow2  = getGridRow(sMachineName, gCurrentStation);
				if(iRow2 > -1)
				{
					String soid25 = frmScreen1.dhGrid1.GetCellData(iRow2, COLUMN_SO_ID);	
					int iSOID25	= StrToIntDef(soid25, 0);
					SaveOrder(gCurrentStation, iRow2, iSOID25);
					
					SetColorRow(iRow2, COLOR_WORK_NEXT);

					gDownloadData[gDownloadNum] = iRow2;
					gDownloadNum++;
				}
			}
		}

		frmScreen1.dhGrid1.UpdateStart(0);
	}
	except
	{
		SetDebug(ExceptionMessage, clRed);
	}
	
	frmScreen1_2.Close();  	

	frmScreen1.dhPanelFill12.Color = clYellow;
	frmScreen1.btnRun.Enabled  = true;
	frmScreen1.btnStop.Enabled = true;
			
	int iSOIDZeroNum = UpdateSOID();
	SetDebug(Format("number of download order : %d", [gDownloadNum, iSOIDZeroNum]));
	
	SetFilterGrid();
	
	if(frmScreen1.btnRun.Pressed)
	{
		if(gDownloadNum > 0)
		{
			frmScreen1_1.dhProgress1.MaxValue = gDownloadNum;
			frmScreen1_1.Show();
			gDownloadedIdx = 0;

			TimerDownloadWork.Enabled = true;
		}
		else
		{
			
			SetUpdateTagTime();
			TimerGetAct.Enabled = true;
		}
	}	
}  

{

}