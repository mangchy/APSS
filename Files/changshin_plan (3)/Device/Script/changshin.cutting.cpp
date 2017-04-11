//Device : Indonesia ChangShin Cutting Plan Counter
//#include "device\\script\\lib\\CRC16.cpp"

#define STX    0x02 	//Start
#define ETX    0x03 	//End Text
#define EOT    0x04 	//End of Text

int		rx_index = 0;
int 	protocol_anal = 0;//
int 	rx[128];

int		IOInterval		= 0;
int		iFrameTime		= 50;//msec
int		iRxTimeOut		= 2000;//msec

int     iDAQIONum;
int     gResultEvent    = 0;

int crc16tab[256] =
[
0x0,   0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x210,   0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0xA50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0xC60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0xE70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0xA1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x2B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x8E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0xAF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0xCC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0xED1, 0x1EF0 ];


int		g_iRxCMD;
int     g_ScriptUnit = nil;
TTimer  g_TimerRes = nil;

//-------------------------------------------------------------------------------
//① 설       명  : 유닛 추가가 가능한지 여부
//② 인       수  : unitNum=현재 추가된 유닛 갯수
//③ 리       턴  :	true=추가가능, false=추가안됨
//-------------------------------------------------------------------------------
bool AvailableAddUnit(int unitNum)
{
	if(unitNum == 0) return true;
	return false;
}

int GetIOInterval()
{
	return IOInterval;
}

void SetIOInterval(int interval)
{
	IOInterval = interval;
}

int GetIOWizardType()
{
	return 0;//
}

Variant GetRxData()
{
	_TRACE1(ICO_NONE, 1, Format("GetRxData : %d", [rx_index]));

    if(rx_index == 0) return nil;

    Variant v = VarArrayCreate([0, rx_index-1], varInteger);
	for(int i=0; i<rx_index; i++) v[i] = rx[i];
	return v;
}
//-------------------------------------------------------------------------------
//① 설       명  : 디바이스 정보
//② 인       수  : 전역변수에 등록된 TStringList DevInfo에 저장
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetDeviceInfo()
{
	DevInfo.Values["Vendor"] 		= "ChangShin";
	DevInfo.Values["DeviceName"]	= "Cutting";
	DevInfo.Values["Category"] 		= "SmartFactory";
	DevInfo.Values["CreateDate"]	= "2016.05.25";
	DevInfo.Values["FileCreation"]  = "2016.05.25";
	DevInfo.Values["ModifyDate"]	= "2015.02.11";
	DevInfo.Values["Revision"] 		= "1.0";
	DevInfo.Values["Descript"] 		= "Cutting Plan Counter";
	DevInfo.Values["MajorRev"] 		= "1";
	DevInfo.Values["MinorRev"] 		= "0";

	DevInfo.Values["SupportDeviceNum"] = "1";//지원 디바이스 : 2개
	DevInfo.Values["SupportDevice1"] = "0";//RS-232
	//DevInfo.Values["SupportDevice2"] = "1";//TCP/IP
}

//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetProperty()
{
	int iPropIndex;
	AddPropCategory("Source");//0
}

//-------------------------------------------------------------------------------
//① 설       명  : IO 추가
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetIOList()
{
	int iIOIndex = 0;
	iIOIndex = Add_DAQIO("Comm", stString, patArray, srtReadContinueCallback, 2, 0, 0, "", "ModBus Comm Tag");

	iDAQIONum = iIOIndex;//총 IO 갯수
}


//-------------------------------------------------------------------------------
//① 설       명  : 디바이스 팝업메뉴 추가하기
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetDevicePopup()
{
	//AddDevicePopup("파라미터 모두 읽기");
}


//-------------------------------------------------------------------------------
//① 설       명  : 디바이스 팝업 메뉴 실행하기
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
int OnDevicePopupClick(int menu_index)
{

}

//-------------------------------------------------------------------------------
//① 설       명  : 유닛 팝업메뉴 추가하기
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetUnitPopup()
{
	//AddUnitPopup("IO Manager...");
}


//-------------------------------------------------------------------------------
//① 설       명  : 유닛 팝업 메뉴 실행하기
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
int OnUnitPopupClick(int menu_index)
{
	return 0;
}

//-------------------------------------------------------------------------------
//① 설       명  : 유닛 속성 클릭 이벤트
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
TPropChangeResult OnUnitPropChange(int Sender, int param_index, String value)
{
	return crSuccess;
}


//-------------------------------------------------------------------------------
//① 설       명  : PLC Device List, CPUS, CPUH는 D만 틀림(%D0~D32767)
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void GetPLCDeviceInfo()
{
}


//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void CommWrite(int Sender, int io_index, String command, int PResultEvent)
{

    //SetCommWrite(Sender, io_index, tx, iTx, iFrameTime, timeOut, IOInterval, PResultEvent);//iRepeatInterval);
}

//-------------------------------------------------------------------------------
//① 설       명  : 	실제 Write시작 이벤트...SetCommWrite는 Tx리스트에 추가만함
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
void OnCommWriteStart(int a_IOIndex)
{
}


//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
bool CommDAQIORead(int PScriptUnit, int io_index, int a_nStation, String a_sDevice, int a_nAddress)
{
    return true;
}


//-------------------------------------------------------------------------------
//① 설       명  : 개별쓰기
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
bool CommDAQIOWrite(int PScriptUnit, int io_index, Variant a_Value, int a_nStation, String a_sCmd, int a_nAddress, int PResultEvent)
{
	gResultEvent = PResultEvent;

    sendResponse(Ord("K"), 10, a_Value);
    _TRACE(ICO_ERROR, Format("changshin : %d, %d, %s", [gResultEvent, PResultEvent, a_Value]));
    return true;
}


//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
int CRC16_init(string packet)
{
	int CRC = 65535;
	int CRCt;
	int i, t, tmp;

	tmp = Length(packet);

	for(i=0; i<tmp; i++)
	{
		t = (Ord(packet[i+1]) ^ CRC) && 0x00ff;
		CRCt = CRC >> 8;
		CRC = (CRCt ^ crc16tab[t]) && 0xffff;
	}

	return CRC ^ 0xffff;
}

//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
int CRC16_get(int crc, int offset)
{
 switch(offset)
 {
  case 0: return (crc)       && 15 || 48;
  case 1: return (crc >> 4)  && 15 || 48;
  case 2: return (crc >> 8)  && 15 || 48;
  case 3: return (crc >> 12) && 15 || 48;
 }
 return 0;
}

//-------------------------------------------------------------------------------
//① 설       명  :
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
string CRC16_gets(int crc)
{
  string scrc = "";

  int crc1 = (crc)       && 15 || 48;
  int crc2 = (crc >> 4)  && 15 || 48;
  int crc3 = (crc >> 8)  && 15 || 48;
  int crc4 = (crc >> 12) && 15 || 48;

  scrc = Format("%s%s%s%s", [Chr(crc1), Chr(crc2), Chr(crc3), Chr(crc4)]);

 return scrc;
}

//-------------------------------------------------------------------------------
void OnTimerRes(TObject Sender)
{
    g_TimerRes.Enabled = false;
    _TRACE(ICO_ERROR, "ON TIMER =================================================");
}

//-------------------------------------------------------------------------------
void startTimerResponse()
{
    if(g_TimerRes == nil) g_TimerRes = new TTimer(nil);

    g_TimerRes.Enabled  = false;
    g_TimerRes.Interval = 0;
    g_TimerRes.OnTimer  = &OnTimerRes;
    g_TimerRes.Enabled  = true;

    _TRACE1(ICO_NONE, 1, "start timer");//Format("start timer", []));
}

//-------------------------------------------------------------------------------
void sendResponse(int cmd, int delay, String res="")
{
    //_TRACE(ICO_NONE, "ontimer 1");//Format("ontimer", []));// : %s", [Chr(g_iRxCMD)]));

    Variant tx = VarArrayCreate([0, 50], varInteger);
	int i;
	int iTx  = 0;

	tx[iTx] = EOT;
    iTx++;

    //command id
    tx[iTx] = rx[1];
    iTx++;

    if(cmd == Ord("D"))
    {
        //addr(2)
        tx[iTx] = rx[2];
        iTx++;
        tx[iTx] = rx[3];
        iTx++;
    }
    else if(cmd == Ord("E"))
    {
        //addr(2)
        tx[iTx] = rx[2];
        iTx++;
        tx[iTx] = rx[3];
        iTx++;
    }
    else if(cmd == Ord("A"))
    {
        for(i=2; i<13; i++)
        {
            tx[iTx] = rx[i];
            iTx++;
        }
    }
    else if(cmd == Ord("K"))
    {
        //tx[iTx] =
        for(i=1; i<Length(res); i++)
        {
            tx[iTx] = Ord(res[i]);
            iTx++;
        }
    }
    else
    {
        return;
    }

    //make CRC(4)
    String packet = "";
    for(i=1; i<iTx; i++)
    {
        packet = packet + Chr(tx[i]);
    }

    int crc = CRC16_init(packet);
    for(i=0; i<4; i++)
    {
        tx[iTx] = CRC16_get(crc, i);
        iTx++;
    }

    tx[iTx] = ETX;
    iTx++;

    //startTimerResponse();

    _TRACE(ICO_NONE, Format("script send : %x, %s, %.02x, %d, %d", [g_ScriptUnit, packet, crc, iTx, delay]));
    int io_index = -1;
	//tx delay를 frame time으로 하자
    SetCommWrite(g_ScriptUnit, io_index, tx, iTx, delay, 0, 0, gResultEvent);//IOInterval
    //_TRACE(ICO_NONE, "ontimer 3");
}


//D => 02	44	30	31	31	30	30	30	38	38	30	36	03	CS	P500
//D => 04	44	30	31	3A	36	34	35	03	CS

//-------------------------------------------------------------------------------
//① 설       명  : Rx 데이터 파싱
//② 인       수  :
//③ 리       턴  :
//-------------------------------------------------------------------------------
int OnRxData(int PScriptUnit, Variant a_Data)
{
	if(a_Data == ETX)
	{
		//_TRACE(ICO_NONE, Format("ETX1 cut rx1 : %d : %s, %.02X", [protocol_anal, Chr(g_iRxCMD), a_Data]));

        /*String sRx = "";
        for(int i=1; i<protocol_anal; i++)//A command Mode값 (HEX) 변환하면서 에러나는게 있음
        {
			try
			{
				sRx = Format("%s%s", [sRx, Chr(rx[i])]);
			}
			except
			{
				_TRACE1(ICO_NONE, 1, Format("ETX2 error : %d/%d", [i, protocol_anal]));
			}
        }*/
		//rx[protocol_anal] = NULL;
		//_TRACE1(ICO_NONE, 1, Format("ETX2 %x, rx : %d, %s", [PScriptUnit, rx_index, sRx]));
		rx_index = protocol_anal;
        protocol_anal = 0;

        rx[0] = rx_index;

        g_ScriptUnit = PScriptUnit;
        int delay;
        switch(g_iRxCMD)
        {
            case Ord("D"):
            {
                SetCommResult(PScriptUnit, rx, rx_index);
                sendResponse(g_iRxCMD, 600);//delay = 200;
            }
            case Ord("A"):
            {
                SetCommResult(PScriptUnit, rx, rx_index);
                sendResponse(g_iRxCMD, 0);//delay = 0;
            }
            case Ord("K"):
            {
                SetCommResult(PScriptUnit, rx, rx_index);//sendResponse(g_iRxCMD, 0);//startTimerResponse();//{delay = 0; return 0;}
            }
            case Ord("E"):
            {
                SetCommResult(PScriptUnit, rx, rx_index);//sendResponse(g_iRxCMD, 0);//startTimerResponse();//{delay = 0; return 0;}
                sendResponse(g_iRxCMD, 0);//delay = 0;
            }
			case Ord("C"):
            {
                SetCommResult(PScriptUnit, rx, rx_index);//sendResponse(g_iRxCMD, 0);//startTimerResponse();//{delay = 0; return 0;}
            }
            //default : return 0;
        }

        //_TRACE1(ICO_NONE, 1, Format("ETX3 %x, rx : %s", [PScriptUnit, sRx]));

        return 0;//완료가 되면 -1이 아닌 값을 리턴
	}
	else if(a_Data == STX)
	{
		//_TRACE(ICO_NONE, "STX cut rx1");
		protocol_anal = 0;
	}
	else
	{
		rx[protocol_anal+1] = a_Data;

		if(protocol_anal == 0)//save Command ID
		{
			g_iRxCMD = a_Data;
			//_TRACE(ICO_NONE, "cut rx1 : " + Format("%d : %s", [protocol_anal, Chr(g_iRxCMD)]));
		}
		protocol_anal++;
	}

	return -1;
}


{
}