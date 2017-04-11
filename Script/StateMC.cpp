#language C++Script

#include "GlobalScripts.pas"
//==================================
void StateMC()
{
	TIniFile ini = TIniFile.Create(ProjectPath + "setting.ini");
	string sect = "ScadaConfig";
	
	ini.WriteInteger(sect, "LAST_STATE_MC", frmScreen1.cbxMC.ItemIndex ); 
    
    ini.Free;
}

//==================================
{
}