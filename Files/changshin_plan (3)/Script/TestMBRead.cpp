#language C++Script

#include "GlobalScripts.pas", "Global.pas"

int wait_rxtime2 = 1;

//==================================
//ModBus result
void OnLPResultTest(int result, Variant aRxValues)
{            
    gDownloadProcessing = false;                            
    String s;
    for(int i=0; i<result; i++)
    {
      s += IntToStr(aRxValues[i]) + " ";
    }
    _TRACE(ICO_ERROR, Format("LP result : %d, %d, %s", [result, Length(aRxValues), s]));//Length(aRxValues)]));
   
}

//==================================
void TestRead()
{
    Variant tx = VarArrayCreate([0, 30], varInteger);                       
    
    //Assy Date  IndexCnt    Hour    M.L    Plan Qty
    //30         32            33        34    35
        
    int unit_addr = 1;
    int mb_addr = 1520;
    int rmode = 10 + 3;//mrHoldingReg
    int size  = 3;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, rmode, size, tx, &OnLPResultTest, wait_rxtime2);
    
    gDownloadProcessing = true;
}  


//==================================
void TestMBRead()
{
TestRead();
}

//==================================
{
}