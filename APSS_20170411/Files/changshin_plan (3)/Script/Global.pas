#language PascalScript

uses 'GlobalScripts.pas';

//const

//////////////////////////////////

const
	//DOWNLOAD - gDownloadStep
	SET_ASSYDATE_ORDER		= 0;//ASSYDATE(2) : mmdd modbus addr : 100, ORDER COUNT(1) : 102
	SET_EACHROWDATA			= 1;
	SET_EACHROWDATA_CLEAN	= 2;
	SET_TOTALROWCOMPLETED	= 3;
	SET_CURRENTTIME			= 4;
	SET_INITWORKCOUNT		= 5;
	
	LPUNIT_MAX				= 20;
	WORK_HOUR_MAX			= 8;
	
	TEST_LP_ADDR_FIX		= 1;//1-FIX, 0-Flexible
	
var
    gVBProgramPath				: String;// = ProjectPath + "app\\Gathering data_CuttingPlan_JJ";
	gVBProgramName				: String;// = ProjectPath + "app\\Gathering data_CuttingPlan_JJ";
	gOrderListIni				: String = ProjectPath + 'OrderList.ini';
	gUnitConnected				: array[0..LPUNIT_MAX] of boolean;
	gVBProgramHandle			: Integer = 0;
	gDownloadStep				: Integer = -1;
	gDownloadProcessing			: boolean = false;
	//gOrderList					: array[0..100] of String;
	//gOrderTotal					: Integer = 0; //download해야할 작업지시 갯수
	gOrderCount					: Integer = 0; //download해야할 작업지시 갯수
	gDownloadedCount			: Integer = 0; //download한 작업 지시 갯수
	gDownIndex					: Integer;

	gLP_UnitAddrCompleted		: Integer = 1;
	
	//download info
	gLP_UnitAddr				: Integer = 1;
	gLP_WorkDateMM, gLP_WorkDateDD : Integer;
	gLP_AssyDateMM, gLP_AssyDateDD : Integer;
	//gLP_DisplayPage				: Integer;//표시수, 0이 처음 페이지(1~5 order)
	gLP_WorkHour				: Integer;
	gLP_ML						: Integer;
	gLP_PlanQty					: Integer;
	
	//1-2H, 3-4H, 5-6H, 7-8H이라 다운로드 시작되면 두개 시간대가 동시에 들어온다
	//gOrderHour					: array[0..100] of Integer;
	
	//download order, SCADA->LP
	gLP_ID0				   	: Integer;
	gLP_Size				: Integer;
	gLP_Style				: Integer;
	gLP_Part				: Integer;
	gLP_PartSeries			: Integer;
	gLP_Layer				: Integer;
	gLP_CDie				: Integer;
	gLP_OrderQty			: Integer;
	gLP_TargetCnt			: Integer;
	gLP_Mode				: Integer;
	gLP_DOW					: Integer;
	gLP_ID1				   	: Integer;
	gLP_EAperPair			: Integer;
	gLP_Gender				: Integer;
	gLP_Lamp				: Integer;
	gLP_Remain				: Integer;
	
	//작업 완료된거 응답
	gLP_FinishWork_Addr		: Integer = 0;
	
	gLP_FinishWork			: array[0..LPUNIT_MAX] of String;// = 0;
	gLP_FinishWork_Hour		: array[0..LPUNIT_MAX] of Integer;// = 0;
	gLP_FinishWork_Index	: array[0..LPUNIT_MAX] of Integer;// = 0;//시작은 1부터 시작

	gJJ_DownloadIndex		: Integer;
	gJJ_DownloadNum			: Integer = 0;	
	gLP_DownloadAddress		: array[0..LPUNIT_MAX] of Integer;
	gLP_DownloadHour		: array[0..WORK_HOUR_MAX] of Integer;
	gLP_EraseAddress		: Integer = 0;
	
	gJJ_OrderList			: array[1..LPUNIT_MAX, 0..WORK_HOUR_MAX] of TStringList;//[유닛,시간]
	gJJ_OrderTotal			: array[1..LPUNIT_MAX, 0..WORK_HOUR_MAX] of Integer;//[유닛,시간]
	gJJ_Ini					: TIniFile = TIniFile.Create(ProjectPath + 'OrderList.ini');
	
	gA_OrderList			: TStringList = TStringList.Create;
	
	g_timerUpdateMsg : TTimer = TTimer.Create(nil);
	
procedure OnExeFormResize(Sender : TObject);
begin
	//SetEmbedExeResize(gVBProgramHandle, Screen1.dhPanelFill1);
end;

//==================================
procedure LoadProgram;
begin
   if gVBProgramHandle <> 0 then exit;
   
   if not FileExists(gVBProgramPath) then exit;
   
   //ProjectPath + 'app\\Gathering data_CuttingPlan_JJ\\CuttingSystem_JJ.exe'
   gVBProgramHandle := SetEmbedExeRun(Screen1.dhPanelFill1, gVBProgramPath, '', gVBProgramName);//'Cutting System  V4.2_20160323');
   //Screen1.dhPanelFill1.OnResize := @OnExeFormResize;
end;

//==================================
procedure UnloadProgram;
begin
   Screen1.dhPanelFill1.OnResize := nil;
   if gVBProgramHandle <> 0 then SetEmbedExeClose(gVBProgramHandle);
   gVBProgramHandle := 0; 
end;


begin
end.