#language PascalScript

uses 'GlobalScripts.pas';
//function SetModBusTagValueWithResult(const tag, unit_adddr, mb_addr, wmode, size: integer; value, a_Result : Variant; wait_time : integer) : boolean

//typedef enum
//{
//	mwNotUse	= 0,
//	mwColi		= 1,
//	mwReg		= 2,
//	mwBroadCoil = 3,
//	mwBroadReg  = 4
//}TMbWrite;

procedure OnResult(result : integer; aRxValues : Variant);
begin
	_TRACE(ICO_ERROR, Format('mb result : %d', [result]));
end;

//==================================
procedure UserScript6;
var
	v : array[0..5] of integer;
	unit_addr, mb_addr, wmode, size, wait_time : integer;
begin
    unit_addr := 2;
    mb_addr := 1;
    wmode := 2;
    size := 4;
    wait_time := 1000;
    v := [1, 2, 3, 4, 5, 6];
    SetModBusTagValueWithResult(Tag2, unit_addr, mb_addr, wmode, size, v, @OnResult, wait_time);
end;

//==================================
begin
end.