#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"


//==================================
void SetFinishOrders()
{
    String soid_sql = GetSelectedGrid();
    if(Length(soid_sql) > 0)
    {
        //ShowMessage(soid_sql);
        
        String sqlsoid = Copy(soid_sql, 1, Length(soid_sql));
		insertFinishSQL(sqlsoid);
		
		ShowMessage(Format("Completed finish order : %s", [sqlsoid]));
    }
    
    /*
    String ss = "";
	String soid_sql = "";
        
    int iSel[500]; 
	String sSO_ID[500];
    iSel[0] = frmScreen1.dhGrid1.GetFirstSelectedRow();  
    if(iSel[0] > 0)
    {
		int iDownNum = 0;
        int iCnt = 1;
        while(iSel[iCnt-1] > -1)
        {   
			int grid_row = iSel[iCnt-1];
			
			if(IsWorkingOrder(grid_row) == 0)
			{
				iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
				iCnt++; 
				continue;
			}
			
			String nor_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORPLNCNT);	
			String nor_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_NORACTCNT);	
			String osd_plncnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDPLNCNT);	
			String osd_actcnt = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_OSNDACTCNT);	

			int inorplncnt = StrToIntDef(nor_plncnt, 0);
			int inoractcnt = StrToIntDef(nor_actcnt, 0);
			int iosdplncnt = StrToIntDef(osd_plncnt, 0);
			int iosdactcnt = StrToIntDef(osd_actcnt, 0);
			
			if(inorplncnt + iosdplncnt == inoractcnt + iosdactcnt) //finish order
			{
				iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
				iCnt++; 
				continue;
			}
			
			String soid  = frmScreen1.dhGrid1.GetCellData(grid_row, COLUMN_SO_ID);	
            sSO_ID[iCnt] = soid;
			ss += Format("%d:%d,%s ", [iCnt-1, iSel[iCnt-1], soid]);
            soid_sql += Format("%s, ", [soid]);
			
            iSel[iCnt] = frmScreen1.dhGrid1.GetNextSelectedRow(iSel[iCnt-1]);
            iCnt++;   
			iDownNum++;
        }           
            
        SetDebug(Format("Selected Row : %s and Finish order", [ss]));
		String sqlsoid = Copy(soid_sql, 1, Length(soid_sql)-2);
		insertFinishSQL(sqlsoid);
		
		ShowMessage(Format("Completed finish order : %s", [sqlsoid]));
		
		for(int i=0; i<iDownNum; i++)
		{
			//frmScreen1.dhGrid1.SetCelldataColor2();
		}
    }*/
}

//==================================
{
}