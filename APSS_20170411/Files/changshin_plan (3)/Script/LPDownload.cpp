#language C++Script

#include "GlobalScripts.pas", "Global.pas", "InitCallBackFromVB.cpp"

Extended sty;
int sty1_1, sty1_2, sty1_3;

//==================================
void LPDownload()
{
    Timer3.Enabled = false;
    
    if(TEST_LP_ADDR_FIX == 1) gLP_UnitAddr = 1;//일단 테스트
    
	//String section = "Unit_" + IntToStr(gLP_UnitAddr) + IntToStr(gLP_WorkHour);
			
    _TRACE(ICO_ERROR, Format("LPDownload : %d, %d/%d, %d", [gDownloadStep, gDownloadedCount, gOrderCount, gLP_WorkHour]));
    switch(gDownloadStep)
    {
        case SET_ASSYDATE_ORDER: 
        {
			/*
			gJJ_OrderList[gLP_UnitAddr][gLP_WorkHour].Clear;
			
			gJJ_Ini = TIniFile.Create(ProjectPath + "OrderList.ini");
			gJJ_Ini.EraseSection(section);
			gJJ_Ini.WriteString(section, "UpdateTime", DateTimeToStr(Now));
			gJJ_Ini.WriteInteger(section, "Address", gLP_UnitAddr);
			gJJ_Ini.WriteInteger(section, "Hour", gLP_WorkHour);
			*/
			
//            gDownloadedCount = 1;
//            hour_order = 100;3;
            //int unit_addr, int mm, int dd, int ordercnt, int hour, int ml, int planqty)  
            
            _TRACE(ICO_ERROR, Format("down start : %d, %d, %d, %d, %d, %d, %d", 
			   [gLP_UnitAddr, 
				gLP_AssyDateMM, 
				gLP_AssyDateDD, 
				gOrderCount,//gLP_DisplayPage, 
				gLP_WorkHour, 
				gLP_ML, 
				gLP_PlanQty]));
            
            Download_InfoOrder(
				gLP_UnitAddr, 
				gLP_AssyDateMM, 
				gLP_AssyDateDD, 
				gOrderCount,//gLP_DisplayPage, 
				gLP_WorkHour, 
				gLP_ML, 
				gLP_PlanQty);
        }
		case SET_EACHROWDATA_CLEAN:
		{                
            //818416600 => 819 416 600
			//sty = gLP_Style / 1000000;
			//sty1_1 = Trunc(sty);
			//sty = gLP_Style % 1000000;
			//sty = (sty) / 1000;
			//sty1_2 = Trunc(sty);
			//sty = gLP_Style / 1000;
			//sty1_3 = gLP_Style % 1000;
            
			_TRACE(ICO_ERROR, Format("clean down ing : %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			   [gLP_UnitAddr, 
				gDownloadedCount, 
				gLP_Size, 
				gLP_OrderQty, 
				sty1_1, 
				sty1_2, 
				sty1_3, 
				gLP_Part, 
				gLP_PartSeries, 
				gLP_Layer, 
				gLP_CDie, 
				gLP_TargetCnt, 
				gLP_Lamp]));
            
			Download_EachOrderData(
				gLP_UnitAddr, 
				gDownloadedCount, 
				0,//gLP_Size, 
				0,//gLP_OrderQty, 
				0,//sty1_1, 
				0,//sty1_2, 
				0,//sty1_3, 
				0,//gLP_Part, 
				0,//gLP_PartSeries, 
				0,//gLP_Layer, 
				0,//gLP_CDie, 
				0,//gLP_TargetCnt, 
				0,//gLP_Lamp,
				0,
				1);//gLP_Remain);
            Screen1.dhProgress1.UserValue = gDownloadedCount;
        }       
		
        case SET_EACHROWDATA:    
        {   
			//818416600 => 819 416 600
			sty = gLP_Style / 1000000;
			sty1_1 = Trunc(sty);
			sty = gLP_Style % 1000000;
			sty = (sty) / 1000;
			sty1_2 = Trunc(sty);
			sty = gLP_Style / 1000;
			sty1_3 = gLP_Style % 1000;
            
			_TRACE(ICO_ERROR, Format("order downloading : %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			   [gLP_UnitAddr, 
				1,//gJJ_OrderList[gLP_UnitAddr][gLP_WorkHour].Count,
				gDownloadedCount, 
				gLP_Size, 
				gLP_OrderQty, 
				sty1_1, 
				sty1_2, 
				sty1_3, 
				gLP_Part, 
				gLP_EAperPair,//gLP_PartSeries, 
				gLP_Layer, 
				gLP_CDie, 
				gLP_TargetCnt, 
				gLP_Lamp,
				gLP_Remain]));
            
			Download_EachOrderData(
				gLP_UnitAddr, 
				gDownloadedCount, 
				gLP_Size, 
				gLP_OrderQty, 
				sty1_1, 
				sty1_2, 
				sty1_3, 
				gLP_Part, 
				gLP_EAperPair,//gLP_PartSeries, 
				gLP_Layer, 
				gLP_CDie, 
				gLP_TargetCnt, 
				gLP_Lamp,
				gLP_TargetCnt - gLP_Remain);
            Screen1.dhProgress1.UserValue = gDownloadedCount;
        }             
        case SET_TOTALROWCOMPLETED:  
        {                    
			_TRACE(ICO_ERROR, Format("down ok : %d, %d", [gLP_UnitAddr, gLP_WorkHour]));
          
			//if(gJJ_Ini == nil) gJJ_Ini = TIniFile.Create(gOrderListIni);
			//String section = "Unit_" + IntToStr(gLP_UnitAddr) + IntToStr(gLP_WorkHour);
			//gJJ_Ini.WriteInteger(section, "Downloaded", 1);		  
			//if(gJJ_Ini != nil) gJJ_Ini.Free;
			//gJJ_Ini = nil;
			
            Download_Completed(gLP_UnitAddr, gLP_WorkHour);
        }
		case SET_CURRENTTIME:
		{
			SetCurrentTime(gLP_UnitAddr, 1);
		}
    }        
}

//==================================
{
}