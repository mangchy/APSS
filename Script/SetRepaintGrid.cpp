#language C++Script

#include "GlobalScripts.pas"
//==================================
void SetRepaintGrid()
{
    TimerRepaintGrid.Enabled = false;
    frmScreen1.dhGrid1.Repaint(); 
}

//==================================
{
}