#language C++Script

#include "GlobalScripts.pas", "Global.pas"

String sLastOrder 	= "";
int    wait_rxtime 	= 1;//wait rx
int    clean_memory_cnt = 0;
int	   work_hour 	= 0;
int    download_ended 	= 1;//A에서 처음 한번만 변수 초기화를 위해
int	   download_finish 	= 1;//D로 시작해서 A로 데이터 받고, LP로 데이터 다운로드 모두 완료되면 2
//0-Start
//1-완료
       
//--1~8H까지 다운로드 되기도 하네...젠장 뭐하는짓인지
//int		down_work_total[WORK_HOUR_MAX];
//int		down_work_cnt[WORK_HOUR_MAX];

//int   	down_workhourindex;
int		reading_count = 0;
//0- 카운트값 읽고 있지 않음
//1- 지금 Order 작업 완료된게 있음을 확인하는 카운트 읽고 있는중...
//2- 카운트값 읽는 것이 완료되면 다운로드 함수 실행하게
//3- 카운트값 읽는거 금지, D명령어 오면 3, 다운로드 완료되면 0
//4- 완료된 작업이 있으니 번지 초기화하기전까지 작업 완료 체크하지 말자 ----사용안함

String	KResFinishOrder = "";

bool    msgUIShow;
String	msgUI1, msgUI2, msgUI3;


//======================================================
//A0400000231115032103414 4  84819416600221 2 2 84 410580003<>5

void ParseAData(String data, bool debug=false)
{
  int offset = 2; 
  int ii;   
  
  try
  {
  //M/C address
  String parse = Copy(data, offset, 2);// data[2] + data[3];
  //_TRACE(ICO_ERROR, Format("Parse 1(Unit Addr) : %s", [parse]));
  offset = offset + 2;                                
      
  ii = StrToInt(parse);
  gLP_UnitAddr = ii;
                               
  //SO_ID column name
  parse = Copy(data, offset, 9);                          
  if(debug) _TRACE(ICO_ERROR, Format("Parse 2(ID0) : %s", [parse]));
  offset = offset + 9;     
  
  ii = StrToInt(parse);
  gLP_ID0 = ii;
                    
  //Working Date ngay thuc hien  
  parse = Copy(data, offset, 4);                          
  //_TRACE(ICO_ERROR, Format("Parse 3 : %s", [parse]));
  offset = offset + 4;
              
  ii = StrToInt(parse);
  Extended mm = ii / 100;
  gLP_WorkDateDD = Trunc(mm);
  gLP_WorkDateMM = ii%100;
  
  //adate - FGA date       DDMM
  parse = Copy(data, offset, 4);                          
  //_TRACE(ICO_ERROR, Format("Parse 4(AsyDate) : %s", [parse]));
  offset = offset + 4;  
                   
  ii = StrToInt(parse);
  mm = ii / 100;
  gLP_AssyDateDD = Trunc(mm);
  gLP_AssyDateMM = ii%100;
      
  //H - 1
  parse = Copy(data, offset, 1);                          
  //_TRACE(ICO_ERROR, Format("Parse 5(H) : %s", [parse]));
  offset = offset + 1;   
  
  ii = StrToInt(parse); 
  work_hour = ii;
    
  //ML - 2
  parse = Copy(data, offset, 2);                          
  //_TRACE(ICO_ERROR, Format("Parse 6(ML) : %s", [parse]));
  offset = offset + 2;
  ii = StrToInt(Trim(parse)); 
  gLP_ML = ii;
                      
  //size - 3
  parse = Copy(data, offset, 3);                          
  String ckSpace = Copy(data, offset+2, 1);                          
  if(debug) _TRACE(ICO_ERROR, Format("Parse 7(size) : %s, %s, %d", [parse, ckSpace, Length(Trim(ckSpace))]));
  offset = offset + 3;  
  
  ii = StrToInt(Trim(parse));
  if(Length(Trim(ckSpace)) == 0) gLP_Size = ii * 10;//반드시 세자리인데 한자리일경우 뒤에는 스페이스로 온다. 
  else gLP_Size = ii;
  if(debug) _TRACE(ICO_ERROR, Format("Parse 7(size) 2 : %d", [gLP_Size]));
        
  //qty - 3
  parse = Copy(data, offset, 3);                          
  if(debug) _TRACE(ICO_ERROR, Format("Parse 8(qty) : %s", [parse]));
  offset = offset + 3;  
  
  ii = StrToInt(parse); 
  gLP_OrderQty = ii;
                      
  //style - 9
  parse = Copy(data, offset, 9);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 9(style) : %s", [parse]));
  offset = offset + 9;                     
  
  ii = StrToInt(parse); 
  gLP_Style = ii;
  
                     
  //part - 2
  parse = Copy(data, offset, 2);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 10(part) : %s", [parse]));
  offset = offset + 2;   
  
  ii = StrToInt(parse); 
  gLP_Part = ii;
                      
  //part series
  parse = Copy(data, offset, 1);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 11(part series) : %s", [parse]));
  offset = offset + 1;
                       
  ii = StrToInt(parse);    
  gLP_PartSeries = ii; 
  
  gLP_Part = (gLP_Part * 10) + gLP_PartSeries;
                
  //layer - 2
  parse = Copy(data, offset, 2);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 12(layer) : %s", [parse]));
  offset = offset + 2;
    
  ii = StrToInt(Trim(parse)); 
  gLP_Layer = ii;
                    
  //cide - 2
  parse = Copy(data, offset, 2);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 13(cdie) : %s", [parse]));
  offset = offset + 2;    
  
  ii = StrToInt(Trim(parse)); 
  gLP_CDie = ii;
                      
  //tar cnt - 3
  parse = Copy(data, offset, 3);                          
if(debug) _TRACE(ICO_ERROR, Format("Parse 14(tarcnt) : %s", [parse]));
  offset = offset + 3;
    
  ii = StrToInt(parse); 
  gLP_TargetCnt = ii;
                   
  //mode - 1
  
  
  parse = Copy(data, offset, 1);                          
  //if(debug) 
  _TRACE(ICO_ERROR, Format("Parse 15(mode) : %s, %d", [parse, Length(parse)]));
  offset = offset + 1;          

  try
  {
	  ii = StrToInt(Trim(parse)); //1(LED Direct),2(LED 2Shift,4(LED OutSide)
	  gLP_Mode = ii;
  }
  except
  {
	  gLP_Mode = 0;
  }	 
  
  
  //gLP_EAperPair
  //Pair per - 1
  parse = Copy(data, offset, 1);                          
  //_TRACE(ICO_ERROR, Format("Parse 16(EAperPair) : %s", [parse]));
  offset = offset + 1;              
  
  ii = StrToInt(Trim(parse)); 
  gLP_EAperPair = ii;
  
  
  //dow - 1  date 0
  parse = Copy(data, offset, 1);                          
  //_TRACE(ICO_ERROR, Format("Parse 17(dow) : %s", [parse]));
  offset = offset + 1;
     
  ii = StrToInt(parse); 
  gLP_DOW = ii;

  
  //ID1 - 3
  parse = Copy(data, offset, 3);                          
  //_TRACE(ICO_ERROR, Format("Parse 18(sort key) : %s", [parse]));
  offset = offset + 3;
                      
  ii = StrToInt(parse); 
  gLP_ID1 = ii;

  
  //cuts - 3 remain count
  parse = Copy(data, offset, 3);                          
  //_TRACE(ICO_ERROR, Format("Parse 19(remain_cut) : %s", [parse]));
  offset = offset + 3;
 
  ii = StrToInt(parse); 
  gLP_Remain = ii;  
  
  //gender - 1
  parse = Copy(data, offset, 1);                          
  //_TRACE(ICO_ERROR, Format("Parse 19(gender) : %s", [parse]));
  offset = offset + 1;
 
  ii = StrToInt(parse); 
  gLP_Gender = ii;  
  
  gLP_Lamp = 0;
  }
  except
  {
	_TRACE(ICO_ERROR, Format("parse error : %s", [data]));
  }
  
  switch(gLP_Mode)
  {
     case 1: gLP_Lamp = 1;
	 case 2: gLP_Lamp = 2;
	 case 4: gLP_Lamp = 4;
  }
  
  switch(gLP_Gender)
  {
     case 1: gLP_Lamp = gLP_Lamp + 0x10;
	 case 2: gLP_Lamp = gLP_Lamp + 0x20;
	 case 3: gLP_Lamp = gLP_Lamp + 0x40;
	 case 4: gLP_Lamp = gLP_Lamp + 0x80;
  }  
}

//==================================
void InitVar()
{
	//gOrderTotal = 0;  
    sLastOrder = "";  
	gDownloadedCount = 0;
	
	int i, j;
	for(i=1; i<LPUNIT_MAX; i++) 
		for(j=0; j<WORK_HOUR_MAX; j++)
			gJJ_OrderTotal[i][j] = 0;
}

//======================================================
void initDownloadVar()
{
	int i, j;
	gJJ_DownloadNum = 0;
	
	for(i=0; i<LPUNIT_MAX; i++) gLP_DownloadAddress[i] = 0;
	for(j=0; j<WORK_HOUR_MAX; j++) gLP_DownloadHour[j] = 0;
	
	for(i=1; i<LPUNIT_MAX; i++) 
		for(j=0; j<WORK_HOUR_MAX; j++)
			gJJ_OrderTotal[i][j] = 0;
}

//==================================
void InitOrder(int addr, int hour_idx)
{
	int i;
	//Extended page;
	
	//page = gJJ_OrderList[addr][hour_idx].Count/5;

	clean_memory_cnt = gJJ_OrderList[addr][hour_idx].Count%5;
	if(clean_memory_cnt > 0)
	{
	  clean_memory_cnt = 5 - clean_memory_cnt;                 
	  //page += 1;
	}    
	else
	{
		clean_memory_cnt += 5;//LP program bug.
	}
	//gLP_DisplayPage = Trunc(page) - 1;   //0이 첫번째 페이지

	gOrderCount = gJJ_OrderList[addr][hour_idx].Count;
	gLP_PlanQty = gJJ_OrderTotal[addr][hour_idx];

	gDownloadedCount = 0;
	
	gLP_WorkHour = hour_idx;
	
	Timer1.Enabled = true; //update ui max value
    Timer2.Enabled = true; //update ui progress value
             
	_TRACE(ICO_ERROR, Format("total order 1 : %d, %d, %d, %d", [gOrderCount, gLP_PlanQty, gDownIndex, clean_memory_cnt]));             
}


//======================================================
void StartDownload(int unit_addr, int hour_index)
{	
	if(reading_count == 1) //지금 작업 완료 카운트 읽고 있는중...
	{	
		reading_count = 2;//K커맨드 끝나고 다시 이 함수 실행하게
		return;
	}
	
	download_ended = 1;
	sLastOrder = "";	 
	   
	InitOrder(unit_addr, hour_index);
		
	gDownIndex = 0;
	ParseAData(gJJ_OrderList[unit_addr, hour_index].Strings[0]);// gOrderList[gDownIndex]);  
			 
	//_TRACE(ICO_ERROR, Format("total order 2 : %d, %d, %d, %s", [clean_memory_cnt, gLP_WorkHour, gLP_PlanQty, gOrderList[0]]));
	SetNextDownloadStep2(SET_ASSYDATE_ORDER);
}

//======================================================
//A0100000205315032103714 55 14819416600011 4 1007 210640003:46
//K080000182251506 4 12510501613414613454943?8
//K050000123890906 6 2"1040006111158111215611:
//K010000020062103 4 110 17123170621170621
//K010000020062103 4 101 17039153831153831

//ID1 -> 3	
//ncut -> 3	
//hhmmss0 -> 6	
//hhmmss1-> 6
	
String makeDBUpdatePacket(String sOrderDB)
{
	String ret = "";//K";
		
	_TRACE(ICO_ERROR, Format("make db upload : %s", [sOrderDB]));
	ParseAData(sOrderDB);
	
	ret = Format("%s%.02d", [ret, gLP_UnitAddr]);	
	ret = Format("%s%.09d", [ret, gLP_ID0]);	
	ret = Format("%s%.02d", [ret, gLP_WorkDateDD]);//gLP_AssyDateDD]);
	ret = Format("%s%.02d", [ret, gLP_WorkDateMM]);//gLP_AssyDateMM]);	
	ret = Format("%s%2d", [ret, gLP_Layer]);
	ret = Format("%s%.02d", [ret, gLP_CDie]);
	ret = Format("%s%d", [ret, gLP_EAperPair]);
	ret = Format("%s%d", [ret, gLP_DOW]);
	ret = Format("%s%3d", [ret, gLP_ID1]);
	
	ret = Format("%s%.03d", [ret, gLP_TargetCnt]);
	ret = Format("%s%s", [ret, FormatDateTime("hhnnss", Now)]);
	ret = Format("%s%s", [ret, FormatDateTime("hhnnss", Now)]);
	
	return ret;
}

//======================================================
void OnTimerUpdateMsg(TObject Sender)
{
	g_timerUpdateMsg.Enabled = false;
	  
    if(!msgUIShow) 
    {
      Screen1.pnlMsg.Visible = false;
      return;
    }
    //if(!Screen1.pnlMsg.Visible) 
    Screen1.pnlMsg.Visible = msgUIShow;
    
	Screen1.lbMsg1.Caption = msgUI1;
	Screen1.lbMsg2.Caption = msgUI2;
	Screen1.lbLPAddr.Caption = msgUI3;
}

//======================================================
void setMsg(bool aShow, String msg1="", String msg2="", String msg3="")
{
	msgUIShow = aShow;
    msgUI1 = msg1;
	msgUI2 = msg2;
	msgUI3 = msg3;
	                         
    g_timerUpdateMsg.Enabled = false;
    g_timerUpdateMsg.Interval = 1;

	g_timerUpdateMsg.OnTimer = &OnTimerUpdateMsg;
	g_timerUpdateMsg.Enabled = true;
}

//1. Rx from VB======================================================
void isFinishOrder(Variant a_RxValues)
{
	if(download_finish == 0) return;//다운로드중이면 빠져나간다
	
	String sAddr = "";
	int iAddr = 0;
	try
	{
		sAddr = Format("%s%s", [Chr(a_RxValues[2]), Chr(a_RxValues[3])]);
		iAddr = StrToInt(sAddr);
	}
	except
	{
		_TRACE(ICO_ERROR, "isFinishOrder error");
	}
	
	int iListCnt = 0;
	int iFH = 0;//
	int iFI = 0;
	if(iAddr > 0) 
	{
		iFH = gLP_FinishWork_Hour[iAddr];
		iFI = gLP_FinishWork_Index[iAddr];
		iListCnt = gJJ_OrderList[iAddr][iFH].Count;
	}
	
	/*for(int i=1; i<LPUNIT_MAX; i++)
	{
		for(int j=0; j<WORK_HOUR_MAX; j++)
		{
			if( gJJ_OrderList[i][j].Count > 0) _TRACE(ICO_ERROR, Format("%d%d = %d", [i, j, gJJ_OrderList[i][j].Count]));
		}
	}*/
	
	_TRACE(ICO_ERROR, Format("finish 1? : %s, %d, %d%d=%d, %d", [sAddr, iAddr, iFH, iFI, iListCnt, gLP_UnitAddr]));
	
	if((iFH > 0) && (iFI > 0))
	{
		_TRACE(ICO_ERROR, Format("finish 2 : %d > %d?", [iListCnt, iFI]));
		iFI = iFI - 1;
		if(iListCnt > iFI)
		{
			String sRes = gJJ_OrderList[iAddr][iFH].Strings[iFI];
						
			sRes = makeDBUpdatepacket(sRes);//K는 스크립트 디바이스에서 덧붙인다
			_TRACE(ICO_ERROR, Format("======finish 3 : %s, %d, %d, %s", [sAddr, iAddr, iFH, sRes]));

			gLP_FinishWork_Addr   = iAddr;
			gLP_FinishWork[iAddr] = sRes;
			KResFinishOrder = sRes;//"-------------K1234...." + sRes;
			
			gLP_FinishWork_Hour[iAddr]  = 0;
			gLP_FinishWork_Index[iAddr] = 0;
			
			TimerKResponse.Enabled = true;//SetKResponse run
		}
		else
		{
			_TRACE(ICO_ERROR, Format("finish 3 : db list error, %d, %d", [iListCnt, iFI]));
			TimerGetWorkCount.Enabled = true;
		}
	}						
	else if(reading_count == 0)//check LP 작업 완료
	{
		gLP_UnitAddr = iAddr;
		TimerGetWorkCount.Enabled = true;
	}
}



//1. Rx from VB======================================================
void OnRxEventVB(int a_Result, Variant a_RxValues)
{
    int i, j, k;
	String sRx = "";
    int cnt = a_RxValues[0];
    if(cnt == 0) return;
    
    switch(a_RxValues[1])//command
    {
      //check finished data .. if exist completed work, response
      case Ord("K"):
      {
         for(i=1; i<cnt; i++)
		 {
		  sRx += Chr(a_RxValues[i]);
		 }                 
		
		 _TRACE(ICO_ERROR, Format("Script RxEvent 1 : %d, %s", [cnt, sRx]));
		
		 _TRACE(ICO_ERROR, Format("K command 1 : %d, %d", [reading_count, gA_OrderList.Count]));
		  		  
		  if(gA_OrderList.Count > 0)//only one time run. because K send continue after about 5 sec (A command sent). 딱 한번만 실행한다. A 완료된 다음 약 5초후부터 계속 K로 날라오니까...한번만 
		  { 			
			if(gLP_EraseAddress == 0)//only downloand
			{			
				int iFirst[LPUNIT_MAX][WORK_HOUR_MAX];
				for(i=0; i<LPUNIT_MAX; i++)
				{
					for(j=0; j<WORK_HOUR_MAX; j++)
					{
						iFirst[i][j] = 0;
					}
				}
				  
				for(i=0; i<gA_OrderList.Count; i++)
				{
					String sParse = gA_OrderList.Strings[i];
					ParseAData(sParse);//, false, a_RxValues);
									  
					String section2 = "Unit_" + IntToStr(gLP_UnitAddr) + IntToStr(work_hour);
					  
					if(iFirst[gLP_UnitAddr][work_hour] == 0)
					{
						iFirst[gLP_UnitAddr][work_hour] = 1;
						
						gJJ_Ini.EraseSection(section2);
						//_TRACE(ICO_ERROR, Format("add 2_2 : %s, %d, %d", [section, work_hour, down_work_cnt[work_hour]]));
						gJJ_Ini.WriteString(section2, "UpdateTime", DateTimeToStr(Now));
						//_TRACE(ICO_ERROR, Format("add 2_3 : %s, %d, %d", [section, work_hour, down_work_cnt[work_hour]]));
						gJJ_Ini.WriteInteger(section2, "Address", gLP_UnitAddr);
						//_TRACE(ICO_ERROR, Format("add 2_4 : %s, %d, %d", [section, work_hour, down_work_cnt[work_hour]]));
						gJJ_Ini.WriteInteger(section2, "Hour", work_hour);			
						//_TRACE(ICO_ERROR, Format("add 2_5 : %s, %d, %d", [section, work_hour, down_work_cnt[work_hour]]));
						gJJ_Ini.WriteInteger(section2, "Downloaded", 0);		
						
						gJJ_OrderTotal[gLP_UnitAddr][work_hour] = 0;
					}
					
					gJJ_OrderList[gLP_UnitAddr][work_hour].Add(sParse);
					gJJ_OrderTotal[gLP_UnitAddr][work_hour] += gLP_OrderQty;
					
					gJJ_Ini.WriteInteger(section2, "Count", gJJ_OrderList[gLP_UnitAddr][work_hour].Count);				
					gJJ_Ini.WriteString(section2, IntToStr(gJJ_OrderList[gLP_UnitAddr][work_hour].Count), sParse);
				}
			}
			else//move
			{
				int iMoveHour[LPUNIT_MAX][WORK_HOUR_MAX], iCnt;
				for(i=0; i<LPUNIT_MAX; i++)
				{
					for(j=0; j<WORK_HOUR_MAX; j++)
					{
						iMoveHour[i][j] = 0;
					}
				}
				
				for(i=0; i<gA_OrderList.Count; i++)
				{
					String sParse2 = gA_OrderList.Strings[i];
					ParseAData(sParse2);
					
					iMoveHour[gLP_UnitAddr][work_hour]++;
									
					String section3 = "Unit_" + IntToStr(gLP_UnitAddr) + IntToStr(work_hour);
					//_TRACE(ICO_ERROR, Format("E command 1_1 : %d, %d, %s", [gOrderTotal, gLP_EraseAddress, section3]));
						
					gJJ_OrderList[gLP_UnitAddr][work_hour].Add(sParse2);
					gJJ_OrderTotal[gLP_UnitAddr][work_hour] += gLP_OrderQty;
					
					gJJ_Ini.WriteString (section3, "UpdateTime", DateTimeToStr(Now));
					gJJ_Ini.WriteInteger(section3, "Downloaded", 0);	
					gJJ_Ini.WriteInteger(section3, "Count", gJJ_OrderList[gLP_UnitAddr][work_hour].Count);		
					
					gJJ_Ini.WriteString (section3, IntToStr(gJJ_OrderList[gLP_UnitAddr][work_hour].Count), sParse2);
					
					int iID = gLP_ID0;
					iCnt = gJJ_OrderList[gLP_EraseAddress][work_hour].Count;
					for(j=iCnt-1; j>-1; j--)
					{
						String s1 = gJJ_OrderList[gLP_EraseAddress][work_hour].Strings[j];
						ParseAData(s1, false);										
						if(iID == gLP_ID0)
						{
							gJJ_OrderList[gLP_EraseAddress][work_hour].Delete(j);
							break;
						}
					}
				}
				
				String ss = "";
				for(i=0; i<gJJ_DownloadNum; i++)
				{
					ss += Format("%d%d ", [gLP_DownloadAddress[i], gLP_DownloadHour[i]]);
				}
				_TRACE(ICO_ERROR, Format("1 -------- move list : %s", [ss]));
				
				//erase order data and save ini
				//gOrderTotal = 0;
				for(j=0; j<LPUNIT_MAX; j++)
				{
					for(i=0; i<WORK_HOUR_MAX; i++)
					{
						if(iMoveHour[j][i] > 0)
						{
							gJJ_OrderTotal[gLP_EraseAddress][i] = 0;
							
							String section4 = "Unit_" + IntToStr(gLP_EraseAddress) + IntToStr(i);
							//_TRACE(ICO_ERROR, Format("E command 1_2 : %s", [section4]));
							gJJ_Ini.EraseSection(section4);
							
							gJJ_Ini.WriteString (section4, "UpdateTime", DateTimeToStr(Now));
							gJJ_Ini.WriteInteger(section4, "Address", gLP_EraseAddress);
							gJJ_Ini.WriteInteger(section4, "Hour", i);			
							gJJ_Ini.WriteInteger(section4, "Downloaded", 0);	

							iCnt = gJJ_OrderList[gLP_EraseAddress][i].Count;
							gJJ_Ini.WriteInteger(section4, "Count", iCnt);	
							
							if(!findSameConfig(gLP_EraseAddress, i)) 
							{
								gLP_DownloadAddress[gJJ_DownloadNum] = gLP_EraseAddress;
								gLP_DownloadHour[gJJ_DownloadNum] = i;
								gJJ_DownloadNum++;
							}
								
							for(k=0; k<iCnt; k++)
							{
								String s3 = gJJ_OrderList[gLP_EraseAddress][i].Strings[k];
								gJJ_Ini.WriteString(section4, IntToStr(k+1), s3);		
								
								ParseAData(s3, false);	
								
								gJJ_OrderTotal[gLP_EraseAddress][i] += gLP_OrderQty;
							}
						}
					}
				}
				gA_OrderList.Clear();
				
				ss = "";
				for(i=0; i<gJJ_DownloadNum; i++)
				{
					ss += Format("%d%d ", [gLP_DownloadAddress[i], gLP_DownloadHour[i]]);
				}
				_TRACE(ICO_ERROR, Format("2 -------- move list : %s", [ss]));
			}
			gA_OrderList.Clear();		
			
			if(gJJ_DownloadNum > 0)
			{
				gJJ_DownloadIndex = 0;
				int iUnitAddr1 = gLP_DownloadAddress[gJJ_DownloadIndex];
				int iHour1 = gLP_DownloadHour[gJJ_DownloadIndex];
				int iCount1 = gJJ_OrderList[gLP_UnitAddr][work_hour].Count;
				StartDownload(iUnitAddr1, iHour1);
				
				setMsg(true, "SCADA -> LP Download Start", Format("HOUR : %d, Order Count : %d", [iHour1, iCount1]), "LP #" + IntToStr(iUnitAddr1));
			}
		  }
		  else
		  {
			isFinishOrder(a_RxValues);
		  }
		  
		  gLP_EraseAddress = 0;
      }
                   
      //move order M-M    .E021059:0;:.
      case Ord("E"):
      { 
		for(i=1; i<cnt; i++)
		 {
		  sRx += Chr(a_RxValues[i]);
		 }                 
		
		 _TRACE(ICO_ERROR, Format("Script RxEvent 1 : %d, %s", [cnt, sRx]));
		
		String sAddrE = Format("%s%s", [Chr(a_RxValues[2]), Chr(a_RxValues[3])]);
		int iAddrErase = StrToInt(sAddrE);//erase target LP address
		gLP_EraseAddress = iAddrErase;
      }
                    
      //delete order - 처음만 들어오는가보네...두번째 이후부터는 D커맨드 안들어오고 A로 바로
      case Ord("D"):
      {                    
        for(i=1; i<cnt; i++)
		 {
		  sRx += Chr(a_RxValues[i]);
		 }                 
		
		 _TRACE(ICO_ERROR, Format("Script RxEvent 1 : %d, %s", [cnt, sRx]));
		
		String sAddrD = Format("%s%s", [Chr(a_RxValues[2]), Chr(a_RxValues[3])]);
		int iAddrD = StrToInt(sAddrD);
		_TRACE(ICO_ERROR, Format("delete? : %s, %d", [sAddrD, iAddrD]));
		
		download_finish = 0;//download start
		
		InitVar();
		
		reading_count = 3;
      }
       
	  //K응답을 하고, DB에 저장하기 전에 finish work에 대한 check하는것 같음
      case Ord("C"):
      {                    
        for(i=1; i<cnt; i++)
		 {
		  sRx += Chr(a_RxValues[i]);
		 }                 
		
		 _TRACE(ICO_ERROR, Format("Script RxEvent 1 : %d, %s", [cnt, sRx]));		
      }
	  
      //add
	  //A, A, A .... K   download
	  //A, E, A .E.. K   move order m1 to m2
      case Ord("A"):
      {		
		for(i=1; i<cnt; i++)
		{
			if(i == 49) //mode data is hex, it's not ascii
			{
				if(a_RxValues[i] == 0x10) sRx += "1";
				else if(a_RxValues[i] == 0x20) sRx += "2";
				else if(a_RxValues[i] == 0x40) sRx += "4";
				else sRx += " ";
				
				//_TRACE(ICO_ERROR, Format("%d : %.02x, %s, %s", [i, a_RxValues[i], Chr(a_RxValues[i]), sRx]));				
			}
		    else sRx += Chr(a_RxValues[i]);
		}                 
		
		_TRACE(ICO_ERROR, Format("Script RxEvent 1 : %d, %s", [cnt, sRx]));
		
		download_finish = 0;//download start
		
		if(sLastOrder != sRx)
        {
		  if(download_ended == 1)
		  {
			download_ended = 0;
			
			InitVar();
			initDownloadVar();
			
			gA_OrderList.Clear();
		  }
		 
		  
		  ParseAData(sRx);//mode data replace from hex to text...0x10 -> 1, 0x20 -> 2, 0x40 -> 4
		 
		  if(!findSameConfig(gLP_UnitAddr, work_hour)) 
		  {
			gLP_DownloadAddress[gJJ_DownloadNum] = gLP_UnitAddr;
			gLP_DownloadHour[gJJ_DownloadNum] = work_hour;
			gJJ_DownloadNum++;
		  }
			
          sLastOrder = sRx;     
          
		  gA_OrderList.Add(sRx);
             
          setMsg(true, "VB -> SCADA Downloading", Format("Order Count : %d", [gA_OrderList.Count]), "LP #" + IntToStr(gLP_UnitAddr));
		  
          _TRACE(ICO_ERROR, Format("Add Data : %d", [gA_OrderList.Count]));
        }                                    
      }
    }         
    
    _TRACE(ICO_ERROR, Format("Script RxEvent 2 : %d, %d, %d, %d, %s", [a_Result, Length(a_RxValues), gOrderCount, a_RxValues[0], sRx]));        
}

//==================================
bool findSameConfig(int addr, int hour)
{
	for(int i=0; i<gJJ_DownloadNum; i++)
	{
		if((gLP_DownloadAddress[i] == addr) && (gLP_DownloadHour[i] == hour))
		{
			return true;
		}
	}
	
	return false;
}

//==================================
//call OnResFinishWork(TimerFinishWork)  ... not use
void SetResFinishWork()
{
	//_TRACE(ICO_ERROR, Format("finish work : %d, %d", [gLP_FinishWork_Hour[], gLP_FinishWork_Index]));        
	
	//reading_count = 0;	
	//TimerGetWorkCount.Enabled = true;
	
	//gDownloadStep = SET_INITWORKCOUNT;
	//TimerInitWorkCount.Enabled = true;
}

//==================================
void SetNextDownloadStep2(int aStep)
{
    gDownloadStep = aStep;   
     
    //_TRACE(ICO_ERROR, Format("next step 1 : %d, cnt=%d/%d", [gDownloadStep, gDownIndex, gOrderCount]));
          
    if(gDownloadStep == SET_EACHROWDATA)
    {
      int addr = gLP_DownloadAddress[gJJ_DownloadIndex];
	  int hour = gLP_DownloadHour[gJJ_DownloadIndex];
	  
	  //_TRACE(ICO_ERROR, Format("next step 2 : %d, cnt=%d/%d", [gDownloadStep, gDownIndex, gOrderCount]));
      ParseAData(gJJ_OrderList[addr][hour].Strings[gDownloadedCount]);//gOrderList[gDownIndex]);     

		//setMsg(true, "SCADA -> LP Downloading", Format("HOUR : %d, Order Count : %d", [down_workhourindex, gOrderTotal]));
      //_TRACE(ICO_ERROR, Format("next step 3 : %d, cnt=%d/%d", [gDownloadStep, gDownIndex, gOrderCount]));
    }  
    else if(gDownloadStep == SET_EACHROWDATA_CLEAN)
    {
      gLP_Size      = 0; 
      gLP_OrderQty  = 0; 
      gLP_Style     = 0; 
      gLP_Part      = 0; 
      gLP_PartSeries = 0; 
      gLP_Layer     = 0; 
      gLP_CDie      = 0; 
      gLP_TargetCnt = 0; 
      gLP_Lamp      = 0;  
      gLP_Remain    = 0;
    }       
    
    Timer3.Enabled = true;//call LPDownload
}


//==================================
//ModBus result
void OnLPResult(int result, Variant aRxValues)
{            
    gDownloadProcessing = false;
    _TRACE(ICO_ERROR, Format("LP result : %d, %d, %d/%d, %d", 
		[result, gDownloadStep, gDownloadedCount, gOrderCount, gDownIndex]));
    
    switch(gDownloadStep) 
    {       
        case SET_ASSYDATE_ORDER: 
		{
		  SetNextDownloadStep2(SET_EACHROWDATA);
        }  
		
        case SET_EACHROWDATA: 
        {
          int addr = gLP_DownloadAddress[gJJ_DownloadIndex];
		  int hour = gLP_DownloadHour[gJJ_DownloadIndex];
			
		  if(result > 0)//next order download
		  {			
			gDownloadedCount++;
		  }
          
		  /*
		  try		  
          {
		  _TRACE(ICO_ERROR, Format("download 1 : %d/%d, %d/%d, %d", 
			[gDownloadedCount, gOrderCount, gDownIndex, gOrderTotal, clean_memory_cnt]));
		  }
		  except{}*/
		  
		  if(gJJ_OrderList[addr][hour].Count > gDownloadedCount)
		  { 
			setMsg(true, "SCADA -> LP Downloading", Format("HOUR : %d, Download : %d/%d", [hour, gDownloadedCount, gOrderCount]), "LP #" + IntToStr(addr));			
			SetNextDownloadStep2(SET_EACHROWDATA);  		
		  } 
		  else if(clean_memory_cnt > 0)//남은 order는 0으로 채우기 위해
		  { 
			_TRACE(ICO_ERROR, Format("down clear : %d", [clean_memory_cnt]));
			
			if(result > 0) 
			{
				clean_memory_cnt--;
			}
			SetNextDownloadStep2(SET_EACHROWDATA_CLEAN);
		  }
		  else 
		  {
			String section2 = "Unit_" + IntToStr(addr) + IntToStr(hour);
			gJJ_Ini.WriteInteger(section2, "Downloaded", 1);		  
			
			gDownloadedCount = 0;			
			SetNextDownloadStep2(SET_TOTALROWCOMPLETED);
		  }
        }  		
         
        case SET_EACHROWDATA_CLEAN:
        {
		  _TRACE(ICO_ERROR, Format("download clean : %d/%d, %d", [gDownloadedCount, gOrderCount, clean_memory_cnt]));
		  
		  if(result > 0)          
          {
            if(clean_memory_cnt > 0)//남은 order는 0으로 채우기 위해
			{
				gDownloadedCount++;
				clean_memory_cnt--;
				int addr2 = gLP_DownloadAddress[gJJ_DownloadIndex];
				int hour2 = gLP_DownloadHour[gJJ_DownloadIndex];
                setMsg(true, "LP Clean", Format("HOUR : %d, Clean : %d, %d", [hour2, gDownloadedCount, clean_memory_cnt]), "LP #" + IntToStr(addr2));			
				SetNextDownloadStep2(SET_EACHROWDATA_CLEAN);						
			}
			else
			{
				//ParseAData(gOrderList[gDownIndex]);  
				
				String section3 = "Unit_" + IntToStr(addr) + IntToStr(hour);
				gJJ_Ini.WriteInteger(section3, "Downloaded", 1);	 
			
				gDownloadedCount = 0;				
				SetNextDownloadStep2(SET_TOTALROWCOMPLETED);
			}
          }		  
          else 
          {
            SetNextDownloadStep2(SET_EACHROWDATA_CLEAN);						
          }
        }  
        
        case SET_TOTALROWCOMPLETED:
        {		   
		   gDownloadedCount = 0;
		   SetNextDownloadStep2(SET_CURRENTTIME); 
        }
		
		case SET_CURRENTTIME:
		{		
			gJJ_DownloadIndex++;
			if(gJJ_DownloadNum > gJJ_DownloadIndex)
			{
				int addr3 = gLP_DownloadAddress[gJJ_DownloadIndex];
				int hour3 = gLP_DownloadHour[gJJ_DownloadIndex];
		  
				StartDownload(addr3, hour3);
				
				//InitOrder(addr3, hour3);
				//SetNextDownloadStep2(SET_ASSYDATE_ORDER); 
				
				//setMsg(true, "SCADA -> LP Downloading", Format("HOUR : %d, Order Count : %d", [hour3, gOrderTotal]));
			}
			else//다운로드 완료되었으면 작업완료 체크한다.
			{
				//initDownloadVar();
				
				download_finish = 1;//다운로드 완료
				gDownloadedCount = 0;
				
				Timer1.Enabled = true;
				Timer2.Enabled = true;
				
				_TRACE(ICO_ERROR, "================ get Work Count Start");
				reading_count = 0;
				TimerGetWorkCount.Enabled = true;//CheckWorkCount
				
				setMsg(false);
			}
		}
		
		case SET_INITWORKCOUNT://초기화 했으니 계속 작업 완료 체크한다
		{
			_TRACE(ICO_ERROR, "================ get Work Count Start");
			
			reading_count = 0;	
			TimerGetWorkCount.Enabled = true;
		}
    }
}

//==================================
void Download_InfoOrder(int unit_addr, int mm, int dd, int orderPage, int hour, int ml, int planqty)
{
    Variant tx = VarArrayCreate([0, 30], varInteger);                       
    
    //Assy Date  IndexCnt	Hour	M.L	Plan Qty
    //30         32	        33	    34	35

    int mb_addr = 30;
    int wmode = 2;
    int size  = 12;
           
    //assy.date(1) - dd    
    tx[0] = (dd >> 8) && 0xff;
    tx[1] = (dd) && 0xff;
    
    //assy.date(1) - mm
    tx[2] = (mm >> 8) && 0xff;
    tx[3] = (mm && 0xff);  
    
    //ordercnt               
    tx[4] = (orderPage >> 8) && 0xff;
    tx[5] = (orderPage && 0xff);
    
    //hour               
    tx[6] = (hour >> 8) && 0xff;
    tx[7] = (hour && 0xff);
    
    //ml               
    tx[8] = (ml >> 8) && 0xff;
    tx[9] = (ml && 0xff);
    
    //planqty               
    tx[10] = (planqty >> 8) && 0xff;
    tx[11] = (planqty && 0xff);
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnLPResult, wait_rxtime);
    
    gDownloadProcessing = true;
}  


//No.	Size	Order Qty.	Style#	Part#	Layer	C.Die	Tar.CNT	Ramp
//36	37	    38	        39	    42	    44	    45	    46	    47     48  49

//==================================
void Download_EachOrderData(
    int unit_addr, int no, int size, int orderqty, 
    int sty1, int sty2, int sty3, int pt1, int eapair,
    int layer, int cdie, int targetcnt, int ramp, int remain, int clean=0)
{
    Variant tx = VarArrayCreate([0, 30], varInteger);
    
    int mb_addr = 36 + (no*14);
    int wmode = 2;
    int xsize = 28;
           
    //index    
	if(clean == 0)
	{
		int idx = no + 1;	
		tx[0] = (idx >> 8) && 0xff;
		tx[1] = (idx) && 0xff;
	}
	else//clean
	{
		tx[0] = 0;
		tx[1] = 0;
	}
    tx[2] = (size >> 8) && 0xff;	
    tx[3] = (size) && 0xff;
    tx[4] = (orderqty >> 8) && 0xff;
    tx[5] = (orderqty) && 0xff;
       
    tx[6] = (sty1 >> 8) && 0xff;
    tx[7] = (sty1) && 0xff;
    tx[8] = (sty2 >> 8) && 0xff;
    tx[9] = (sty2) && 0xff;
    tx[10] = (sty3 >> 8) && 0xff;
    tx[11] = (sty3) && 0xff;

    tx[12] = (pt1 >> 8) && 0xff;
    tx[13] = (pt1) && 0xff;
	
    tx[14] = (eapair >> 8) && 0xff;
    tx[15] = (eapair) && 0xff;

    tx[16] = (layer >> 8) && 0xff;
    tx[17] = (layer) && 0xff;

    tx[18] = (cdie >> 8) && 0xff;
    tx[19] = (cdie) && 0xff;

    tx[20] = (targetcnt >> 8) && 0xff;
    tx[21] = (targetcnt) && 0xff;

    tx[22] = (ramp >> 8) && 0xff;
    tx[23] = (ramp) && 0xff;   

    int lp_area = 0;
    tx[24] = (remain >> 8) && 0xff;
    tx[25] = (remain) && 0xff;
    tx[26] = (lp_area >> 8) && 0xff;
    tx[27] = (lp_area) && 0xff;
               
    _TRACE(ICO_ERROR, Format("send modbus each row data : %d, %d", [no, mb_addr]));
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, xsize, tx, &OnLPResult, wait_rxtime);
    
    gDownloadProcessing = true;
}  

//==================================
void Download_Completed(int unit_addr, int time)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
    
    int mb_addr = 1999;
    int wmode = 2;//mwReg
    int size  = 4;
           
    //completed download, and next move memory in LP    
    tx[0] = (time >> 8) && 0xff;
    tx[1] = (time) && 0xff;
    tx[2] = 0;
    tx[3] = 0;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnLPResult, wait_rxtime);
    
    gDownloadProcessing = true;
}  


//==================================
void SetCurrentTime(int unit_addr, int time)
{
    Variant tx = VarArrayCreate([0, 10], varInteger);
    
    int mb_addr = 1512;
    int wmode = 2;
    int size  = 4;
           
    //completed download, and next move memory in LP    
    tx[0] = (time >> 8) && 0xff;
    tx[1] = (time) && 0xff;
    tx[2] = 0;
    tx[3] = 0;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnLPResult, wait_rxtime);
    
    gDownloadProcessing = true;
}  

//순차적으로 증가하면서 체크한다.
void setNextLPCheck()
{
	gLP_UnitAddrCompleted++;
	if(gLP_UnitAddrCompleted > LPUNIT_MAX)
	{
		gLP_UnitAddrCompleted = 1;
	}
	
	reading_count = 0;	
	TimerGetWorkCount.Enabled = true;//GetWorkCount 실행
}

	
//==================================
//ModBus result, (Hour-2, Index-2) => 00, 00이면 완료된게 없음
void OnLPResult2(int result, Variant aRxValues)
{            
	if(result <= 0) 
    {
      _TRACE(ICO_ERROR, Format("LP result2 : %d, %d", [result, reading_count]));
	  
	  if(reading_count == 2)//다운로드할게 있음 
	  {
		reading_count = 0;	
		
		int addr = gLP_DownloadAddress[gJJ_DownloadIndex];
		int hour = gLP_DownloadHour[gJJ_DownloadIndex];
		  
		StartDownload(addr, hour);
		return;
	  }
	
	  setNextLPCheck();
      return;
    }
    
	try
	{
		String s = "";
		for(int i=0; i<result; i++)
		{
		  s += IntToStr(aRxValues[i]) + " ";
		}
		_TRACE(ICO_ERROR, Format("LP result2 : %d, %d, %s", [result, Length(aRxValues), s]));
    }
	except
	{
	}
   
	//완료된 작업이 있는가
	if((aRxValues[1] > 0) && (aRxValues[3] > 0))//hour, index
	{
		gLP_FinishWork_Hour[gLP_UnitAddrCompleted]  = aRxValues[1];
		gLP_FinishWork_Index[gLP_UnitAddrCompleted] = aRxValues[3];
		
		try
		{
			_TRACE(ICO_ERROR, Format("finish work : %d, %d, %d", [gLP_UnitAddrCompleted, gLP_FinishWork_Hour[gLP_UnitAddrCompleted], gLP_FinishWork_Index[gLP_UnitAddrCompleted]]));
		}
		except
		{
		}
	}
		
    if(reading_count == 2)//다운로드할게 있음 
	{
		reading_count = 0;		

		int addr1 = gLP_DownloadAddress[gJJ_DownloadIndex];
		int hour1 = gLP_DownloadHour[gJJ_DownloadIndex];
		
		StartDownload(addr1, hour1);
		return;
	}
	
	setNextLPCheck();	
}

//==================================
//LP : 작업 완료된 오더 체크
void GetWorkCount(int unit_addr)
{
    if(reading_count == 3)// || (reading_count == 4))//다운로드쪽 진행중임
	{
		_TRACE(ICO_ERROR, "LP downloding....getting work count stop");
		return;
	}
	
	Variant tx = VarArrayCreate([0, 30], varInteger);                       
    
    reading_count = 1;

    int mb_addr = 1520;
    int rmode = 10 + 3;//3=mrHoldingReg, read할때는 10베이스 위에서 read모드 해야한다.
    int size  = 2;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, rmode, size, tx, &OnLPResult2, wait_rxtime);
}  

//==================================
void SetInitWorkCount(int unit_addr)
{
	_TRACE(ICO_ERROR, Format("-------Set Init count : %d", [unit_addr]));
	
    Variant tx = VarArrayCreate([0, 10], varInteger);
    
    int mb_addr = 1522;
    int wmode = 2;
    int size  = 4;
           
    tx[0] = 0;
    tx[1] = 1;
    tx[2] = 0;
    tx[3] = 0;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnLPResult, wait_rxtime);
    
    //gDownloadProcessing = true;
}


//==================================
void SetKResponse()
{
	SetTagValue(Tag1, KResFinishOrder);
	
	_TRACE(ICO_ERROR, "-------SetKResponse");
	gDownloadStep = SET_INITWORKCOUNT;
	TimerInitWorkCount.Enabled = true;
}


//==================================
//vb에서 rx되고 파싱 완료될때 콜백 함수 등록 
void InitCallBackFromVB()
{
	SetUnitCallback(UTag1, &OnRxEventVB);   
    
    
    //ParseAData("A0400000231115032103414 4  84819416600221 2 2 84 410580003<>5");
}

//==================================
{
}