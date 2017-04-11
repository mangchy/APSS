//Device : Barcode Scanner
//Rx : ASCII Code ... 0x0D

int		rx_index;
int 	protocol_anal = 0;//
int 	rx[128];
//int		resAddress;
//int		resDataLen;
//int		reqType;		//0-read, 1-write

int		IOInterval		= 1000;
int		iFrameTime		= 50;//msec
int		iRxTimeOut		= 2000;//msec

int		iDAQIONum		= 0;
int 	iUseIOIndex;	//사용 유닛의 IO인덱스
String	barcode;
//int		iCommFailNum 	= 0;

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
	DevInfo.Values["Vendor"] 		= "System";
	DevInfo.Values["DeviceName"]	= "Barcode Scanner";
	DevInfo.Values["Category"] 		= "Barcode";
	DevInfo.Values["CreateDate"]	= "2015.02.11";
	DevInfo.Values["FileCreation"]  = "2015.02.11";
	DevInfo.Values["ModifyDate"]	= "2015.02.11";
	DevInfo.Values["Revision"] 		= "1.0";
	DevInfo.Values["Descript"] 		= "Barcode Scanner";
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
	iIOIndex = Add_DAQIO("Barcode", stString, patArray, srtReadContinue, 2, 0, 0, "", "Barcode Value");

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
TPropChangeResult OnUnitPropChange(int Sender, int param_index, String value, int PResultEvent)
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
bool CommDAQIORead(int PScriptUnit, int io_index, int a_nStation, String a_sDevice, int a_nAddress, TCommWriteMode txMode)
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
	return true;
}

//-------------------------------------------------------------------------------
//① 설       명  : Rx 데이터 파싱
//② 인       수  : 
//③ 리       턴  :
//-------------------------------------------------------------------------------
int OnRxData(int Sender, Variant a_Data)
{
	//_TRACE(ICO_NONE, "barcode rx1 : " + Format("%d : %.02X", [protocol_anal, a_Data]));	
	if(a_Data == 0x0D)
	{		
		//rx[protocol_anal] = NULL;
		barcode = Utf8ToAnsi(barcode);
		//_TRACE1(ICO_NONE, 1, "barcode rx : " + Format("%s", [barcode]));	
		SetIODAQData(Sender, 0, 0, -2, barcode, true);//-1:Alarm, -2 : message
		//protocol_anal = 0;
		barcode = "";
		//rx[protocol_anal++] = a_Data;
	}
	else
	{
		barcode = barcode + Chr(a_Data);
		//rx[protocol_anal] = a_Data;
		//protocol_anal++;
	}
	
	return -1;
}


{
}