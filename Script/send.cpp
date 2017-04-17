#language C++Script
#include "GlobalScripts.pas", "LPComm.cpp", "GlobalJJ.cpp"

//download start
void send()
{
	String ss;
    for(i=0; i<STATION_NUM; i++)
    {
		gFinishOrders[i] = NOT_FINISH_WORK;
		ss += Format("%d, ", [gWorkingRow[i]]);
    }         
    SetDebug("Working row : " + ss);
    
    
    if(gDownloadNum == 0) 
    {
        TimerGetAct.Enabled = true;
        SetDebug("not find download order", clRed);
        return;
    }
	
	gDownloadedIdx = 0;
	                  
    frmScreen1_1.dhProgress1.UserValue = 0;
    frmScreen1_1.dhProgress1.MaxValue  = gDownloadNum;
    frmScreen1_1.Show;   
    
    frmScreen1_1.dhLabel1.Caption = Format("%d/%d", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue]);
                         
    frmScreen1_4.btnInitialize.Enabled = frmScreen1.btnStop.Pressed;
    
	TimerDownloadWork.Enabled = true;
}  

{

}