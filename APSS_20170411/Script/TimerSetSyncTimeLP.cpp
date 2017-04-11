#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp", "LPComm.cpp"
//==================================
void TimerSetSyncTimeLP()
{
    int iLP = gDownloadData[gDownloadedIdx];
    
    int hour = StrToIntDef(frmScreen1_4.edtHour.Text, 1);
    int min  = StrToIntDef(frmScreen1_4.edtMin.Text, 0);
    
    int tagAddr = gTagTimeWrite[iLP];
    
	LP_SetTime(tagAddr, hour, min);
    
    gDownloadedIdx++;
    frmScreen1_1.dhProgress1.UserValue = gDownloadedIdx;
    frmScreen1_1.dhLabel1.Caption = Format("%d/%d, LP : %d", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue, iLP]);
    
    if(gDownloadedIdx == gDownloadNum)
    {
        frmScreen1_1.Close;
        TimerSyncTimeLP.Enabled = false;
		
		//TimerGetAct.Enabled = frmScreen1.btnRun.Pressed;
	}
}

//==================================
{
}