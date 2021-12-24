#include "interface.h"


/* Input signals: buttons */
Bool g_in_keyCruise_bool   = 0;
Bool g_in_keySetAccel_bool = 0;
Bool g_in_keyResDecel_bool = 0;
Bool g_in_keyCancel_bool   = 0;

/* Input signals: others */
Bool  g_in_CANTimeout_bool = 0;
Bool  g_in_CANFault_bool   = 0;
Bool  g_in_vehReady_bool   = 0;
Bool  g_in_IgOn_bool       = 0;
Int16 g_in_vehSpeed_s16    = -1;
Int16 g_in_brakePedal_s16  = -1;

/* Output signals */
UInt8 g_out_cruiseMode_u8  = CruiseMode_Off;
Int16 g_out_tgtSpeed_s16   = -1;
UInt8 g_out_alarmFlag_u8   = 0;

UInt16 Counter(Bool enable, stCounter* pst)
{
	UInt32 temp_u32 = (UInt32)pst->count_u16;

	if (enable != FALSE) {
		temp_u32++;
	}
	else {
		temp_u32 = 0;
	}

	if (MAX_UINT16 < temp_u32) {
		pst->count_u16 = MAX_UINT16;
	}
	else {
		pst->count_u16 = (UInt16)temp_u32;
	}

	return pst->count_u16;
}

Bool RisingDetect(Bool input, stDetect* pst)
{
	Bool result_bool = FALSE;

	if ((input != FALSE) && (pst->delay_bool == FALSE)) {
		result_bool = TRUE;
	}
	else {
		result_bool = FALSE;
	}

	pst->delay_bool = input;

	return result_bool;
}

Bool FallingDetect(Bool input, stDetect* pst)
{
	Bool result_bool = FALSE;

	if ((input == FALSE) && (pst->delay_bool != FALSE)) {
		result_bool = TRUE;
	}
	else {
		result_bool = FALSE;
	}

	pst->delay_bool = input;

	return result_bool;
}

Bool SR_FlipFlop(Bool s, Bool r, stSRFlipFlop* pst)
{
	Bool result_bool = FALSE;

	if (r != FALSE) {
		result_bool = FALSE;
	}
	else if ((s != FALSE) || (pst->delay_bool != FALSE)) {
		result_bool = TRUE;
	}
	else {
		result_bool = pst->delay_bool;
	}

	pst->delay_bool = result_bool;

	return result_bool;
}