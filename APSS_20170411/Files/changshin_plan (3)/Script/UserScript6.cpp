#language C++Script

#include "GlobalScripts.pas", "Global.pas"




//==================================
void UserScript6()
{
  Screen1.dhProgress1.MaxValue = 100;
  Screen1.dhProgress1.UserValue = 0;
                            
  gOrderCount = 100;   
  gDownloadedCount = 0;      
  
  gLP_UnitAddr = 1;
  gLP_Size = 1; 
  gLP_OrderQty = 2; 
  gLP_Part = 2; 
  gLP_PartSeries = 1; 
  gLP_Layer = 3; 
  gLP_CDie = 1; 
  gLP_TargetCnt = 5; 
  gLP_Lamp = 0xff;    
      
    gLP_AssyDateMM = 3;  
    gLP_AssyDateDD = 21;   
    //gLP_DisplayPage = 19; 
    //gLP_WorkHour = StrToInt(Screen1.dhEdit1.Text); 
    gLP_ML = 14;
    gLP_PlanQty = 20;   
    gLP_Style = 123456789;   
  
  gDownloadStep = SET_ASSYDATE_ORDER;
  TimerScreenDownload.Enabled = true;
}

//==================================
{
}