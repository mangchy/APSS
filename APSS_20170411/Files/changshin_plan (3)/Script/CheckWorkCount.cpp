#language C++Script

#include "GlobalScripts.pas", "Global.pas", "InitCallBackFromVB.cpp"


//==================================
void CheckWorkCount()
{
    if(gDownloadProcessing) return;
    
    if(TEST_LP_ADDR_FIX == 1) gLP_UnitAddr = 1;//�ϴ� �׽�Ʈ
    
    GetWorkCount(gLP_UnitAddrCompleted);
}

//==================================
{
}