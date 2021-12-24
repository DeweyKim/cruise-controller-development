#ifndef __INTERFACE_H__
#define __INTERFACE_H__

typedef signed char    Bool;
typedef signed char    Int8;
typedef unsigned char  UInt8;
typedef signed short   Int16;
typedef unsigned short UInt16;
typedef signed long    Int32;
typedef unsigned long  UInt32;
typedef float          Float32;
typedef double         Double;

#define FALSE 0
#define TRUE  1

#define MIN_INT8   0x80
#define MAX_INT8   0x7F

#define MIN_UINT8  0x00
#define MAX_UINT8  0xFF

#define MIN_INT16  0x8000
#define MAX_INT16  0x7FFF

#define MIN_UINT16 0x0000
#define MAX_UINT16 0xFFFF

#define MIN_INT32  0x80000000
#define MAX_INT32  0x7FFFFFFF

#define MIN_UINT32 0x00000000
#define MAX_UINT32 0xFFFFFFFF

typedef enum {
	InputKey_Idle = 0,
	InputKey_Cruise,
	InputKey_SetAccel_Short,
	InputKey_SetAccel_Long,
	InputKey_ResDecel_Short,
	InputKey_ResDecel_Long,
	InputKey_Cancel,
	InputKey_Max,
	InputKey_Brake,
	InputKey_Resume,
	InputKey_Exit
};

#define defInterval_ShortKey  5
#define defInterval_LongKey  30

typedef enum {
	CruiseMode_Off = 0,
	CruiseMode_On,
	CruiseMode_Max
} eCruiseMode;

#define defAlarmFlag_invalid      0b00000000
#define defAlarmFlag_vehReady     0b00000001
#define defAlarmFlag_IgOn         0b00000010
#define defAlarmFlag_canTimeout   0b00000100
#define defAlarmFlag_canFault     0b00001000
#define defAlarmFlag_vehSpeed     0b00010000

#define defTargetSpeed_Short 1
#define defTargetSpeed_Long  5

/* Input signals: buttons */
extern Bool g_in_keyCruise_bool;
extern Bool g_in_keySetAccel_bool;
extern Bool g_in_keyResDecel_bool;
extern Bool g_in_keyCancel_bool;

/* Input signals: others */
extern Bool  g_in_CANTimeout_bool;
extern Bool  g_in_CANFault_bool;
extern Bool  g_in_vehReady_bool;
extern Bool  g_in_IgOn_bool;
extern Int16 g_in_vehSpeed_s16;
extern Int16 g_in_brakePedal_s16;

/* Output signals */
extern UInt8 g_out_cruiseMode_u8;
extern Int16 g_out_tgtSpeed_s16;
extern UInt8 g_out_alarmFlag_u8;

typedef struct stCounter_Tag {
	UInt16 count_u16;
} stCounter, *pstCounter;


extern UInt16 Counter(Bool enable, stCounter* pst);

typedef struct stDetect_Tag {
	Bool delay_bool;
} stDetect, * pstDetect;

extern Bool RisingDetect(Bool input, stDetect* pst);
extern Bool FallingDetect(Bool input, stDetect* pst);

typedef struct stSRFlipFlop_Tag {
	Bool delay_bool;
} stSRFlipFlop, * pstSRFlipFlop;

extern Bool SR_FlipFlop(Bool s, Bool r, stSRFlipFlop* pst);

#endif
