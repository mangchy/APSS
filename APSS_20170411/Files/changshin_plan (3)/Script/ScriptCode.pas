uses 'Global.pas', 'GlobalScripts.pas', 'UserScript4.pas', 'UpdateUI.cpp', 'UserScript7.cpp', 'LPDownload.cpp', 'CheckWorkCount.cpp', 'OnResFinishWork.cpp', 'OnSetInitWorkCount.cpp', 'ResKResponse.cpp', 'InitProgram.cpp', 'UserScript3.pas', 'UserScript2.pas', 'UserScript1.pas', 'UserScript5.pas', 'DownloadToLP.cpp', 'UserScript6.cpp', 'TestMBRead.cpp', 'SetMinimize.cpp', 'TestParse.cpp', 'InitCallBackFromVB.cpp', 'HideMsg.cpp', 'SetConfig.cpp', 'TestParse.cpp';

procedure OnTimerScreenDownload(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerScreenDownload.Enabled := false;
    	exit
    end;

    	TimerScreenDownload.Enabled := false;
    DownloadToLP;

end;

procedure OnTimerKResponse(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerKResponse.Enabled := false;
    	exit
    end;

    	TimerKResponse.Enabled := false;
    ResKResponse;

end;

procedure OnTimerInitWorkCount(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerInitWorkCount.Enabled := false;
    	exit
    end;

    	TimerInitWorkCount.Enabled := false;
    OnSetInitWorkCount;

end;

procedure OnTimerFinishWork(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerFinishWork.Enabled := false;
    	exit
    end;

    	TimerFinishWork.Enabled := false;
    OnResFinishWork;

end;

procedure OnTimerGetWorkCount(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerGetWorkCount.Enabled := false;
    	exit
    end;

    	TimerGetWorkCount.Enabled := false;
    CheckWorkCount;

end;

procedure OnTimer3(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	Timer3.Enabled := false;
    	exit
    end;

    	Timer3.Enabled := false;
    LPDownload;

end;

procedure OnTimer2(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	Timer2.Enabled := false;
    	exit
    end;

    	Timer2.Enabled := false;
    UserScript7;

end;

procedure OnTimer1(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	Timer1.Enabled := false;
    	exit
    end;

    	Timer1.Enabled := false;
    UpdateUI;

end;

procedure OnTimerLoadProgram(Sender:TObject);
begin
    if TerminateApp = true then
    begin
    	TimerLoadProgram.Enabled := false;
    	exit
    end;

    	TimerLoadProgram.Enabled := false;
    UserScript4;

end;

procedure OnHMIStart;
begin

    if TimerLoadProgram = nil then TimerLoadProgram := TTimer.Create(nil);

    if TimeCheckLoadProgram = nil then TimeCheckLoadProgram := TdhTimeCheck.Create;
    begin
        TimerLoadProgram.Name     := 'TimerLoadProgram';
        TimerLoadProgram.Enabled  := false;
        TimerLoadProgram.OnTimer  := @OnTimerLoadProgram;
        TimerLoadProgram.Interval := 1000;
    end;
    TimerLoadProgram.Enabled  := true;
    TimeCheckLoadProgram.Start;


    if Timer1 = nil then Timer1 := TTimer.Create(nil);

    if TimeCheck1 = nil then TimeCheck1 := TdhTimeCheck.Create;
    begin
        Timer1.Name     := 'Timer1';
        Timer1.Enabled  := false;
        Timer1.OnTimer  := @OnTimer1;
        Timer1.Interval := 1;
    end;


    if Timer2 = nil then Timer2 := TTimer.Create(nil);

    if TimeCheck2 = nil then TimeCheck2 := TdhTimeCheck.Create;
    begin
        Timer2.Name     := 'Timer2';
        Timer2.Enabled  := false;
        Timer2.OnTimer  := @OnTimer2;
        Timer2.Interval := 1;
    end;


    if Timer3 = nil then Timer3 := TTimer.Create(nil);

    if TimeCheck3 = nil then TimeCheck3 := TdhTimeCheck.Create;
    begin
        Timer3.Name     := 'Timer3';
        Timer3.Enabled  := false;
        Timer3.OnTimer  := @OnTimer3;
        Timer3.Interval := 1;
    end;


    if TimerGetWorkCount = nil then TimerGetWorkCount := TTimer.Create(nil);

    if TimeCheckGetWorkCount = nil then TimeCheckGetWorkCount := TdhTimeCheck.Create;
    begin
        TimerGetWorkCount.Name     := 'TimerGetWorkCount';
        TimerGetWorkCount.Enabled  := false;
        TimerGetWorkCount.OnTimer  := @OnTimerGetWorkCount;
        TimerGetWorkCount.Interval := 1000;
    end;


    if TimerFinishWork = nil then TimerFinishWork := TTimer.Create(nil);

    if TimeCheckFinishWork = nil then TimeCheckFinishWork := TdhTimeCheck.Create;
    begin
        TimerFinishWork.Name     := 'TimerFinishWork';
        TimerFinishWork.Enabled  := false;
        TimerFinishWork.OnTimer  := @OnTimerFinishWork;
        TimerFinishWork.Interval := 1;
    end;


    if TimerInitWorkCount = nil then TimerInitWorkCount := TTimer.Create(nil);

    if TimeCheckInitWorkCount = nil then TimeCheckInitWorkCount := TdhTimeCheck.Create;
    begin
        TimerInitWorkCount.Name     := 'TimerInitWorkCount';
        TimerInitWorkCount.Enabled  := false;
        TimerInitWorkCount.OnTimer  := @OnTimerInitWorkCount;
        TimerInitWorkCount.Interval := 1;
    end;


    if TimerKResponse = nil then TimerKResponse := TTimer.Create(nil);

    if TimeCheckKResponse = nil then TimeCheckKResponse := TdhTimeCheck.Create;
    begin
        TimerKResponse.Name     := 'TimerKResponse';
        TimerKResponse.Enabled  := false;
        TimerKResponse.OnTimer  := @OnTimerKResponse;
        TimerKResponse.Interval := 1;
    end;

    DAQConnect(true);
    DAQRun(true);
    InitProgram;

end;


procedure Screen1dhSpeedButton3Click(Sender:TObject);
begin

    if TimerScreenDownload = nil then TimerScreenDownload := TTimer.Create(nil);

    if TimeCheckScreenDownload = nil then TimeCheckScreenDownload := TdhTimeCheck.Create;
    begin
        TimerScreenDownload.Name     := 'TimerScreenDownload';
        TimerScreenDownload.Enabled  := false;
        TimerScreenDownload.OnTimer  := @OnTimerScreenDownload;
        TimerScreenDownload.Interval := 10;
    end;

    UserScript6;

end;


procedure OnHMIStop;
begin
    UserScript3;

    DAQConnect(false);
end;


procedure Screen1dhSpeedButton2Click(Sender:TObject);
begin
    UserScript2;

end;


procedure Screen1dhSpeedButton1Click(Sender:TObject);
begin
    UserScript1;

end;


procedure Screen1dhShapeButton1Click(Sender:TObject);
begin
    UserScript5;

    TerminateApp := true;
    FormHMIBaseRunner.Close;
end;


procedure Screen1dhShapeButton6Click(Sender:TObject);
begin
    Screen1_1.Parent := FormHMIBaseRunner;
    Screen1_1.BorderStyle := bsNone;
    Screen1_1.Align := alClient;
    Screen1_1.Show;
end;


procedure Screen1dhShapeButton2Click(Sender:TObject);
begin
    TestMBRead;

end;


procedure Screen1dhSpeedButton5Click(Sender:TObject);
begin
end;


procedure Screen1dhSpeedButton6Click(Sender:TObject);
begin
    SetMinimize;

end;


procedure Screen1dhSpeedButton7Click(Sender:TObject);
begin
    TestParse;

end;


procedure Screen1dhSpeedButton4Click(Sender:TObject);
begin
    InitCallBackFromVB;

end;


procedure Screen1dhLabel1Click(Sender:TObject);
begin
    HideMsg;

end;


procedure Screen1_1dhShapeButton1Click(Sender:TObject);
begin
    SetConfig;

    Screen1_1.Close;

end;


procedure Screen1_1dhSpeedButton7Click(Sender:TObject);
begin
    TestParse;

end;


begin
end.
