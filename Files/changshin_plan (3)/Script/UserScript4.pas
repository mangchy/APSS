#language PascalScript

uses 'GlobalScripts.pas', 'Global.pas', 'InitCallBackFromVB.cpp';

//==================================
procedure UserScript4;
begin
    InitCallBackFromVB;    
    
    Screen1.dhProgress1.UserValue := 0;
    LoadProgram;     
    
    Screen1_1.dhEdit1.Text := gVBProgramPath;
    Screen1_1.dhEdit2.Text := gVBProgramName;
    
    Screen1_1.dhCheckBox1.Checked := gUnitConnected[0];
    Screen1_1.dhCheckBox2.Checked := gUnitConnected[1];
    Screen1_1.dhCheckBox3.Checked := gUnitConnected[2];
	Screen1_1.dhCheckBox4.Checked := gUnitConnected[3];
	Screen1_1.dhCheckBox5.Checked := gUnitConnected[4];
	Screen1_1.dhCheckBox6.Checked := gUnitConnected[5];
	Screen1_1.dhCheckBox7.Checked := gUnitConnected[6];
	Screen1_1.dhCheckBox8.Checked := gUnitConnected[7];	
end;

//==================================
begin
end.