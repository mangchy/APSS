#language C++Script

#include "GlobalScripts.pas"
//==================================
void TestDB()
{
string namedb = "atSQL";
string svr_type = "SQL SERVER";
string svr = "(local)";
string port = "1433";
string uid = "sa";
string pwd = "system" ;
string dbname = "IP_SS"; 

    boolean opend;
    opend = DBConnect(namedb, svr_type, svr, port, uid, pwd, dbname); 
    if (opend) ShowMessage("DB Connected : OK");
    else       ShowMessage("DB Connected : Fail");
    DBDisconnect(namedb, true);
}
   
//==================================
{
}