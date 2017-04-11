#language C++Script

#include "GlobalScripts.pas", "JJUtils.cpp", "LPComm.cpp"
//==================================
void TestUtil()
{
            
    int selrow = frmScreen1.dhGrid1.GetSelectRow;
    if(selrow > -1)
    {
        String sStartPlan = frmScreen1.dhGrid1.GetCellData(selrow, 3);
        String sDecode = DecodeShift(sStartPlan);//"16 08:20:20");       
        
        LP_Shift(mLShift1_1, sDecode);
    }
}

//==================================
{
}