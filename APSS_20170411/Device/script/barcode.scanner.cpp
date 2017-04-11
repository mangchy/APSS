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
int 	iUseIOIndex;	//��� ������ IO�ε���
String	barcode;
//int		iCommFailNum 	= 0;

//-------------------------------------------------------------------------------
//�� ��       ��  : ���� �߰��� �������� ����
//�� ��       ��  : unitNum=���� �߰��� ���� ����
//�� ��       ��  :	true=�߰�����, false=�߰��ȵ�
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
//�� ��       ��  : ����̽� ����
//�� ��       ��  : ���������� ��ϵ� TStringList DevInfo�� ����
//�� ��       ��  :
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
	
	DevInfo.Values["SupportDeviceNum"] = "1";//���� ����̽� : 2��
	DevInfo.Values["SupportDevice1"] = "0";//RS-232
	//DevInfo.Values["SupportDevice2"] = "1";//TCP/IP
}

//-------------------------------------------------------------------------------
//�� ��       ��  : 
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void GetProperty()
{
	int iPropIndex;
	AddPropCategory("Source");//0
}

//-------------------------------------------------------------------------------
//�� ��       ��  : IO �߰�
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void GetIOList()
{
	int iIOIndex = 0;                                   
	iIOIndex = Add_DAQIO("Barcode", stString, patArray, srtReadContinue, 2, 0, 0, "", "Barcode Value");

	iDAQIONum = iIOIndex;//�� IO ����
}


//-------------------------------------------------------------------------------
//�� ��       ��  : ����̽� �˾��޴� �߰��ϱ�
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void GetDevicePopup()
{
	//AddDevicePopup("�Ķ���� ��� �б�");
}


//-------------------------------------------------------------------------------
//�� ��       ��  : ����̽� �˾� �޴� �����ϱ�
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
int OnDevicePopupClick(int menu_index)
{
	
}

//-------------------------------------------------------------------------------
//�� ��       ��  : ���� �˾��޴� �߰��ϱ�
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void GetUnitPopup()
{
	//AddUnitPopup("IO Manager...");
}


//-------------------------------------------------------------------------------
//�� ��       ��  : ���� �˾� �޴� �����ϱ�
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
int OnUnitPopupClick(int menu_index)
{  
	return 0;
}

//-------------------------------------------------------------------------------
//�� ��       ��  : ���� �Ӽ� Ŭ�� �̺�Ʈ
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
TPropChangeResult OnUnitPropChange(int Sender, int param_index, String value, int PResultEvent)
{  
	return crSuccess;
}


//-------------------------------------------------------------------------------
//�� ��       ��  : PLC Device List, CPUS, CPUH�� D�� Ʋ��(%D0~D32767)
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void GetPLCDeviceInfo()
{
}


//-------------------------------------------------------------------------------
//�� ��       ��  : 
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void CommWrite(int Sender, int io_index, String command, int PResultEvent)
{

}

//-------------------------------------------------------------------------------
//�� ��       ��  : 	���� Write���� �̺�Ʈ...SetCommWrite�� Tx����Ʈ�� �߰�����
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
void OnCommWriteStart(int a_IOIndex)
{
}


//-------------------------------------------------------------------------------
//�� ��       ��  : 
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
bool CommDAQIORead(int PScriptUnit, int io_index, int a_nStation, String a_sDevice, int a_nAddress, TCommWriteMode txMode)
{
	return true;
}


//-------------------------------------------------------------------------------
//�� ��       ��  : ��������
//�� ��       ��  : 
//�� ��       ��  :
//-------------------------------------------------------------------------------
bool CommDAQIOWrite(int PScriptUnit, int io_index, Variant a_Value, int a_nStation, String a_sCmd, int a_nAddress, int PResultEvent)
{
	return true;
}

//-------------------------------------------------------------------------------
//�� ��       ��  : Rx ������ �Ľ�
//�� ��       ��  : 
//�� ��       ��  :
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