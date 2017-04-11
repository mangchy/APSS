#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"
//==================================
void SetMachineTag()
{
	/*int i;
	
	int iMCA1 = frmScreen1_4.cbxMCA1.ItemIndex;
	int iMCA2 = frmScreen1_4.cbxMCA2.ItemIndex;
	int iMCA3 = frmScreen1_4.cbxMCA3.ItemIndex;
	
	int offset1 = STATION_NUM*iMCA1;
	int offset2 = STATION_NUM*iMCA2;
	int offset3 = STATION_NUM*iMCA3;
	
	int offsetLP1 = LP_NUM*iMCA1;
	int offsetLP2 = LP_NUM*iMCA2;
	int offsetLP3 = LP_NUM*iMCA3;
	
	//int STATION_NUM
	
	for(i=0; i<STATION_NUM; i++)
	{
		//machine 1
		gTagNorP[i] = gTagNorPtmp[i + offset1];
		gTagNorA[i] = gTagNorAtmp[i + offset1];
		gTagOsdA[i] = gTagOsdAtmp[i + offset1];
		gTagOsdP[i] = gTagOsdPtmp[i + offset1];
		gTagSOID[i] = gTagSOIDtmp[i + offset1];
		
		//machine 2
		gTagNorP[i + STATION_NUM] = gTagNorPtmp[i + offset2];
		gTagNorA[i + STATION_NUM] = gTagNorAtmp[i + offset2];
		gTagOsdA[i + STATION_NUM] = gTagOsdAtmp[i + offset2];
		gTagOsdP[i + STATION_NUM] = gTagOsdPtmp[i + offset2];
		gTagSOID[i + STATION_NUM] = gTagSOIDtmp[i + offset2];
		
		//machine 3
		gTagNorP[i + STATION_NUM*2] = gTagNorPtmp[i + offset3];
		gTagNorA[i + STATION_NUM*2] = gTagNorAtmp[i + offset3];
		gTagOsdA[i + STATION_NUM*2] = gTagOsdAtmp[i + offset3];
		gTagOsdP[i + STATION_NUM*2] = gTagOsdPtmp[i + offset3];
		gTagSOID[i + STATION_NUM*2] = gTagSOIDtmp[i + offset3];
	}
	
	for(i=0; i<LP_NUM; i++)
	{   
		gTagWrite[i] = gTagWritetmp[i + offsetLP1];
		gTagTimeWrite[i] = gTagTimeWritetmp[i + offsetLP1];
		
		gTagWrite[i + LP_NUM] = gTagWritetmp[i + offsetLP2];
		gTagTimeWrite[i + LP_NUM] = gTagTimeWritetmp[i + offsetLP2];
		
		gTagWrite[i + LP_NUM*2] = gTagWritetmp[i + offsetLP3];
		gTagTimeWrite[i + LP_NUM*2] = gTagTimeWritetmp[i + offsetLP3];
	}*/		
	
	TIniFile ini = TIniFile.Create(ProjectPath + "setting.ini");
	string sect = "ScadaConfig";
	
	ini.WriteInteger(sect, "MC1", frmScreen1_4.cbxMCA1.ItemIndex); 
	ini.WriteInteger(sect, "MC2", frmScreen1_4.cbxMCA2.ItemIndex); 
	ini.WriteInteger(sect, "MC3", frmScreen1_4.cbxMCA3.ItemIndex); 
    
    ini.Free;

}

//==================================
{
}