#language C++Script

#include "GlobalScripts.pas", "Global.pas"
//==================================
void SetConfig()
{
    gVBProgramPath = Screen1_1.dhEdit1.Text;
    
    gUnitConnected[0] = Screen1_1.dhCheckBox1.Checked;
    gUnitConnected[1] = Screen1_1.dhCheckBox2.Checked;
    gUnitConnected[2] = Screen1_1.dhCheckBox3.Checked;
    gUnitConnected[3] = Screen1_1.dhCheckBox4.Checked;
    gUnitConnected[4] = Screen1_1.dhCheckBox5.Checked;
    gUnitConnected[5] = Screen1_1.dhCheckBox6.Checked;
	gUnitConnected[6] = Screen1_1.dhCheckBox7.Checked;
	gUnitConnected[7] = Screen1_1.dhCheckBox8.Checked;
    
    
    //gJJ_Ini = TIniFile.Create(ProjectPath + "OrderList.ini");
    
    for(int i=0; i<8; i++)
    {
      if(gUnitConnected[i] == false)
        gJJ_Ini.WriteInteger("Config", "Unit" + IntToStr(i), 0);
      else                                                     
        gJJ_Ini.WriteInteger("Config", "Unit" + IntToStr(i), 1);
    }
    
    gJJ_Ini.WriteString("Config", "Path_Exe", gVBProgramPath);
    
    //gJJ_Ini.Free;	
}

//==================================
{
}