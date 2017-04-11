#language C++Script

#include "GlobalScripts.pas"
//==================================
void SaveLogFile()
{
    //0-csv, 1-txt, 2-html, 3-rtf
    frmScreen1_3.dhGrid1.SaveFile(Format("%s\\log\\%s.txt", [ProjectPath, FormatDateTime("YYYYMMDD_HHNNSS", Now())]), 1);//txt파일 저장
}

//==================================
{
}