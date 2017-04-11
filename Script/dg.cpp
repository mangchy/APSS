#language C++Script
#include "GlobalScripts.pas"


//=======================================================================================
int dataGrid(
	string staddr, 		//1
	string msize, 		//2
	string mcolor, 		//3
	string planstart, 	//4
	string planend, 	//5
	int    qtyplan, 	//6
	int    qtyact, 		//7
	int    osdplan, 	//8
	int    osdact, 		//9
	string inj, 		//10
	string actstart, 	//11
	string actend, 		//12
	string rmark, 		//13
	string sort_key, 	//14
	string side, 		//15
	string station,		//16
	string moldcode,	//17
	string so_id,		//18
	string color_name,	//19
	string machine_cd,	//20
	string prs_qty,		//21
	TColor a1, 			//22
	TColor a2, 			//23
	bool aUpdate)		//24
{
	int row = frmScreen1.dhGrid1.SetAddRow(false);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 0, staddr, false, a1, a2);//0x00009393, clBlack);//
    frmScreen1.dhGrid1.SetCelldataColor2(row, 1, msize, false, a1, a2); //0x00009393, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 2, mcolor, false, a1, a2); //0x00009393, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 3, planstart, false, a1, a2); //0x00009393, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 4, planend, false, a1, a2); //0x00009393, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 5, IntToStr(qtyplan), false, a1, clBlack);//0x00FFAF60, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 6, IntToStr(qtyact), false, a1, clBlack);//clWhite);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 7, IntToStr(osdplan), false, a1, clBlack);//clWhite);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 8, IntToStr(osdact), false, a1, clBlack);//clWhite);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 9, inj, false, a1, clBlack);//0x00FFAF60, clBlack);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 10, actstart, false, a1, clBlack);//clWhite);
    frmScreen1.dhGrid1.SetCelldataColor2(row, 11, actend, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 12, rmark, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 13, sort_key, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 14, side, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 15, station, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 16, moldcode, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 17, so_id, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 18, color_name, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 19, machine_cd, false, a1, clBlack);//clWhite);
	frmScreen1.dhGrid1.SetCelldataColor2(row, 20, prs_qty, aUpdate, a1, clBlack);//clWhite);
	
	return row;
}

{
	
}