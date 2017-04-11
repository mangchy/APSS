#language C++Script

#include "GlobalScripts.pas", "GlobalJJ.cpp"
//==================================
void UpdateVersionID()
{
    try
    {
        string namedb = "readSQL";          
        boolean opend = DBConnect(namedb, gSvr_type, gSvr, gPort, gUid, gPwd, gDbname);
               
// SELECT TOP 4 VERSION_ID 
// FROM DATA_SO
// WHERE VERSION_ID >= 'IPI-' + CONVERT(CHAR(6), DATEADD(DAY, -20, GETDATE()),12) + '-M-001'
// GROUP BY VERSION_ID
// ORDER BY VERSION_ID DESC
 
        string sqlquery = "";
                sqlquery += "SELECT TOP 4 VERSION_ID  ";
                sqlquery += "FROM DATA_SO ";
                sqlquery += "WHERE VERSION_ID >= 'IPI-' + CONVERT(CHAR(6), DATEADD(DAY, -20, GETDATE()),12) + '-M-001' " ;
                sqlquery += "GROUP BY VERSION_ID ";
                sqlquery += "ORDER BY VERSION_ID DESC" ;
        DBScriptClear(namedb);
		DBScriptSQL(namedb, sqlquery);
		DBScriptExecute(namedb);
		int cnt = DBRecordCount(namedb); 
        //ShowMessage(IntToStr(cnt)); 
        frmScreen1.cbxVersionID.Items.Clear();
        for(int i=0; i<cnt; i++)
        {
            frmScreen1.cbxVersionID.Items.Add(DBFieldByName_String(namedb, "VERSION_ID"));
            DBNext(namedb);
        }
        
    	DBDisconnect(namedb, true);
        
        frmScreen1.cbxVersionID.ItemIndex = 0; 
	}
    except
    {
         SetDebug(ExceptionMessage, clRed);
    }
}

//==================================
{
}