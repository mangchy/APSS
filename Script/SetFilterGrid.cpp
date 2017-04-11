#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"
//==================================
void SetFilterGrid()
{
    if(frmScreen1.cbxMC.ItemIndex == 0)
    {
        if(frmScreen1.cbxStation.ItemIndex == 0)
        {
            frmScreen1.dhGrid1.SetFilterRow2(COLUMN_MACHINE, "", COLUMN_STATION, "");
        }
        else
        {
            int istation = frmScreen1.cbxStation.ItemIndex;
            frmScreen1.dhGrid1.SetFilterRow2(COLUMN_STATION, Format("%.02d", [istation]), COLUMN_MACHINE, "");
        }
    }
    else
    {
        if(frmScreen1.cbxStation.ItemIndex == 0)
        {
            frmScreen1.dhGrid1.SetFilterRow2(COLUMN_MACHINE, frmScreen1.cbxMC.Text, COLUMN_STATION, "");
        }
        else
        {
            int istation2 = frmScreen1.cbxStation.ItemIndex;
            frmScreen1.dhGrid1.SetFilterRow2(COLUMN_MACHINE, frmScreen1.cbxMC.Text, COLUMN_STATION, Format("%.02d", [istation2]));
        }
    }
}

//==================================
{
}