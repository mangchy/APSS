#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp", "UpdateVersionID.cpp", "SetMachineTag.cpp"
//==================================
void iniSetting()
{
	TIniFile ini = TIniFile.Create(ProjectPath + "setting.ini");
	string sect  = "ScadaConfig";
	
	String sZone = ini.ReadString(sect, "ZONE", "");
	int startMC  = ini.ReadInteger(sect, "MC_START", 1);
	int endMC    = ini.ReadInteger(sect, "MC_END", 8);
	int sStateMC = ini.ReadInteger(sect, "LAST_STATE_MC", 0);
	gMachinePrefix = ini.ReadString(sect, "MACHINE_PREFIX", "IPI-I");
    
    gQueryWithCompleted = ini.ReadInteger(sect, "SHOW_COMPLETED", 0);
    //int iGridHeaderVisible = ini.ReadInteger(sect, "HEADER_VISIBLE", 0);
	frmScreen1.lbZone.Caption = sZone;
	                                                
//    if(iGridHeaderVisible == 1)
//    {
//        frmScreen1.dhGrid1.Header.Options = frmScreen1.dhGrid1.Header.Options << hoVisible;     
//    }
    
    frmScreen1.cbxStation.Items.Clear();
    frmScreen1.cbxStation.Items.Add("All Stations");
	for(int i=0; i<8; i++)
	{
		frmScreen1.cbxStation.Items.Add(Format("Station %d", [i+1]));
	}
	
	for(i=0; i<DOOR_NUM; i++)
	{
		gDoorStatus[i] = DOOR_OPEN;
	}
	
	//for(int i=0; i<8; i++)
	//{
	//	frmScreen1.cbxStation.Items.Add(Format("Station %d L", [i+1]));
    //    frmScreen1.cbxStation.Items.Add(Format("Station %d R", [i+1]));
	//}
	frmScreen1.cbxStation.ItemIndex = 0;    	
                                  
    frmScreen1.cbxMC.Items.Clear();
    //frmScreen1.cbxMCA.Items.Clear();
    frmScreen1.cbxMC.Items.Add("All MCA");
    frmScreen1_4.cbxMCA.Items.Add("All MCA");
	for(i=startMC; i<=endMC; i++)
	{
		frmScreen1.cbxMC.Items.Add(Format("MCA%.02d", [i]));        
        //frmScreen1.cbxMCA.Items.Add(Format("MCA%.02d", [i]));
        frmScreen1_4.cbxMCA.Items.Add(Format("MCA%.02d", [i]));  
        frmScreen1_4.cbxMCA1.Items.Add(Format("MCA%.02d", [i]));
        frmScreen1_4.cbxMCA2.Items.Add(Format("MCA%.02d", [i]));
        frmScreen1_4.cbxMCA3.Items.Add(Format("MCA%.02d", [i]));
	}
	frmScreen1.cbxMC.ItemIndex = sStateMC;  
    //frmScreen1.cbxMCA.ItemIndex = sStateMC;  
    frmScreen1_4.cbxMCA.ItemIndex = sStateMC;      
    frmScreen1_4.cbxMCA1.ItemIndex = 0;
    frmScreen1_4.cbxMCA2.ItemIndex = 1;
    frmScreen1_4.cbxMCA3.ItemIndex = 2;
	                                         
    frmScreen1_4.cbxStation.Items.Clear();
    frmScreen1_4.cbxStation.Items.Add("All Stations");
	for(i=0; i<8; i++)
	{
		frmScreen1_4.cbxStation.Items.Add(Format("Station %d L", [i+1]));
        frmScreen1_4.cbxStation.Items.Add(Format("Station %d R", [i+1]));
	}
	frmScreen1_4.cbxStation.ItemIndex = 0;    	
    
	frmScreen1_4.cbxMC.Items.Clear();
    frmScreen1_4.cbxMC.Items.Add("All MCA");
	for(i=startMC; i<=endMC; i++)
	{
		frmScreen1_4.cbxMC.Items.Add(Format("MCA%.02d", [i]));
	}
	frmScreen1_4.cbxMC.ItemIndex = sStateMC;
    
	string sectSQL = "ScadaSQLConfig";
	gSvr_type   = ini.ReadString(sectSQL, "SERVER_TYPE", ""); //"SQL SERVER");
	gSvr        = ini.ReadString(sectSQL, "SERVER", ""); //"(local)");
	gPort       = ini.ReadString(sectSQL, "PORT", ""); //"1433");
	gUid        = ini.ReadString(sectSQL, "USR", ""); //"sa");
	gPwd        = ini.ReadString(sectSQL, "PWD", ""); //"system") ;
	gDbname     = ini.ReadString(sectSQL, "DB_NAME", ""); //"IP_SS");
	         
	frmScreen1_4.cbxMCA1.ItemIndex = ini.ReadInteger(sect, "MC1", 0); 
    frmScreen1_4.cbxMCA2.ItemIndex = ini.ReadInteger(sect, "MC2", 1);
    frmScreen1_4.cbxMCA3.ItemIndex = ini.ReadInteger(sect, "MC3", 2);	
    
	ini.Free;	
}


void InitSetup()
{ 
    iniSetting(); 
    
    UpdateVersionID();    
    
    //SetMachineTag();
}

//==================================
{
}