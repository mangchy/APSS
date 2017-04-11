#language C++Script

#include "GlobalScripts.pas", "Global.pas"

int wait_time = 1;//wait rx

//==================================
void SetNextDownloadStep(int aStep)
{
    gDownloadStep = aStep;
    TimerScreenDownload.Enabled = true;
}

//==================================
//ModBus result
void OnResult(int result, Variant aRxValues)
{            
    gDownloadProcessing = false;    
                  
    //if(!result) return;
    
    switch(gDownloadStep) 
    {       
        //case SET_DOWNLOAD_START: SetNextDownloadStep(SET_ASSYDATE_ORDER);
        case SET_ASSYDATE_ORDER: SetNextDownloadStep(SET_EACHROWDATA);
        case SET_EACHROWDATA: 
        {
          _TRACE(ICO_ERROR, Format("mb result : %d, %d, step=%d", [result, gDownloadStep, gDownloadedCount]));
          if(result > 0) gDownloadedCount = gDownloadedCount + 1;
          
          if(gDownloadedCount <= gOrderCount) 
          {
            SetNextDownloadStep(SET_EACHROWDATA);            
          }
          else 
          {
            gDownloadedCount = 0;
            SetNextDownloadStep(SET_TOTALROWCOMPLETED);
          }
        }
    }
}

//==================================
void Download_AssyDateOrder(int unit_addr, int mm, int dd, int ordercnt, int hour, int ml, int planqty)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
                     
    
    //Assy Date  IndexCnt	Hour	M.L	Plan Qty
    //30         32	        33	    34	35

    int mb_addr = 30;
    int wmode = 2;
    int size  = 12;
           
    //assy.date(1) - dd    
    tx[0] = (dd >> 8) && 0xff;
    tx[1] = (dd) && 0xff;
    
    //assy.date(1) - mm
    tx[2] = (mm >> 8) && 0xff;
    tx[3] = (mm && 0xff);  
    
    //ordercnt               
    tx[4] = (ordercnt >> 8) && 0xff;
    tx[5] = (ordercnt && 0xff);
    
    //hour               
    tx[6] = (hour >> 8) && 0xff;
    tx[7] = (hour && 0xff);
    
    //ml               
    tx[8] = (ml >> 8) && 0xff;
    tx[9] = (ml && 0xff);
    
    //planqty               
    tx[10] = (planqty >> 8) && 0xff;
    tx[11] = (planqty && 0xff);
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnResult, wait_time);
    
    gDownloadProcessing = true;
}    

//No.	Size	Order Qty.	Style#	Part#	Layer	C.Die	Tar.CNT	Ramp
//36	37	    38	        39	    42	    44	    45	    46	    47     48  49
//==================================
void Download_EachRowData(
    int unit_addr, int no, int size, int orderqty, 
    int sty1, int sty2, int sty3, int pt1, int pt2,
    int layer, int cdie, int targetcnt, int ramp, int remain)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
    
    int mb_addr = 36 + ((no-1)*14);
    int wmode = 2;
    int xsize = 28;
           
    //index     
    tx[0] = (no >> 8) && 0xff;
    tx[1] = (no) && 0xff;
    tx[2] = (size >> 8) && 0xff;
    tx[3] = (size) && 0xff;
    tx[4] = (orderqty >> 8) && 0xff;
    tx[5] = (orderqty) && 0xff;
       
    tx[6] = (sty1 >> 8) && 0xff;
    tx[7] = (sty1) && 0xff;
    tx[8] = (sty2 >> 8) && 0xff;
    tx[9] = (sty2) && 0xff;
    tx[10] = (sty3 >> 8) && 0xff;
    tx[11] = (sty3) && 0xff;

    tx[12] = (pt1 >> 8) && 0xff;
    tx[13] = (pt1) && 0xff;
    tx[14] = (pt2 >> 8) && 0xff;
    tx[15] = (pt2) && 0xff;

    tx[16] = (layer >> 8) && 0xff;
    tx[17] = (layer) && 0xff;

    tx[18] = (cdie >> 8) && 0xff;
    tx[19] = (cdie) && 0xff;

    tx[20] = (targetcnt >> 8) && 0xff;
    tx[21] = (targetcnt) && 0xff;

    tx[22] = (ramp >> 8) && 0xff;
    tx[23] = (ramp) && 0xff;   

    int lp_area = 0;
    tx[24] = (remain >> 8) && 0xff;
    tx[25] = (remain) && 0xff;
    tx[26] = (lp_area >> 8) && 0xff;
    tx[27] = (lp_area) && 0xff;

    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, xsize, tx, &OnResult, wait_time);
    
    gDownloadProcessing = true;
}  
/*
//==================================
void Download_EachRowData(
    int unit_addr, int no, int size, int orderqty, 
    int sty1, int sty2, int pt1, int pt2,
    int layer, int cdie, int targetcnt, int ramp)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
    
    int mb_addr = 36 + ((no-1)*14);
    int wmode = 2;
    int xsize = 28;
           
    //index     
    tx[0] = (no >> 8) && 0xff;
    tx[1] = (no) && 0xff;
    tx[2] = (size >> 8) && 0xff;
    tx[3] = (size) && 0xff;
    tx[4] = (orderqty >> 8) && 0xff;
    tx[5] = (orderqty) && 0xff;
       
    tx[6] = (sty1 >> 8) && 0xff;
    tx[7] = (sty1) && 0xff;
    tx[8] = (sty1 >> 24) && 0xff;
    tx[9] = (sty1 >> 16) && 0xff;
    tx[10] = (sty2 >> 8) && 0xff;
    tx[11] = (sty2) && 0xff;

    tx[12] = (pt1 >> 8) && 0xff;
    tx[13] = (pt1) && 0xff;
    tx[14] = (pt2 >> 8) && 0xff;
    tx[15] = (pt2) && 0xff;

    tx[16] = (layer >> 8) && 0xff;
    tx[17] = (layer) && 0xff;

    tx[18] = (cdie >> 8) && 0xff;
    tx[19] = (cdie) && 0xff;

    tx[20] = (targetcnt >> 8) && 0xff;
    tx[21] = (targetcnt) && 0xff;

    tx[22] = (ramp >> 8) && 0xff;
    tx[23] = (ramp) && 0xff;   

    int lp_area = 0;
    tx[24] = (lp_area >> 8) && 0xff;
    tx[25] = (lp_area) && 0xff;
    tx[26] = (lp_area >> 8) && 0xff;
    tx[27] = (lp_area) && 0xff;

    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, xsize, tx, &OnResult, wait_time);
    
    gDownloadProcessing = true;
}  
*/

//==================================
void Download_InitTotalCount(int unit_addr)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
                                                
    int mb_addr = 1982;
    int wmode = 2;
    int size  = 14;
           
    tx[0] = 0;
    tx[1] = 0;
    
    tx[2] = 0;
    tx[3] = 0;  
    
    tx[4] = 0;
    tx[5] = 0;
    
    tx[6] = 0;
    tx[7] = 0;
    
    tx[8] = 0;
    tx[9] = 0;
    
    tx[10] = 0;
    tx[11] = 0;
    
    tx[12] = 0;
    tx[13] = 0;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnResult, wait_time);
    
    gDownloadProcessing = true;
}    

//==================================
void Download_TotalRowCompleted(int unit_addr, int time)
{
    Variant tx = VarArrayCreate([0, 128], varInteger);
    
    int mb_addr = 1999;
    int wmode = 2;
    int size  = 4;
           
    //completed download, and next move memory in LP    
    tx[1] = (time) && 0xff;
    tx[0] = (time >> 8) && 0xff;
    tx[2] = 0;
    tx[3] = 0;
    
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, tx, &OnResult, wait_time);
    
    gDownloadProcessing = true;
}  

//==================================
void DownloadToLP()
{          
    _TRACE(ICO_ERROR, Format("dwonload to LP, %d", [gDownloadStep]));       
        
    switch(gDownloadStep)
    {
        case SET_ASSYDATE_ORDER: 
        {
            //Download_AssyDateOrder(
            //int unit_addr, int mm, int dd, int order_cnt, int hour, int ml, int planqty)
            /*
            Download_AssyDateOrder(
				gLP_UnitAddr, 
				gLP_AssyDateMM, 
				gLP_AssyDateDD, 
				gLP_DisplayPage, 
				gLP_WorkHour, 
				gLP_ML, 
				gLP_PlanQty);
            */
            //Download_AssyDateOrder(unit_address, 5, 28, 10);//¿ùÀÏ,Order_Count
        }
        case SET_EACHROWDATA:    
        {
            //Download_EachRowData(
            //int unit_addr, int no, int size, int orderqty, 
            //int sty1, int sty2, int pt1, int pt2,
            //int layer, int cdie, int targetcnt, int ramp)
/*               
            gLP_Style++;   
              
            Extended sty = gLP_Style / 1000000;
			int sty1_1 = Trunc(sty);
			sty = gLP_Style % 1000000;
			sty = (sty) / 1000;
			int sty1_2 = Trunc(sty);
			sty = gLP_Style / 1000;
			int sty1_3 = gLP_Style % 1000;
            
            Download_EachRowData(
				gLP_UnitAddr, 
				gDownloadedCount, 
				gLP_Size, 
				gLP_OrderQty, 
				sty1_1, 
				sty1_2,
                sty1_3, 
				gLP_Part, 
				gLP_Part, 
				gLP_Layer, 
				gLP_CDie, 
				gLP_TargetCnt, 
				gLP_Lamp,
                gLP_Remain);
            Screen1.dhProgress1.UserValue = gDownloadedCount;*/
        }             
        case SET_TOTALROWCOMPLETED:  
        {
            //Download_TotalRowCompleted(gLP_UnitAddr, gLP_WorkHour);
        }
    }          
}

//==================================
{
}