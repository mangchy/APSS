#language C++Script

#include "GlobalScripts.pas"
//==================================
String sMold_CD, sMold_Size;
void ReadBarcode()
{
	TDateTime dt = GetTagUpdateTime(TagBarcode);
	if(gBarcodeTime != dt)
    {
		String sBarcode = Trim(GetTagValueS(TagBarcode));
		if(Length(sBarcode) < 2) return;

		frmScreen1_4.lbReadTime.Caption = DateTimeToStr(Now);
        frmScreen1_4.pnlBarcode.Caption = sBarcode;
		
		gBarcodeTime = dt;
		
		//SetDebug(Format("%s, %s, %d, %d", [sBarcode, gMachinePrefix, Pos(gMachinePrefix, sBarcode), gBarcodeStep]));
		if(Pos(gMachinePrefix, sBarcode) > 0)//found MACHINE CODE.....IPI-MCA17-08-L
		{
			gBarcodeMCA = Copy(sBarcode, 5, 5);
			
			string sStation = Copy(sBarcode, 11, 2);
			gBarcodeStation = StrToIntDef(sStation, 0);
			
			string sSide = Copy(sBarcode, 14, 2);
			gBarcodeSide = StrToIntDef(sSide, 0);	
			
			int iGridRow;
			for(int i=0; i<ALL_ORDERS; i++)
			{
				iGridRow = gWorkingRow[i];
				if(iGridRow == -1) continue;
				
				String version_id  = frmScreen1.dhGrid1.GetCellData(iGridRow, COLUMN_VERSION_ID);
				if(version_id == sBarcode)
				{
					gBarcodeStep = 1;
					
					String sInfo = frmScreen1.dhGrid1.GetCellData(iGridRow, COLUMN_MOLDSIZE);//mold id     MS249037-1 10, MS249037-1 10T      
					int pos = Pos(" ", sInfo);
					sMold_CD   = Copy(sInfo, 1, pos-1);
					sMold_Size = Copy(sInfo, pos+1, Length(sInfo));
					SetDebug(Format("%d, Machine barcode : %s, %s, %s, %s, %s, %s, %s", [iGridRow, sBarcode, gBarcodeMCA, sStation, sSide, sInfo, sMold_CD, sMold_Size]));	
					break;
				}
			}
			return;
		}
		
		if(gBarcodeStep == 1)//mold barcode       MS005280-JJME06T06TA
		{
			gBarcodeStep = 0;
			
			gMoldID[gBarcodeStation-1] = sBarcode;
			int found = compareMold(sBarcode, sMold_CD, sMold_Size);
			
			SetDebug(Format("Mold barcode : %s, %s, %s, %d", [sBarcode, sMold_CD, sMold_Size, found]));	
			
			
		} 		
    }	
}

//==================================
{
}