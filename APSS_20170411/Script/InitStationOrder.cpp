#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp", "RefreshSRC.cpp", "srcSQL.cpp"
//==================================
void InitStationOrder()
{
    TimerGetAct.Enabled = false;
	       
    frmScreen1.btnStop.Pressed = true;
	
	frmScreen1.btnStop.Enabled = false;
	frmScreen1.btnRun.Enabled = false;
    if(frmScreen1_4.cbxStation.ItemIndex == 0)//all
    {
		for(int i=0; i<STATION_NUM; i++)
		{
			gDownloadData[i] = i;
		}
		gDownloadNum = STATION_NUM;		
    }
    else
    {
       int iStation = frmScreen1_4.cbxStation.ItemIndex - 1;
       
	   gDownloadData[0] = iStation;
       gDownloadNum = 1;
    }
	
	frmScreen1_1.dhProgress1.MaxValue = gDownloadNum;
    frmScreen1_1.dhProgress1.UserValue = 0;
	frmScreen1_1.dhLabel1.Caption = Format("%d/%d", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue]);
    frmScreen1_1.Show();
	gDownloadedIdx = 0;
	 
	TimerInitOrder.Enabled = true;
	//srcSQL();
	//TimerRefresh.Enabled = false;
}

//==================================
{
}