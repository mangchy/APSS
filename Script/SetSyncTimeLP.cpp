#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"
//==================================
void SetSyncTimeLP()
{
	if(frmScreen1_4.cbxLP.ItemIndex == 0)//all
    {
		for(int i=0; i<LP_NUM; i++)
		{
			gDownloadData[i] = i;
		}
		gDownloadNum = LP_NUM;		
    }
    else
    {
       int iLP = frmScreen1_4.cbxLP.ItemIndex - 1;
	   //iStation = iStation;//Int(iStation/2);
       gDownloadNum = 1;
	   
	   gDownloadData[0] = iLP;
    }
	
	frmScreen1_1.dhProgress1.MaxValue = gDownloadNum;
    frmScreen1_1.dhProgress1.UserValue = 0;
	frmScreen1_1.dhLabel1.Caption = Format("%d/%d", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue]);
    frmScreen1_1.Show();
	gDownloadedIdx = 0;
	
	TimerSyncTimeLP.Enabled = true;
}

//==================================
{
}