#language C++Script

#include "GlobalScripts.pas", "dg.cpp", "JJUtils.cpp", "GlobalJJ.cpp", "srcSQL.cpp"
//==================================
void RefreshSRC()
{
	TimerCheck.Enabled = false;	
	srcSQL();
}

//==================================
{
}