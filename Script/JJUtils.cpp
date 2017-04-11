#language C++Script

#include "GlobalScripts.pas"
//==================================


String DecodeSec(String aSecond)
{
	//string aSecond = "2017-02-09 06:30:00.000";
	String sH = Copy(aSecond, 12, 2);
	String sM = Copy(aSecond, 15, 2);
	String sS = Copy(aSecond, 18, 2);

	int dbTimeMin = StrToInt(sH)*60 + StrToInt(sM);
	int dbTimeSec = dbTimeMin*60 + StrToInt(sS);

	string aCurrDateTime = FormatDateTime("YYYY-MM-DD HH:NN:SS" + ".000", Now());
//	string aCurrDateTime = "2017-02-09 06:50:00.000";
	String scH = Copy(aCurrDateTime, 12, 2);
	String scM = Copy(aCurrDateTime, 15, 2);
	String scS = Copy(aCurrDateTime, 18, 2);
	int TimeMinCurrent = StrToInt(scH)*60 + StrToInt(scM);
	int TimeSecCurrent = TimeMinCurrent*60 + StrToInt(scS);
	int TotalSec = TimeSecCurrent-dbTimeSec;
	
	return IntToStr(TotalSec);
	//ShowMessage(TotalSec);
}


//aDateTime : 16 08:20:20
String DecodeShift(String aDateTime)
{
	String sH = Copy(aDateTime, 4, 2);
	String sM = Copy(aDateTime, 7, 2);
	
	int dbTimeMin = StrToIntDef(sH, 0)*60 + StrToIntDef(sM, 0);
	sH = FormatDateTime("HH", Now());
	sM = FormatDateTime("NN", Now());
	
	//int TimeMinCurrent = StrToIntDef(sH, 0)*60 + StrToIntDef(sM, 0);
	
	if((dbTimeMin >= 390) && (dbTimeMin < 870))
	{
		return "A";
	}
	else if((dbTimeMin >= 870) && (dbTimeMin < 1350))
	{
		return "B";
	}
	 return "C";
	//ShowMessage(sH + ", " + sM);
}

int DecodeShift2()
{
	String sH = FormatDateTime("HH", Now());
	String sM = FormatDateTime("NN", Now());
	int dbTimeMin = StrToIntDef(sH, 0)*60 + StrToIntDef(sM, 0);
	
	if((dbTimeMin >= 390) && (dbTimeMin < 870))
	{
		return 3;//"A";
	}
	else if((dbTimeMin >= 870) && (dbTimeMin < 1350))
	{
		return 1;//"B";
	}
	 return 2;//"C";
	//ShowMessage(sH + ", " + sM);
}

//aShift : 0, 1, 2        A, B, C
//aDate : 20160522
//return 2016-05-22 06:30:00.000
String EncodeShift_Start(String aDate, int aShift)
{
	String sY = Copy(aDate, 0, 4);
	String sM = Copy(aDate, 5, 2);
	String sD = Copy(aDate, 7, 2);
	
	if(aShift == 0)
	{
		return Format("%s-%s-%s 06:30:00.000", [sY, sM, sD]);
	}
	else if(aShift == 1)
	{
		return Format("%s-%s-%s 14:30:00.000", [sY, sM, sD]);
	}
	else if(aShift == 2)
	{
		return Format("%s-%s-%s 22:30:00.000", [sY, sM, sD]);
	}
	return "";
}

String EncodeShift_End(String aDate, int aShift)		
{
	String sY = Copy(aDate, 0, 4);
	String sM = Copy(aDate, 5, 2);
	String sD = Copy(aDate, 7, 2);
	
	if(aShift == 0)
	{
		return Format("%s-%s-%s 14:30:00.000", [sY, sM, sD]);
	}
	else if(aShift == 1)
	{
		return Format("%s-%s-%s 22:30:00.000", [sY, sM, sD]);
	}
	else if(aShift == 2)
	{
		int nextday = StrToIntDef(sD, 0) + 1;
		return Format("%s-%s-%.02d 06:30:00.000", [sY, sM, nextday]);
	}
	return "";
}

{
	
}