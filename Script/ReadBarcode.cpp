#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"
//==================================
String sMold_CD, sMold_Size, sGridMoldData;
int iGridRow;
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
		
		SetDebug(Format("read barcode : %s, %s, %d, step:%d", [sBarcode, gMachinePrefix, Pos(gMachinePrefix, sBarcode), gBarcodeStep]));
		if(Pos(gMachinePrefix, sBarcode) > 0)//found MACHINE CODE.....IPI-MCA17-08-L
		{
			gBarcodeMCA = Copy(sBarcode, 5, 5);
			
			string sStation = Copy(sBarcode, 11, 2);
			gBarcodeStation = StrToIntDef(sStation, 0);
			
			string sSide = Copy(sBarcode, 14, 2);
			gBarcodeSide = StrToIntDef(sSide, 0);				
			
			for(int i=0; i<ALL_ORDERS; i++)
			{
				iGridRow = gWorkingRow[i];
				if(iGridRow == -1) continue;
				
				String version_id  = frmScreen1.dhGrid1.GetCellData(iGridRow, COLUMN_VERSION_ID);
				version_id = StringReplace(version_id, "_", "-");   
                //function StringReplace(const S : string; const OldPattern: string; const NewPattern) : string
				SetDebug(Format("compare barcode : %s, %s", [sBarcode, version_id]));
				if(version_id == sBarcode)
				{
					gBarcodeStep = 1;
					
					String sInfo = frmScreen1.dhGrid1.GetCellData(iGridRow, COLUMN_MOLDSIZE);//mold id     MS249037-1 10, MS249037-1 10T      
					sGridMoldData= sInfo;
					int pos1 	= Pos("-", sInfo);
					sMold_CD   	= Copy(sInfo, 1, pos1-1);
					pos1 		= Pos(" ", sInfo);
					sMold_Size 	= Copy(sInfo, pos1+1, Length(sInfo));
					SetDebug(Format("  => row %d, Machine barcode : %s, %s, Station=%s, Dir=%s, Grid Data=%s, CD=%s, Size=%s", [iGridRow, sBarcode, gBarcodeMCA, sStation, sSide, sInfo, sMold_CD, sMold_Size]));	
					break;
				}
			}
			return;
		}
		
		if(gBarcodeStep == 1)//mold barcode       MS005280-JJME06T06TA
		{
			gBarcodeStep = 0;
			
			gMoldID[gBarcodeStation-1] = sBarcode;
			int found = findMoldFromDataMold(sBarcode, sMold_CD, sMold_Size);
			
			SetDebug(Format("Mold barcode : barcode=%s, grid=%s, %s, %s, %d", [sBarcode, sGridMoldData, sMold_CD, sMold_Size, found]));	
			
			//if(found)//found MOLD of DATA_MOLD table
			{
				//grid=MS249037-1 11, barcode=MS249037-JJME013013A
				
				int pos2 = Pos("-", sBarcode);
				String sMold_CD2   = Copy(sBarcode, 1, pos2-1);
				int len = Length(sBarcode);
				String sMold_Size2 = Copy(sBarcode, len-3, 3);
				if(sMold_Size2[1] == "0") sMold_Size2 = Copy(sMold_Size2, 2, 2);//if "09T" => 9T
				
				SetDebug(Format(" => same? : %s=?%s, %s=?%s", [sMold_CD, sMold_CD2, sMold_Size, sMold_Size2]));	
				if((sMold_CD == sMold_CD2) && (sMold_Size == sMold_Size2))
				{
					checkMoldChange(iGridRow, DOOR_AUTO);
				}
			}			
		} 		
    }	
}

//==================================
{
}