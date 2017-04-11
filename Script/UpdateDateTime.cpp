#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp", "UpdateVersionID.cpp", "srcSQL.cpp", "InitStationOrder.cpp", "SetAllDataItems.cpp"
//==================================
void UpdateDateTime()
{
   //frmScreen1.txtDate.Text = FormatDateTime("YYYYMMDD", Now());
   TDateTime dt = Now();            
   String sD = FormatDateTime("DD", dt);
   String sH = FormatDateTime("HH", dt);
   String sM = FormatDateTime("NN", dt);
   String sS = FormatDateTime("SS", dt);   
   
   frmScreen1.lbTime.Caption = Format("Time : %s %s:%s:%s", [sD, sH, sM, sS]);// + FormatDateTime("DD HH:NN:SS", dt);
   
   int iH, iM, iS;
   iH = StrToInt(sH);
   iM = StrToInt(sM);
   iS = StrToInt(sS);
               
   if(grefreshDatabase == 0)
   {
        if((iH == 14) && (iM == 27) && (iS == 10))
        {      
            TimerGetAct.Enabled = false;
			
            SetDebug("Refresh VersionID");
            UpdateVersionID();    
            srcSQL();				
            grefreshDatabase = 1;
        }                        
        else
        {
            grefreshDatabase = 0;
        }
   }
   else if((iH != 14))
   {                                                         
        grefreshDatabase = 0;
   }
   
   if(frmScreen1_4.cbxNowTime.Checked)
   {                        
        frmScreen1_4.edtHour.Text = sH;//FormatDateTime("HH", Now());
        frmScreen1_4.edtMin.Text  = sM;//FormatDateTime("NN", Now());
   }
}

//==================================
{
}