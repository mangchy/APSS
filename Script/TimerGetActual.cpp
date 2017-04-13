#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"



//========================================================================================
void TimerGetActual()
{     
	if(gLPSOIDZeroNum > 0) 
	{
		gLPSOIDZeroNum = UpdateSOID();		
		SetDebug(Format("number of zero SOID : %d", [gLPSOIDZeroNum]));		
	}

	String sMCAs[3];
	sMCAs[0] = frmScreen1_4.cbxMCA1.Text;
	sMCAs[1] = frmScreen1_4.cbxMCA2.Text;
	sMCAs[2] = frmScreen1_4.cbxMCA3.Text;	
	
	//check door
	frmScreen1_4.dhGrid2.UpdateStart(1);
	for(i=0; i<8; i++)//8 = LP1=door 4, LP2=door 4
	{	
		TDateTime dtDoor = GetTagUpdateTime(gTagDoor[i]);
		if(dtDoor != gTagUpdateTimeDoor[i]) 
		{			
			int iDoorStatus = GetTagValueI(gTagDoor[i]);
			String sTime = FormatDateTime("ss", dtDoor);
			frmScreen1_4.dhGrid2.SetCellData(2*i+1, 2, Format("%d, %s", [iDoorStatus, sTime]), false);
			
			if((gDoorStatus[i] == DOOR_OPEN) && (iDoorStatus == DOOR_CLOSE))//close door?
			{	
				gDoorStatus[i] = DOOR_CLOSE;
				SetDebug(Format("door close : %d", [i]));
			}
			else if(iDoorStatus == DOOR_OPEN) 
			{
				gDoorStatus[i] = DOOR_OPEN;
			}
			
			gTagUpdateTimeDoor[i] = dtDoor;
		}		
	}	
	
	int iGridRow = 0;
	for(i=0; i<STATION_NUM; i++)
    {
		
		iGridRow = gWorkingRow[i];
		if(iGridRow == -1) continue;

		TDateTime dt1 = GetTagUpdateTime(gTagNorA[i]);
		if(dt1 == gTagUpdateTime1[i]) continue;
		
		TDateTime dt2 = GetTagUpdateTime(gTagOsdA[i]);
		if(dt2 == gTagUpdateTime2[i]) continue;
		
		TDateTime dt3 = GetTagUpdateTime(gTagOsdP[i]);		
		if(dt3 == gTagUpdateTime3[i]) continue;

		gTagUpdateTime1[i] = dt1;
		gTagUpdateTime2[i] = dt2;
		gTagUpdateTime3[i] = dt3;
		
		TDateTime dt4 = GetTagUpdateTime(gTagSOID[i]);
		if(dt4 != gTagUpdateTimeSOID[i])
		{
			String sTime2 = FormatDateTime("ss", dtDoor);
			int iSOID = GetTagValueI(gTagSOID[i]);	
			frmScreen1_4.dhGrid2.SetCellData(i+1, 1, Format("%d, %s", [iSOID, sTime2]), false);
			
			gTagUpdateTimeSOID[i] = dt4;
		}
		
		try
		{
			int iAct 	= GetTagValueI(gTagNorA[i]);//LP Data, down count
			int iOSD 	= GetTagValueI(gTagOsdA[i]);   
			int iOSDPln = GetTagValueI(gTagOsdP[i]);
			
			int iDBnor_actcnt = gWorkingNormalPln[i] - gWorkingNormal[i];
			
			frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_NORACTCNT, IntToStr(gWorkingNormalPln[i] - iAct), true);	
						
			if(gWorkingOSnDPln[i] < iOSDPln)//update (OS&D Plan) of DATA_SO
			{
				gWorkingOSnDPln[i] = iOSDPln;
				frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_OSNDPLNCNT, IntToStr(iOSDPln), false);	
				frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_OSNDACTCNT, IntToStr(iOSDPln-iOSD), true);	
				UpdateOSnDPlnToDB(gWorkingSOID[i], iOSDPln);
			}
			
			int 	iMachine  = Int(i/STATION_NUM);
			int 	iDoor 	  = Int(i/2);
			String 	sMachineName = sMCAs[iMachine];
			String 	osnd_act  = frmScreen1.dhGrid1.GetCellData(iGridRow, COLUMN_OSNDACTCNT);	
			int 	iosnd_act = StrToIntDef(osnd_act, 0);
			
			if(iAct < iDBnor_actcnt)//normal count
			{				
				int iDBSaveActCount = iDBnor_actcnt - iAct;			
				int prs_qty = iDBSaveActCount * gTagPRSQTY[i];
				prs_qty = Int(prs_qty / gWorkingNormalPln[i]);
				
				SetDebug(Format("SOID=%d, Normal Act Count Row : %d, %d, %d, %d, %d, prs_qty=%d, door=%d", [gWorkingSOID[i], iGridRow, iDBnor_actcnt, iAct, gTagPRSQTY[i], gWorkingNormalPln[i], prs_qty, iDoor]));
				
				gWorkingNormal[i] = gWorkingNormalPln[i] - iAct;
	
				TDateTime dtEnd = Now();
				//SQLActDtRead(gWorkingSOID[i]);
				
				//if(checkFinish(iGridRow, i, iCalcAct, iCalcOSD) == 1)
				//{						
				//	SetDebug(Format("insert db : %d, %d(%d-%d)", [gWorkingSOID[i], iCalcAct]), clRed);
				//	return;						
				//}	
				//else
				//{	
					SetDebug(Format("SOID=%d, insert db : %d(%d-%d)", [gWorkingSOID[i], iDBSaveActCount, iDBnor_actcnt, iAct]), clRed);
					
					if(iDBSaveActCount < 0)
					{
						SetDebug(Format("SOID=%d, error count : %d(%d-%d)", [gWorkingSOID[i], iDBSaveActCount, iDBnor_actcnt, iAct]), clRed);
						checkFinish(iGridRow, i, 0, 0);
						return;
					}
					InsertWorkCountToDB(gWorkingSOID[i], prs_qty, iDBSaveActCount, gZone, REASON_NORMAL_COUNT, sMachineName, gTagUpdateTimeDoor[iDoor], dtEnd);
					
					SQLActDtRead(gWorkingSOID[i]);
					String start_time = Copy(gRstStartDt, 9, Length(gRstStartDt));
					String end_time   = Copy(gRstEndDt, 9, Length(gRstEndDt));
					frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_ACTDATESTART, start_time, false);
					frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_ACTDATEEND, end_time, false);
					frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_NORACTCNT, gAct, false);
					
					int iCalcAct   = gWorkingNormalPln[i] - StrToIntDef(gAct, 0);
					int iCalcOSD   = gWorkingOSnD[i] - StrToIntDef(gOSD, 0);
					
					
					if(checkFinish(iGridRow, i, iCalcAct, iCalcOSD) == 1) 
					{
						SetDebug(Format("SOID=%d, finish order : Nor=%d(%s), OSnD=%d(%s)", [gWorkingSOID[i], gWorkingNormalPln[i], gAct, gWorkingOSnD[i], gOSD]), clRed);
						return;
					}
				//}
			}
			else if((gWorkingOSnDPln[i] != iosnd_act) && (iOSD == 0))//os&d count
			{
				gWorkingOSnD[i] = gWorkingOSnDPln[i];// - iOSD;
				frmScreen1.dhGrid1.SetCellData(iGridRow, COLUMN_OSNDACTCNT, IntToStr(gWorkingOSnD[i]), false);	

				int prs_qty2 = gWorkingOSnD[i];
				prs_qty2 = prs_qty2 * gTagPRSQTY[i];
				prs_qty2 = Int(prs_qty2 / gWorkingNormalPln[i]);
							
				SetDebug(Format("SOID=%d, OS&D Act Count Row : %d, %d, prs_qty2=%d, door=%d", [gWorkingSOID[i], iGridRow, gWorkingOSnDPln[i], prs_qty2, iDoor]));
				
				TDateTime dtEnd2 = Now();
				InsertWorkCountToDB(gWorkingSOID[i], prs_qty2, gWorkingOSnD[i], gZone, REASON_OSND_COUNT, sMachineName, gTagUpdateTimeDoor[iDoor], dtEnd2);
				
				if(checkFinish(iGridRow, i, iAct, iOSD) == 1) return;
			}	
			
		}
		except
		{
			SetDebug(Format("SOID=%d, GetActual error : %s", [gWorkingSOID[i], ExceptionMessage]), clRed);
		}
	}
	
	frmScreen1_4.dhGrid2.UpdateStart(0);
	frmScreen1_4.dhGrid2.Repaint();	
	frmScreen1.dhGrid1.Repaint();
	
	frmScreen1_4.lbGetActualTime.Caption = FormatDateTime("HH:NN:SS", Now());
}

//==================================
{
}