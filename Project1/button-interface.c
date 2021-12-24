#include "button-interface.h"

static stDetect g_stDetect_KeyCruise = { 0 };
static stCounter g_stCounter_KeyCruise = { 0 };

static stDetect g_stDetect_KeyCancel = { 0 };
static stCounter g_stCounter_KeyCancel = { 0 };

static stDetect g_stDetect_KeySetAccel = { 0 };
static stSRFlipFlop g_stSRFlipFlop_KeySetAccel = { 0 };
static stCounter g_stCounter_KeySetAccel = { 0 };

static stCounter g_stCounter_KeyResDecel = { 0 };
static stSRFlipFlop g_stSRFlipFlop_KeyResDecel = { 0 };
static stDetect g_stDetect_KeyResDecel = { 0 };

static UInt8 g_out_lastInputKey_u8 = InputKey_Idle;

UInt8 g_out_inputKey_u8 = InputKey_Idle;

UInt8 GetCurrentInputKey()
{
	UInt8 result_u8 = InputKey_Idle;

	Bool keyCruise_fdet_bool = FALSE;
	Bool keyCancel_fdet_bool = FALSE;
	Bool keySetAccel_fdet_bool = FALSE;
	Bool keyResDecel_fdet_bool = FALSE;

	Bool keySetAccel_srff_bool = FALSE;
	Bool keyResDecel_srff_bool = FALSE;

	keyCruise_fdet_bool   = FallingDetect(g_in_keyCruise_bool, &g_stDetect_KeyCruise);
	keyCancel_fdet_bool   = FallingDetect(g_in_keyCancel_bool, &g_stDetect_KeyCancel);
	keySetAccel_fdet_bool = RisingDetect(g_in_keySetAccel_bool, &g_stDetect_KeySetAccel);
	keyResDecel_fdet_bool = FallingDetect(g_in_keyResDecel_bool, &g_stDetect_KeyResDecel);

	keySetAccel_srff_bool = SR_FlipFlop(g_in_keySetAccel_bool, keySetAccel_fdet_bool, &g_stSRFlipFlop_KeySetAccel);
	keyResDecel_srff_bool = SR_FlipFlop(g_in_keyResDecel_bool, keyResDecel_fdet_bool, &g_stSRFlipFlop_KeyResDecel);

	if (keyCruise_fdet_bool != FALSE) {
		result_u8 = InputKey_Cruise;
	}
	else if (keyCancel_fdet_bool != FALSE) {
		result_u8 = InputKey_Cancel;
	}
	else {

		if (result_u8 == InputKey_Idle) {

			if (keySetAccel_fdet_bool != FALSE) {

				if ((g_out_lastInputKey_u8 != InputKey_SetAccel_Long) &&
					(defInterval_ShortKey < g_stCounter_KeySetAccel.count_u16) &&
					(g_stCounter_KeySetAccel.count_u16 < defInterval_LongKey)) {
					result_u8 = InputKey_SetAccel_Short;
				}
				else {
					g_out_lastInputKey_u8 = result_u8;
					g_stCounter_KeySetAccel.count_u16 = 0;
					g_stSRFlipFlop_KeySetAccel.delay_bool = FALSE;
				}
			}
			else {
				Counter(keySetAccel_srff_bool, &g_stCounter_KeySetAccel);
			}

			if (defInterval_LongKey <= g_stCounter_KeySetAccel.count_u16) {
				result_u8 = InputKey_SetAccel_Long;
			}
		}

		if (result_u8 == InputKey_Idle) {

			if (keyResDecel_fdet_bool != FALSE) {

				if ((g_out_lastInputKey_u8 != InputKey_SetAccel_Long) &&
					(defInterval_ShortKey < g_stCounter_KeyResDecel.count_u16) &&
					(g_stCounter_KeyResDecel.count_u16 < defInterval_LongKey)) {
					result_u8 = InputKey_SetAccel_Short;
				}
				else {
					g_out_lastInputKey_u8 = result_u8;
					g_stCounter_KeyResDecel.count_u16 = 0;
					g_stSRFlipFlop_KeyResDecel.delay_bool = FALSE;
				}
			}
			else {
				Counter(keyResDecel_srff_bool, &g_stCounter_KeyResDecel);
			}

			if (defInterval_LongKey <= g_stCounter_KeyResDecel.count_u16) {
				result_u8 = InputKey_SetAccel_Long;
			}
		}
	}


	if (result_u8 != InputKey_Idle) {

		g_out_lastInputKey_u8 = result_u8;

		g_stDetect_KeyCruise.delay_bool = FALSE;
		g_stDetect_KeyCancel.delay_bool = FALSE;

		g_stCounter_KeySetAccel.count_u16 = 0;
		g_stSRFlipFlop_KeySetAccel.delay_bool = FALSE;

		g_stCounter_KeyResDecel.count_u16 = 0;
		g_stSRFlipFlop_KeyResDecel.delay_bool = FALSE;
	}

	return result_u8;
}