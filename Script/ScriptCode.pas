uses 'GlobalScripts.pas', 'UpdateDateTime.cpp', 'DownloadNext.cpp', 'DownloadWork.cpp', 'TimerGetActual.cpp', 'TimerSetCheck.cpp', 'InitSetup.cpp', 'OnTimerAllClear2.cpp', 'DownloadInitOrder.cpp', 'SetRepaintGrid.cpp', 'ReadBarcode.cpp', 'TimerSetSyncTimeLP.cpp', 'SetFilterGrid.cpp', 'SetFilterGrid.cpp', 'UpdateVersionID.cpp', 'GetSelectOrder.cpp', 'SetFinishOrders.cpp', 'srcSQL.cpp', 'send.cpp', 'SetStop.cpp', 'ClearDebugMessage.cpp', 'SaveLogFile.cpp', 'SetAllDataItems.cpp', 'SetSyncTimeLP.cpp', 'SetMachineTag.cpp', 'SetMachineTag.cpp', 'SetMachineTag.cpp', 'InitStationOrder.cpp';

procedure OnTimerSyncTimeLP(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerSyncTimeLP.Enabled := false;
    	exit
    end;

    TimerSetSyncTimeLP;

end;

procedure OnTimerReadBarcode(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerReadBarcode.Enabled := false;
    	exit
    end;

    ReadBarcode;

end;

procedure OnTimerRepaintGrid(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerRepaintGrid.Enabled := false;
    	exit
    end;

    	TimerRepaintGrid.Enabled := false;
    SetRepaintGrid;

end;

procedure OnTimerInitOrder(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerInitOrder.Enabled := false;
    	exit
    end;

    DownloadInitOrder;

end;

procedure OnTimerAllClear(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerAllClear.Enabled := false;
    	exit
    end;

    OnTimerAllClear2;

end;

procedure OnTimerCheck(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerCheck.Enabled := false;
    	exit
    end;

    TimerSetCheck;

end;

procedure OnTimerGetAct(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerGetAct.Enabled := false;
    	exit
    end;

    TimerGetActual;

end;

procedure OnTimerDownloadWork(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerDownloadWork.Enabled := false;
    	exit
    end;

    DownloadWork;

end;

procedure OnTimerDownload(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerDownload.Enabled := false;
    	exit
    end;

    	TimerDownload.Enabled := false;
    DownloadNext;

end;

procedure OnTimerDateTime(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerDateTime.Enabled := false;
    	exit
    end;

    UpdateDateTime;

end;

procedure OnHMIStart;
begin
    DAQConnect(true);
    DAQRun(true);

    if TimerDateTime = nil then TimerDateTime := TTimer.Create(nil);

    if TimeCheckDateTime = nil then TimeCheckDateTime := TdhTimeCheck.Create;
    begin
        TimerDateTime.Name     := 'TimerDateTime';
        TimerDateTime.Enabled  := false;
        TimerDateTime.OnTimer  := @OnTimerDateTime;
        TimerDateTime.Interval := 1000;
    end;
    TimerDateTime.Enabled  := true;
    TimeCheckDateTime.Start;


    if TimerDownload = nil then TimerDownload := TTimer.Create(nil);

    if TimeCheckDownload = nil then TimeCheckDownload := TdhTimeCheck.Create;
    begin
        TimerDownload.Name     := 'TimerDownload';
        TimerDownload.Enabled  := false;
        TimerDownload.OnTimer  := @OnTimerDownload;
        TimerDownload.Interval := 1000;
    end;


    if TimerDownloadWork = nil then TimerDownloadWork := TTimer.Create(nil);

    if TimeCheckDownloadWork = nil then TimeCheckDownloadWork := TdhTimeCheck.Create;
    begin
        TimerDownloadWork.Name     := 'TimerDownloadWork';
        TimerDownloadWork.Enabled  := false;
        TimerDownloadWork.OnTimer  := @OnTimerDownloadWork;
        TimerDownloadWork.Interval := 1000;
    end;


    if TimerGetAct = nil then TimerGetAct := TTimer.Create(nil);

    if TimeCheckGetAct = nil then TimeCheckGetAct := TdhTimeCheck.Create;
    begin
        TimerGetAct.Name     := 'TimerGetAct';
        TimerGetAct.Enabled  := false;
        TimerGetAct.OnTimer  := @OnTimerGetAct;
        TimerGetAct.Interval := 1000;
    end;


    if TimerCheck = nil then TimerCheck := TTimer.Create(nil);

    if TimeCheckCheck = nil then TimeCheckCheck := TdhTimeCheck.Create;
    begin
        TimerCheck.Name     := 'TimerCheck';
        TimerCheck.Enabled  := false;
        TimerCheck.OnTimer  := @OnTimerCheck;
        TimerCheck.Interval := 300;
    end;

    InitSetup;


    if TimerAllClear = nil then TimerAllClear := TTimer.Create(nil);

    if TimeCheckAllClear = nil then TimeCheckAllClear := TdhTimeCheck.Create;
    begin
        TimerAllClear.Name     := 'TimerAllClear';
        TimerAllClear.Enabled  := false;
        TimerAllClear.OnTimer  := @OnTimerAllClear;
        TimerAllClear.Interval := 1000;
    end;


    if TimerInitOrder = nil then TimerInitOrder := TTimer.Create(nil);

    if TimeCheckInitOrder = nil then TimeCheckInitOrder := TdhTimeCheck.Create;
    begin
        TimerInitOrder.Name     := 'TimerInitOrder';
        TimerInitOrder.Enabled  := false;
        TimerInitOrder.OnTimer  := @OnTimerInitOrder;
        TimerInitOrder.Interval := 1000;
    end;


    if TimerRepaintGrid = nil then TimerRepaintGrid := TTimer.Create(nil);

    if TimeCheckRepaintGrid = nil then TimeCheckRepaintGrid := TdhTimeCheck.Create;
    begin
        TimerRepaintGrid.Name     := 'TimerRepaintGrid';
        TimerRepaintGrid.Enabled  := false;
        TimerRepaintGrid.OnTimer  := @OnTimerRepaintGrid;
        TimerRepaintGrid.Interval := 1000;
    end;


    if TimerReadBarcode = nil then TimerReadBarcode := TTimer.Create(nil);

    if TimeCheckReadBarcode = nil then TimeCheckReadBarcode := TdhTimeCheck.Create;
    begin
        TimerReadBarcode.Name     := 'TimerReadBarcode';
        TimerReadBarcode.Enabled  := false;
        TimerReadBarcode.OnTimer  := @OnTimerReadBarcode;
        TimerReadBarcode.Interval := 100;
    end;
    TimerReadBarcode.Enabled  := true;
    TimeCheckReadBarcode.Start;


    if TimerSyncTimeLP = nil then TimerSyncTimeLP := TTimer.Create(nil);

    if TimeCheckSyncTimeLP = nil then TimeCheckSyncTimeLP := TdhTimeCheck.Create;
    begin
        TimerSyncTimeLP.Name     := 'TimerSyncTimeLP';
        TimerSyncTimeLP.Enabled  := false;
        TimerSyncTimeLP.OnTimer  := @OnTimerSyncTimeLP;
        TimerSyncTimeLP.Interval := 500;
    end;

end;


procedure OnHMIStop;
begin
    DAQRun(false);
    DAQConnect(false);
end;


procedure frmScreen1dhSpeedButton3Click(Sender:TObject);
begin
    frmScreen1_3.Parent := FormHMIBaseRunner;
    frmScreen1_3.BorderStyle := bsNone;
    frmScreen1_3.Align := alClient;
    frmScreen1_3.Show;
end;


procedure frmScreen1dhSpeedButton4Click(Sender:TObject);
begin
    frmScreen1_4.Parent := FormHMIBaseRunner;
    frmScreen1_4.BorderStyle := bsNone;
    frmScreen1_4.Align := alClient;
    frmScreen1_4.Show;
end;


procedure frmScreen1cbxStationClick(Sender:TObject);
begin
    SetFilterGrid;

end;


procedure frmScreen1cbxMCClick(Sender:TObject);
begin
    SetFilterGrid;

end;


procedure frmScreen1dhSpeedButton6Click(Sender:TObject);
begin
    UpdateVersionID;

end;


procedure frmScreen1dhGrid1Click(Sender:TObject);
begin
    GetSelectOrder;

end;


procedure frmScreen1dhSpeedButton5Click(Sender:TObject);
begin
    SetFinishOrders;

end;


procedure frmScreen1dhShapeButton1Click(Sender:TObject);
begin
    DAQRun(false);
    DAQConnect(false);
    TerminateApp := true;
    FormHMIBaseRunner.Close;
end;


procedure frmScreen1dhSpeedButton1Click(Sender:TObject);
begin
    srcSQL;

end;


procedure frmScreen1btnRunClick(Sender:TObject);
begin
    send;

end;


procedure frmScreen1btnStopClick(Sender:TObject);
begin
    SetStop;

end;


procedure frmScreen1_3dhSpeedButton3Click(Sender:TObject);
begin
    frmScreen1.Parent := FormHMIBaseRunner;
    frmScreen1.BorderStyle := bsNone;
    frmScreen1.Align := alClient;
    frmScreen1.Show;
end;


procedure frmScreen1_3dhSpeedButton1Click(Sender:TObject);
begin
    ClearDebugMessage;

end;


procedure frmScreen1_3dhShapeButton1Click(Sender:TObject);
begin
    frmScreen1.Parent := FormHMIBaseRunner;
    frmScreen1.BorderStyle := bsNone;
    frmScreen1.Align := alClient;
    frmScreen1.Show;
end;


procedure frmScreen1_3btnLogSaveClick(Sender:TObject);
begin
    SaveLogFile;

end;


procedure frmScreen1_4dhSpeedButton3Click(Sender:TObject);
begin
    frmScreen1.Parent := FormHMIBaseRunner;
    frmScreen1.BorderStyle := bsNone;
    frmScreen1.Align := alClient;
    frmScreen1.Show;
end;


procedure frmScreen1_4cbxAllItemsClick(Sender:TObject);
begin
    SetAllDataItems;

end;


procedure frmScreen1_4dhShapeButton1Click(Sender:TObject);
begin
    frmScreen1.Parent := FormHMIBaseRunner;
    frmScreen1.BorderStyle := bsNone;
    frmScreen1.Align := alClient;
    frmScreen1.Show;
end;


procedure frmScreen1_4dhSpeedButton2Click(Sender:TObject);
begin
    SetSyncTimeLP;

end;


procedure frmScreen1_4cbxMCA1Change(Sender:TObject);
begin
    SetMachineTag;

end;


procedure frmScreen1_4cbxMCA2Change(Sender:TObject);
begin
    SetMachineTag;

end;


procedure frmScreen1_4cbxMCA3Change(Sender:TObject);
begin
    SetMachineTag;

end;


procedure frmScreen1_4btnInitializeClick(Sender:TObject);
begin
    InitStationOrder;

end;


begin
end.
