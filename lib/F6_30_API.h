/*++

Module Name:

    F6_30_API.H

Abstract:

    Master include file for applications that call 
    functions provided by F6_30_API.DLL

Revision History:

    2014-11-04 : created

--*/

#ifndef _F6_30_API_H
#define _F6_30_API_H


#include "F6_30_ERRS.h"


typedef long READERHANDLE;
typedef READERHANDLE *PREADERHANDLE;


#ifndef __in
#define __in
#endif
#ifndef __out
#define __out
#endif
#ifndef __inout
#define __inout
#endif


#ifdef __cplusplus
extern "C" {
#endif


LONG	
WINAPI 
F6_Connect(
	__in	DWORD	dwPortNumber,
	__in	DWORD	dwBaudRate,
	__out	PREADERHANDLE phReader
	);
LONG	
WINAPI 
F6_Disconnect(
	__in	READERHANDLE hReader
	);
LONG	
WINAPI
F6_Cancel(
	__in	READERHANDLE hReader
	);
LONG	
WINAPI 
F6_ExecCommand(
	__in	READERHANDLE hReader,
	__in	BYTE	bCode1,
	__in	BYTE	bCode2,
	__in	PBYTE	pbCmdData,
	__in	USHORT	usCmdLen,
	__out	PBYTE	pbRespData,
	__inout	PDWORD	pdwRespLen
	);

#define RESET_NOACTION	0x30
#define RESET_EJECT	0x31
#define RESET_RETRACT	0x32
#define RESET_RETAIN	0x33
LONG 
WINAPI 
F6_Reset(
	__in	READERHANDLE hReader,
	__in	BYTE	bAction,
	__in	PSTR	pszVerBuff,
	__inout	PDWORD	pcbVerLength
	);

#define CRS_CARDINGATEPOS	0x30
#define CRS_CARDINFRONT		0x31
#define CRS_CARDINRFPOS		0x32
#define CRS_CARDINICPOS		0x33
#define CRS_CARDINREAR		0x34
#define CRS_NOCARDIN		0x35
#define CRS_NOTINSTDPOS		0x36
#define CRS_CARDINREREADPOS	0x37
LONG 
WINAPI 
F6_GetStatus(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbStatus
	);

LONG 
WINAPI 
F6_GetSenserDetail(
	__in	READERHANDLE hReader,
	__out	BYTE	(&bDetail)[9]
	);
LONG
WINAPI
F6_GetSenserLevel(
	__in	READERHANDLE hReader,
	__out	float	(&fVoltage)[9]
	);

#define ENTRY_FROMFRONT 0x30
#define ENTRY_FROMREAR 0x32
LONG
WINAPI
F6_Entry(
	__in READERHANDLE hReader,
	__in BYTE bMethod
	);

LONG 
WINAPI 
F6_PermitInsertion(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_DenieInsertion(
	__in	READERHANDLE hReader
	);

#define MOVE_TORFPOS	0x30
#define MOVE_TOICPOS	0x31
#define MOVE_TOFRONT	0x32
#define MOVE_TOREAR	0x33
#define MOVE_TOGATEPOS	0x34
#define MOVE_RETRACT	0x35
#define MOVE_TOREREADPOS 0x36
LONG 
WINAPI 
F6_MoveCard(
	__in	READERHANDLE hReader,
	__in	BYTE	bMode
	);
LONG 
WINAPI
F6_SetBaudRate(
	__in	READERHANDLE hReader,
	__in	DWORD	dwBaudRate
	);

#define LED_OFF		0x30
#define LED_LIGHTEN	0x31
#define LED_BLINK	0x32
LONG 
WINAPI 
F6_LedControl(
	__in	READERHANDLE hReader,
	__in	BYTE	bAction
	);

//
// Mag.card operations
//

#define TRACKST_NORMAL		0x60
#define TRACKST_SS_ERROR	0x61
#define TRACKST_ES_ERROR	0x62
#define TRACKST_PARITY_ERROR	0x63
#define TRACKST_LRC_ERROR	0x64
#define TRACKST_NO_DATA		0x65

typedef struct _TRACKINFO
{
	char	Contents[3][256];
	int	Lengths[3];
	BYTE	Status[3];
} TRACKINFO, *PTRACKINFO;


#define TRACK_1		0x10
#define TRACK_2		0x20
#define TRACK_3		0x40

#define READ_ASCII	0x37
#define READ_BINARY	0x38

LONG 
WINAPI 
F6_ReadTracks(
	__in	READERHANDLE hReader,
	__in	BYTE	bMode,
	__in	BYTE	bSource,
	__out	PTRACKINFO pTrackInfo
	);

// IC Card Operations
//

#define ICCTYPE_AT24C01		0x30
#define ICCTYPE_AT24C02		0x31
#define ICCTYPE_AT24C04		0x32
#define ICCTYPE_AT24C08		0x33
#define ICCTYPE_AT24C16		0x34
#define ICCTYPE_AT24C32		0x35
#define ICCTYPE_AT24C64		0x36
#define ICCTYPE_AT45DB041	0x37
#define ICCTYPE_AT88SC102	0x38
#define ICCTYPE_AT88SC1604	0x39
#define ICCTYPE_AT88SC1608	0x3A
#define ICCTYPE_SLE4442		0x3B
#define ICCTYPE_SLE4428		0x3C
#define ICCTYPE_T0_CPU		0x3D
#define ICCTYPE_T1_CPU		0x3E
#define ICCTYPE_UNKNOWN		0xFF

LONG 
WINAPI 
F6_IccPowerOn(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_IccPowerOff(
	__in	READERHANDLE hReader
	);
LONG
WINAPI 
F6_DetectICCType(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbCardType
	);

#define RFCTYPE_MIFARE_UL	0x30
#define RFCTYPE_MIFARE_S50	0x31
#define RFCTYPE_MIFARE_S70	0x32
#define RFCTYPE_TYPEA_CPU	0x33
#define RFCTYPE_TYPEB_CPU	0x42
#define RFCTYPE_UNKNOWN		0xFF
LONG
WINAPI
F6_DetectRFCardType(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbCardType
	);

#define ICC_PROTOCOL_T0	0x30
#define ICC_PROTOCOL_T1	0x31

#define VOLTAGE_1_8	0x30
#define VOLTAGE_3	0x31
#define VOLTAGE_5	0x32

// CPU Operations
//

LONG
WINAPI 
F6_CpuActivate(
	__in	READERHANDLE hReader,
	__in	BYTE	bVoltage,
	__out	PBYTE	pbProtocol,
	__out	PBYTE	pbATRBuff,
	__inout	PDWORD	pcbATRLength
	);
LONG 
WINAPI 
F6_CpuDeactivate(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_CpuTransmit(
	__in	READERHANDLE hReader,
	__in	BYTE	bProtocol,
	__in	PBYTE	pbSendBuff,
	__in	USHORT	cbSendLength,
	__out	PBYTE	pbRecvBuff,
	__inout	PDWORD	pcbRecvLength
	);

// SAM Operations
//

LONG 
WINAPI
F6_SamActivate(
	__in	READERHANDLE hReader,
	__in	BYTE	bVoltage,
	__out	PBYTE	pbProtocol,
	__out	PBYTE	pbATRBuff,
	__inout	PDWORD	pcbATRLength
	);
LONG 
WINAPI 
F6_SamDeactivate(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_SamTransmit(
	__in	READERHANDLE hReader,
	__in	BYTE	bProtocol,
	__in	PBYTE	pbSendBuff,
	__in	USHORT	cbSendLength,
	__out	PBYTE	pbRecvBuff,
	__inout	PDWORD	pcbRecvLength
	);
LONG 
WINAPI
F6_SamSelect(
	__in	READERHANDLE hReader,
	__in	BYTE	bSAMNumber	// 1, 2, 3 ... 
	);

// I2C Operations
//

#define I2CTYPE_24C01		0x30
#define I2CTYPE_24C02		0x31
#define I2CTYPE_24C04		0x32
#define I2CTYPE_24C08		0x33
#define I2CTYPE_24C16		0x34
#define I2CTYPE_24C32		0x35
#define I2CTYPE_24C64		0x36

LONG 
WINAPI 
F6_I2cRead(
	__in	READERHANDLE hReader,
	__in	BYTE	bCardType,
	__in	WORD	wStartAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer,
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_I2cWrite(
	__in	READERHANDLE hReader,
	__in	BYTE	bCardType,
	__in	WORD	wStartAddress,
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);

// AT45DB041 Operations

LONG 
WINAPI 
F6_Db041Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Db041ReadPage(
	__in	READERHANDLE hReader,
	__in	WORD	wPageAddress,
	__out	BYTE	(&bPageBuffer)[264]
	);
LONG 
WINAPI 
F6_Db041WritePage(
	__in	READERHANDLE hReader,
	__in	WORD	wPageAddress,
	__in	BYTE	(&bPageBuffer)[264]
	);


#define PERSONALIZATION_TEST	0x30
#define PERSONALIZATION_LOGOUT	0x31
#define PERSONALIZATION_REAL	0x32


// AT88SC102 Operations

LONG
WINAPI 
F6_Sc102Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sc102VerifySC(
	__in	READERHANDLE hReader,
	__in	BYTE	bSCByte1,
	__in	BYTE	bSCByte2
	);
LONG 
WINAPI 
F6_Sc102UpdateSC(
	__in	READERHANDLE hReader,
	__in	BYTE	bSCByte1,
	__in	BYTE	bSCByte2
	);
LONG 
WINAPI 
F6_Sc102ReadMemory(
	__in	READERHANDLE hReader,
	__in	BYTE	bStartAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer,
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sc102WriteMemory(
	__in	READERHANDLE hReader, 
	__in	BYTE	bStartAddress, 
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);
LONG 
WINAPI 
F6_Sc102EraseMemory(
	__in	READERHANDLE hReader, 
	__in	BYTE	bStartAddress, 
	__in	BYTE	bBytesToErase
	);
LONG 
WINAPI 
F6_Sc102EraseAZ1(
	__in	READERHANDLE hReader,
	__in	BYTE	(&bKeyBytes)[6]
	);
LONG 
WINAPI 
F6_Sc102EraseAZ2(
	__in	READERHANDLE hReader,
	__in	BOOL	fEC2Enabled,
	__in	BYTE	(&bKeyBytes)[4]
	);
LONG 
WINAPI 
F6_Sc102UpdateEZ1(
	__in	READERHANDLE hReader,
	__in	BYTE	(&bKeyBytes)[6]
	);
LONG 
WINAPI 
F6_Sc102UpdateEZ2(
	__in	READERHANDLE hReader,
	__in	BYTE	(&bKeyBytes)[4]
	);
LONG 
WINAPI 
F6_Sc102Personalize(
	__in	READERHANDLE hReader,
	__in	BYTE	bMode
	);

// AT88SC1604 Operations

#define PWDTYPE_SC		0x30
#define PWDTYPE_SC1		0x31
#define PWDTYPE_EZ1		0x32
#define PWDTYPE_SC2		0x33
#define PWDTYPE_EZ2		0x34
#define PWDTYPE_SC3		0x35
#define PWDTYPE_EZ3		0x36
#define PWDTYPE_SC4		0x37
#define PWDTYPE_EZ4		0x38

LONG 
WINAPI 
F6_Sc1604Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sc1604VerifyPassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bPwdType,
	__in	BYTE	bPwdByte1,
	__in	BYTE	bPwdByte2
	);
LONG 
WINAPI
F6_Sc1604UpdatePassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bPwdType,
	__in	BYTE	bPwdByte1,
	__in	BYTE	bPwdByte2
	);
LONG 
WINAPI 
F6_Sc1604ReadMemory(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer,
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sc1604WriteMemory(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress,
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);
LONG 
WINAPI 
F6_Sc1604EraseMemory(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress,
	__in	BYTE	bBytesToErase
	);
LONG 
WINAPI
F6_Sc1604Personalize(
	__in	READERHANDLE hReader,
	__in	BYTE	bMode
	);

// AT88SC1608 Operations

#define ZONEID_USER0		0x30
#define ZONEID_USER1		0x31
#define ZONEID_USER2		0x32
#define ZONEID_USER3		0x33
#define ZONEID_USER4		0x34
#define ZONEID_USER5		0x35
#define ZONEID_USER6		0x36
#define ZONEID_USER7		0x37
#define ZONEID_CONFIG		0x38

#define RPW_USER0		0x30
#define RPW_USER1		0x31
#define RPW_USER2		0x32
#define RPW_USER3		0x33
#define RPW_USER4		0x34
#define RPW_USER5		0x35
#define RPW_USER6		0x36
#define RPW_USER7		0x37

#define WPW_USER0		0x38
#define WPW_USER1		0x39
#define WPW_USER2		0x3A
#define WPW_USER3		0x3B
#define WPW_USER4		0x3C
#define WPW_USER5		0x3D
#define WPW_USER6		0x3E
#define WPW_USER7		0x3F

LONG 
WINAPI 
F6_Sc1608Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sc1608VerifyPassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bPwdType,
	__in	BYTE	(&bPwdBytes)[3]
	);
LONG 
WINAPI
F6_Sc1608UpdatePassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bPwdType,
	__in	BYTE	(&bPwdBytes)[3]
	);
LONG 
WINAPI 
F6_Sc1608ReadZone(
	__in	READERHANDLE hReader,
	__in	BYTE	bZoneID,
	__in	BYTE	bAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer,
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sc1608WriteZone(
	__in	READERHANDLE hReader,
	__in	BYTE	bZoneID,
	__in	BYTE	bAddress,
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);

#define FUSE_BURNED		0x30
#define FUSE_UNBURNED		0x31
LONG 
WINAPI 
F6_Sc1608ReadFuses(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbFAB,
	__out	PBYTE	pbCMA,
	__out	PBYTE	pbPER
	);
LONG 
WINAPI 
F6_Sc1608WriteFuses(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sc1608InitAuth(
	__in	READERHANDLE hReader,
	__in	BYTE	(&bRandomNumberBytes)[8]
	);
LONG 
WINAPI 
F6_Sc1608VerifyAuth(
	__in	READERHANDLE hReader,
	__in	BYTE	(&bChallengeBytes)[8]
	);


// SLE4428 Operations

LONG 
WINAPI 
F6_Sle4428Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sle4428VerifyPSC(
	__in	READERHANDLE hReader,
	__in	BYTE	bPSCByte1,
	__in	BYTE	bPSCByte2
	);
LONG 
WINAPI 
F6_Sle4428UpdatePSC(
	__in	READERHANDLE hReader,
	__in	BYTE	bPSCByte1,
	__in	BYTE	bPSCByte2
	);
LONG 
WINAPI 
F6_Sle4428ReadWithoutPB(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress, 
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer, 
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sle4428ReadProtectionBits(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress, 
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer, 
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sle4428WriteWithPB(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress, 
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);
LONG 
WINAPI 
F6_Sle4428WriteWithoutPB(
	__in	READERHANDLE hReader,
	__in	WORD	wStartAddress, 
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);


// SLE4442 Operations

LONG 
WINAPI 
F6_Sle4442Reset(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_Sle4442VerifyPSC(
	__in	READERHANDLE hReader,
	__in	BYTE	bPSCByte1,
	__in	BYTE	bPSCByte2,
	__in	BYTE	bPSCByte3
	);
LONG 
WINAPI 
F6_Sle4442UpdatePSC(
	__in	READERHANDLE hReader,
	__in	BYTE	bPSCByte1,
	__in	BYTE	bPSCByte2,
	__in	BYTE	bPSCByte3
	);
LONG 
WINAPI 
F6_Sle4442ReadMainMemory(
	__in	READERHANDLE hReader,
	__in	BYTE	bStartAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer, 
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sle4442WriteMainMemory(
	__in	READERHANDLE hReader,
	__in	BYTE	bStartAddress, 
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);
LONG 
WINAPI 
F6_Sle4442ReadSecurityMemory(
	__in	READERHANDLE hReader,
	__out	BYTE	(&bBuffer)[4]
	);
LONG 
WINAPI 
F6_Sle4442ReadProtectionBits(
	__in	READERHANDLE hReader,
	__in	BYTE	bStartAddress,
	__in	BYTE	bBytesToRead,
	__out	PBYTE	pbBuffer, 
	__inout	PDWORD	pcbLength
	);
LONG 
WINAPI 
F6_Sle4442WriteProtectionMemory(
	__in	READERHANDLE hReader,
	__in	BYTE	bStartAddress,
	__in	BYTE	bBytesToWrite,
	__in	PBYTE	pbBuffer
	);

// S50 Operations


LONG 
WINAPI 
F6_S50Select(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_S50ReadUID(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbUIDBuff,
	__inout	PDWORD	pcbUIDLength
	);
LONG 
WINAPI 
F6_S50VerifyPassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BOOL	bWithKeyA,
	__in	BYTE	(&bKeyBytes)[6]
	);
LONG 
WINAPI 
F6_S50ReadOneBlock(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__out	BYTE	(&bBlockBuff)[16]
	);
LONG 
WINAPI 
F6_S50WriteOneBlock(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	BYTE	(&bBlockBuff)[16]
	);
LONG 
WINAPI 
F6_S50InitializeValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG 
WINAPI 
F6_S50IncrementValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG 
WINAPI 
F6_S50DecrementValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG
WINAPI
F6_S50ReadValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__out	PUINT32	puValue
	);

// S70 Operations

LONG 
WINAPI 
F6_S70Select(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_S70ReadUID(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbUIDBuff,
	__inout	PDWORD	pcbUIDLength
	);
LONG 
WINAPI 
F6_S70VerifyPassword(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BOOL	bWithKeyA,
	__in	BYTE	(&bKeyBytes)[6]
	);
LONG 
WINAPI 
F6_S70ReadOneBlock(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__out	BYTE	(&bBlockBuff)[16]
	);
LONG
WINAPI 
F6_S70WriteOneBlock(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	BYTE	(&bBlockBuff)[16]
	);
LONG 
WINAPI 
F6_S70InitializeValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG 
WINAPI 
F6_S70IncrementValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG 
WINAPI 
F6_S70DecrementValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__in	UINT32	uValue
	);
LONG
WINAPI
F6_S70ReadValue(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	bBlockNumber,
	__out	PUINT32	puValue
	);

//
// Ultralight Operations

LONG 
WINAPI 
F6_UlSelect(
	__in	READERHANDLE hReader
	);
LONG 
WINAPI 
F6_UlReadUID(
	__in	READERHANDLE hReader,
	__out	PBYTE	pbUIDBuff,
	__inout	PDWORD	pcbUIDLength
	);
LONG 
WINAPI 
F6_UlReadSector(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__out	BYTE	(&bBuffer)[4]
	);
LONG 
WINAPI 
F6_UlWriteSector(
	__in	READERHANDLE hReader,
	__in	BYTE	bSectorNumber,
	__in	BYTE	(&bBuffer)[4]
	);
LONG 
WINAPI 
F6_UlHalt(
	__in	READERHANDLE hReader
	);

// TypeA/B CPU Operations

LONG 
WINAPI 
F6_TypeABCpuActivate(
	__in	READERHANDLE hReader,
	__in	LPBYTE	pbATRBuff,
	__inout	PDWORD	pcbATRLength
	);
LONG 
WINAPI 
F6_TypeABCpuTransmit(
	__in	READERHANDLE hReader,
	__in	PBYTE	pbSendBuff,
	__in	USHORT	cbSendLength,
	__in	BOOL	fSendMBit,
	__out	PBYTE	pbRecvBuff,
	__inout	PDWORD	pcbRecvLength,
	__out	PBOOL	pfRecvMBit
	);


LONG
WINAPI
F6_EnterIAP(
	__in	READERHANDLE hReader
	);

#define AGTEST_FRONTEJECT 0
#define AGTEST_REAREJECT 1

LONG
WINAPI
F6_EnableAgingTest(
	__in READERHANDLE hReader,
	__in BYTE bMode,
	__in BYTE bInterval
	);

#ifdef __cplusplus
}
#endif


#endif // _F6SAPI_H