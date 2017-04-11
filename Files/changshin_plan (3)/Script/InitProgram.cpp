#language C++Script

#include "GlobalScripts.pas", "Global.pas"
//==================================
void InitProgram()
{    	
	int i, j;
	
	for(i=1; i<LPUNIT_MAX; i++)//À¯´Ö¼ö, 20
    {
      for(j=0; j<WORK_HOUR_MAX; j++)//1~8H
        gJJ_OrderList[i][j] = TStringList.Create;
    }      
    

	TStringList secs = TStringList.Create;
	TStringList secv = TStringList.Create;
	
	//_TRACE(ICO_ERROR, "Init Program 1");
	//if(gJJ_Ini == nil) gJJ_Ini = TIniFile.Create(gOrderListIni);
	
	gVBProgramPath = gJJ_Ini.ReadString("Config", "Path_Exe", Projectpath + "app\\Gathering data_CuttingPlan_JJ");
	gVBProgramName = gJJ_Ini.ReadString("Config", "Path_Name", "Cutting System  V4.2_20160323");
	
	int iUse;
	for(i=0; i<LPUNIT_MAX; i++)
	{
		iUse = gJJ_Ini.ReadInteger("Config", "Unit" + IntToStr(i), 0);		
		if(iUse == 0) gUnitConnected[i] = false;
        else gUnitConnected[i] = true; 
		
		gLP_FinishWork_Hour[i] = 0;
		gLP_FinishWork_Index[i] = 0;
	}
	
	//_TRACE(ICO_ERROR, "Init Program 2");
	
	//_TRACE(ICO_ERROR, "START 1");
    gJJ_Ini.ReadSections(secs);
	//_TRACE(ICO_ERROR, Format("START 2, %d, %s", [secs.Count, gVBProgramPath]));
	
	//for(i=0; i<LPUNIT_MAX; i++) 
	//	for(j=0; j<WORK_HOUR_MAX; j++)
	//		gJJ_OrderTotal[i][j] = 0;
			
	for(i=0; i<secs.Count; i++)
	{
		gjj_Ini.ReadSectionValues(secs[i], secv);
		
		//_TRACE(ICO_ERROR, Format("secs v : %s, %d", [secs[i], secv.Count]));
		
		if(secs[i] == "Config") continue;
		
		_TRACE(ICO_ERROR, Format("secs v : %s, %d, %s", [secs[i], secv.Count, secv.Values["Address"]]));
		int addr, hour;
		try
		{
			addr = StrToInt(secv.Values["Address"]);
		}
		except
		{
			_TRACE(ICO_ERROR, Format("error addr : %d, %s", [i, secv.Values["Address"]]));
			continue;
		}
		
		try
		{
			hour = StrToInt(secv.Values["Hour"]);
		}
		except
		{
			_TRACE(ICO_ERROR, Format("error hour : %d, %s", [i, secv.Values["Hour"]]));
			continue;
		}
		int count = secv.Count - 5;
		
		_TRACE(ICO_ERROR, Format("---------secs v 2 : %s, %d, %d, %d", [secs[i], addr, hour, count]));
		for(j=0; j<count; j++)
		{
			gJJ_OrderList[addr][hour].Add(secv.Values[IntToStr(j+1)]);
			//_TRACE(ICO_ERROR, Format("secs v : %d, %s", [j, secv.Values[IntToStr(j+1)]]));
		}
		
		_TRACE(ICO_ERROR, Format("ini read : %s, %d, %d, %d", [secs[i], addr, hour, gJJ_OrderList[addr][hour].Count]));
	}
	//_TRACE(ICO_ERROR, "START 3");
	//if(gJJ_Ini != nil) gJJ_Ini.Free;
	//gJJ_Ini = nil; 
	secs.Free;
	secv.Free;
	
	//Screen1.dhPanelFill2.Visible = false;

    TimerGetWorkCount.Enabled = true;
}

//==================================
{
}