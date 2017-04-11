#language PascalScript

uses 'GlobalScripts.pas', 'Global.pas';

//==================================
procedure UserScript3;
begin
   gJJ_Ini.Free;
   
   if gVBProgramHandle <> 0 then SetEmbedExeClose(gVBProgramHandle);
   gVBProgramHandle := 0; 
end;

//==================================
begin
end.