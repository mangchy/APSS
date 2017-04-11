#language C++Script

#include "GlobalScripts.pas", "LPComm.cpp", "GlobalJJ.cpp"
//==================================
void DownloadInitOrder()
{
	int 	inor_plncnt;
	int 	iosd_plncnt;
    int 	inor_actcnt;
	int 	iosd_actcnt;
    string 	shift 	= "A";//--later calculate from start_date
	Variant vso_id;
	String 	moldsize;
	String 	moldcolor; 
	
	try
	{
		int istat = gDownloadData[gDownloadedIdx];
	
		if(frmScreen1_4.cbxNPCnt.Checked) inor_plncnt = StrToIntDef(frmScreen1_4.edtNPCnt.Text, 0);
		else inor_plncnt = GetTagValueI(gTagNorP[istat]);
		
		if(frmScreen1_4.cbxNACnt.Checked) inor_actcnt = StrToIntDef(frmScreen1_4.edtNACnt.Text, 0);
		else inor_actcnt = GetTagValueI(gTagNorA[istat]);
		
		if(frmScreen1_4.cbxOPCnt.Checked) iosd_plncnt = StrToIntDef(frmScreen1_4.edtOPCnt.Text, 0);
		else iosd_plncnt = GetTagValueI(gTagOsdP[istat]);
		
		if(frmScreen1_4.cbxOACnt.Checked) iosd_actcnt = StrToIntDef(frmScreen1_4.edtOACnt.Text, 0);
		else iosd_actcnt = GetTagValueI(gTagOsdA[istat]);
		
		if(frmScreen1_4.cbxSOID.Checked) vso_id = StrToIntDef(frmScreen1_4.edtSOID.Text, 0);
		else vso_id = GetTagValueI(gTagSOID[istat]);
		
		if(frmScreen1_4.cbxMold.Checked) moldsize = frmScreen1_4.edtMold.Text;
		
		if(frmScreen1_4.cbxMoldColor.Checked) moldcolor = frmScreen1_4.edtMoldColor.Text;
			
		int istation = Int(istat/2);
		int iside 	 = Int(istat%2);
		int tagAddr  = gTagWrite[Int(istation/4)];
		
		LP_SendData(tagAddr, istation, iside, moldsize, moldcolor, shift, inor_plncnt, inor_plncnt-inor_actcnt, iosd_plncnt, iosd_plncnt-iosd_actcnt, vso_id);
		
		gDownloadedIdx++;
		frmScreen1_1.dhProgress1.UserValue = gDownloadedIdx;
		frmScreen1_1.dhLabel1.Caption = Format("%d/%d, Station : %d, Side : %d", [frmScreen1_1.dhProgress1.UserValue, frmScreen1_1.dhProgress1.MaxValue, istation, iside]);
		
		if(gDownloadedIdx == gDownloadNum)
		{
			gLPSOIDZeroNum = UpdateSOID();
			
			frmScreen1_1.Close;
			TimerInitOrder.Enabled = false;
			
			TimerGetAct.Enabled = frmScreen1.btnRun.Pressed;
		}
	}
	except
	{
		SetDebug(ExceptionMessage, clRed);
	}
}

//==================================
{
}