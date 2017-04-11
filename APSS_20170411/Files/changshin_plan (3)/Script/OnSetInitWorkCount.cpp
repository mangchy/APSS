#language C++Script

#include "GlobalScripts.pas", "Global.pas", "InitCallBackFromVB.cpp"
//==================================
void OnSetInitWorkCount()
{
    SetInitWorkCount(gLP_FinishWork_Addr);
}

//==================================
{
}