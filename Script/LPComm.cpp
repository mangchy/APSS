#language C++Script

#include "GlobalScripts.pas"

//=============================================================
void OnDownloadResult(Variant aResult, Variant aRxValues)
{
	//SetDebug("")
	//ShowMessage(Format("%d", [aResult]));
	//TimerDownload.Enabled = true;
}

//=============================================================
//station : start 0, direction : left=0, right=1
void LP_SendData(int LPTag, int station, int direction, string moldcd, string mcs, string shift, int tar, int count, int osdtar, int osdcount, Variant so_id )
{
	station = station%4;

	Variant tx  = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 100 + (60 * station + 30*direction);
    int wmode   = 2;
	int offset  = 0;
	
	int idx = 0;
		
	for(int i=0; i<120; i++)
	{
		tx[i] = 0;
	}
	
	try
	{		
		//SetDebug();
		for(i=0; i<Length(moldcd); i++)
		{
			if(i%2 > 0) idx = i - 1;
			else        idx = i + 1;
			tx[idx] = Ord(moldcd[i+1]);
		}
		offset = 14;
		//ShowMessage(Format("%d, %s", [Length(mcs), mcs]));
		for(i=0; i<Length(mcs); i++)
		{
			if(i%2 > 0) idx = i - 1;
			else        idx = i + 1;
			tx[offset + idx] = Ord(mcs[i+1]);		
			//ShowMessage(Format("%d, %s, %d, %d, %d, %d, %x", [Length(mcs), mcs, i, i%2, offset + idx, idx, tx[offset + idx]]));
		}
		offset = 34;
		
		tx[offset] = 0x00;
		offset++;
		tx[offset] = 0x00;//Ord(shift[1]);
		offset++;
		
		tx[offset] = ( tar >> 8) && 0xff;
		offset++;
		tx[offset] = ( tar ) && 0xff;
		offset++;
		
		tx[offset] = ( count >> 8) && 0xff;
		offset++;
		tx[offset] = ( count ) && 0xff;
		offset++;
		
		tx[offset] = ( osdtar >> 8) && 0xff;
		offset++;
		tx[offset] = ( osdtar ) && 0xff;
		offset++;
		
		tx[offset] = ( osdcount >> 8) && 0xff;
		offset++;
		tx[offset] = ( osdcount ) && 0xff;
		offset++;		
		
		//so_id size=4, integer
		for(i=0; i<4; i++)
		{		
			int divid = (8*(3-i));//(4-i));
			tx[offset] = ( so_id >> (divid + 8)) && 0xff;
			offset++;
			
			tx[offset] = ( so_id >> (divid)) && 0xff;
			offset++;
		}
		
		_TRACE(ICO_ERROR, Format("write 1 : %d, %d, %d", [offset, LPTag, wmode]));
			//ShowMessage(offset);
		SetModBusTagValueWithResult(LPTag, 1, mb_addr, wmode, offset, tx, nil, 2000);
		_TRACE(ICO_ERROR, Format("write 2 : %d", [offset]));
		//ShowMessage("write");
	}
	except
	{
		SetDebug("LPComm Error " + ExceptionMessage, clRed);
	}
}

//=============================================================
void LP_SetTime(int LPTag, int aHour, int aMin)
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 800;
    int wmode = 2;
	int offset = 0;
	
	tx[offset] = ( aHour >> 8) && 0xff;
	offset++;
	tx[offset] = ( aHour ) && 0xff;
	offset++;
	
	tx[offset] = ( aMin >> 8) && 0xff;
	offset++;
	tx[offset] = ( aMin ) && 0xff;
	offset++;
	
    SetModBusTagValueWithResult(LPTag, 1, mb_addr, wmode, offset, tx, nil, 2000);
}


/* void LP_SetMoldChingSatusON(int LPTag,int orders)
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 810 + orders;
    int wmode = 2;
	int offset = 0;
	
	tx[offset] = ( 1 >> 8) && 0xff;
	offset++;
	tx[offset] = ( 1 ) && 0xff;
	offset++;
	
    SetModBusTagValueWithResult(LPTag, 1, mb_addr, wmode, offset, tx, nil, 2000);
} */

//=============================================================
void LP_ReadWork()
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 300;
    int wmode = 2;
    int xsize = 8;	
	
    SetModBusTagValueWithResult(WI1, 1, mb_addr, wmode, xsize, tx, nil, 2000);
}

//=============================================================
void LP_ShiftData(int LPTag, int aShift)
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 820;
    int wmode = 2;
    int xsize = 2;
	
	tx[0] = (aShift >> 8) && 0xff;
	tx[1] = (aShift) && 0xff;
	SetModBusTagValueWithResult(LPTag, 1, mb_addr, wmode, xsize, tx, nil, 2000);//tag13
}

//=============================================================
void LP_Shift(int tag, string aData)
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 310;
    int wmode = 2;
    int xsize = 2;
	
	int iDecode = Ord(aData);
	tx[0] = (iDecode >> 8) && 0xff;
	tx[1] = (iDecode) && 0xff;
	SetModBusTagValueWithResult(tag, 1, mb_addr, wmode, xsize, tx, nil, 2000);//tag13
}

//=============================================================
//aStation : 0 ~ 7
//aData : 0-OFF, 1-ON
void LP_SetAutoManual(int aStation, int aData, String aDataDescript)
{
	Variant tx = VarArrayCreate([0, 128], varInteger);
	int mb_addr = 420 + aStation;
    int wmode = 2;
    int xsize = 2;
	
	tx[0] = (aData >> 8) && 0xff;
	tx[1] = (aData) && 0xff;
	SetModBusTagValueWithResult(WI1, 1, mb_addr, wmode, xsize, tx, nil, 2000);//tag13
	
	SetDebug(Format("#%d station, Door : set Auto/Manaul, %d=%s", [aStation+1, aData, aDataDescript]));//gDoorAMDescript[aData]]));	
}

//=============================================================
{
	
}