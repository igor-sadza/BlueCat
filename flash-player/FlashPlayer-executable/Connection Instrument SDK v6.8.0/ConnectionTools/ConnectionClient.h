#ifndef CXC_H
#define CXC_H
/*
*********************************************************************

	COPYRIGHT (c) 2008-2013 Thought Technology Ltd.

	Connection Client API.

	About:

		Definitions and import declarations for ConnectionClient.dll.

		The Connection Client API allows a client application to
		received simple data from BioGraph Infiniti(tm) through the
		Connection Instrument SDK dll.

		The usage model is similar to that of a joystick or gamepad
		under DirectInput, but without the COM aspect of that API.
		Only the "immediate mode" has been implemented, since it is
		the mode most commonly used in a game's frame update loop.

	Targets:

		Windows 2000 and higher GUI applications written in C or C++,
		multi-byte or unicode. A type library is generated to allow
		simple and efficient use in Visual Basic 6.0 applications.

		Thought Technology Ltd.
		2180 Belgrave Ave.
		Montreal, Quebec H4A,2L8
		800-361-3651 (Voice)
		514-489-8255 (Fax)

*********************************************************************
*/

#ifdef __cplusplus
	#define CXC_DECOR extern "C"
#else
	#define CXC_DECOR
#endif

#ifdef CXC_EXPORTS
//	#define CXC_VER 0x200 // 2010/08/10
	#define CXC_VER 0x300 // 2020/06/03
	#define CXC_API(T) CXC_DECOR __declspec(dllexport) T WINAPI
#else
	#ifndef CXC_VER
	#define CXC_VER 0x100 // you must #define CXC_VER if you want updated features
	#endif
	#define CXC_API(T) CXC_DECOR __declspec(dllimport) T WINAPI
#endif

#if defined( _UNICODE ) || defined( UNICODE )
	#define CXC_FUN_AW(f) f##W
	#define CXC_TYP_AW(t) f##_W
#else
	#define CXC_FUN_AW(f) f##A
	#define CXC_TYP_AW(t) f##_A
#endif

#ifdef __midl
	#define IN [ in ]
	#define OUT [ out ]
	#define OPTIONAL [ defaultvalue(0) ]
	#define WINAPI __stdcall
	#define CXC_MIDL_HELP(x) [ helpstring( x ) ]
	#define CXC_MIDL_ENTRY(x) [ entry( #x ), usesgetlasterror ]
#else
	#define CXC_MIDL_HELP(x)
	#define CXC_MIDL_ENTRY(x)
#endif

//-------------------------------------------------------------------

#ifndef MIDL_MODULE

#include "pshpack8.h"

	typedef enum tag_CXC_CONSTANTS
	{
		CXC_MAX_ACTIONS = 8
	#if 0x200 <= CXC_VER
	,	CXCEX_MAX_ACTIONS = 64
	#endif // 0x200 <= CXC_VER
	#if 0x300 <= CXC_VER
	,	CXC_MAX_SAMPLING_RATE = 64
	#endif // 0x300 <= CXC_VER
		// event bits in CXC_RESULT:
	,	CXC_GOT_NEW_STATE = 1<<0
	,	CXC_GOT_CLIENTID  = 1<<1
	,	CXC_GOT_FORMAT    = 1<<2
	,	CXC_GOT_START     = 1<<7
	,	CXC_GOT_RESUME    = 1<<8
	#if 0x200 <= CXC_VER
	,	CXC_GOT_SESSION_STATE = 1<<9
	,	CXC_GOT_SESSION_MODE  = 1<<10
	#endif // 0x200 <= CXC_VER
	#ifdef __midl
	,	CXC_VERSION = 0x100
	#if 0x200 <= CXC_VER
	,	CXCEX_VERSION = 0x200
	#endif // 0x200 <= CXC_VER
	#if 0x300 <= CXC_VER
	,	CXC_VERSION_1 = 0x100
	,	CXC_VERSION_2 = 0x200
	,	CXC_VERSION_3 = 0x300
	#endif // 0x300 <= CXC_VER
	#endif
	}
	CXC_CONSTANTS;

	typedef enum tag_CXC_RESULT
	{
		// success states:
		CXC_S_PENDING  = 0
		// error states:
	,	CXC_E_ERROR            = -1 // check GetLastError
	,	CXC_E_NOT_INITIALIZED  = -2
	,	CXC_E_NOT_CONNECTED    = -3
	,	CXC_E_SESS_IN_PROGRESS = -4
	,	CXC_E_SESS_PAUSED      = -6
	,	CXC_E_CONNECTION_LOST  = -5
	}
	CXC_RESULT;

	#if 0x200 <= CXC_VER

	typedef enum tag_CXC_ERROR_CODES
	{
		CXC_ERROR_UNEXPECTED      = 0xA0040201u
	,	CXC_ERROR_POINTER         = 0xA0040202u
	,	CXC_ERROR_INVALIDARG      = 0xA0040203u
	,	CXC_ERROR_VERNOTSUPPORTED = 0xA0040210u
	,	CXC_ERROR_NOTINITIALIZED  = 0xA0040211u
	,	CXC_ERROR_NOTATTHISTIME   = 0xA0040212u
	}
	CXC_ERROR_CODES;

	typedef enum tag_CXCEX_SESSION_MODES
	{
		CXCEX_SESSION_MODE_UNKNOWN
	,	CXCEX_SESSION_MODE_RECORD
	,	CXCEX_SESSION_MODE_REPLAY
	}
	CXCEX_SESSION_MODES;
	
	typedef enum tag_CXCEX_SESSION_STATES
	{
		CXCEX_SESSION_STATE_UNKNOWN
	,	CXCEX_SESSION_STATE_RUNNING
	,	CXCEX_SESSION_STATE_PAUSED
	,	CXCEX_SESSION_STATE_STOPPED
	,	CXCEX_SESSION_STATE_FROZEN
	}
	CXCEX_SESSION_STATES;

	#endif // 0x200 <= CXC_VER

	typedef enum tag_CXC_FORMAT
	{
		CXC_FORMAT_NONE       = 0
	,	CXC_FORMAT_BOOLEAN    = 1
	,	CXC_FORMAT_PERCENTAGE = 2
	,	CXC_FORMAT_FULL       = 3
	}
	CXC_FORMAT;

	typedef enum tag_CXC_FORMAT_FLAG
	{
		CXC_TYPE        = 1<<0
	,	CXC_SCALE_MIN   = 1<<1
	,	CXC_SCALE_MAX   = 1<<2
	,	CXC_THRESHOLD_1 = 1<<3
	,	CXC_THRESHOLD_2 = 1<<4
	}
	CXC_FORMAT_FLAG;

	typedef float CXC_SAMPLE;

	typedef struct tag_CXC_ACTION_FORMAT
	{
		CXC_MIDL_HELP( "Set of one or more CXC_FORMAT_FLAG's indicating which fields are valid" )
		long maskValid;
		CXC_MIDL_HELP( "Set of one or more CXC_FORMAT_FLAG's indicating which fields have changed" )
		long maskChanged;
		CXC_FORMAT eType;
		CXC_SAMPLE nScaleMin;
		CXC_SAMPLE nScaleMax;
		CXC_SAMPLE nThreshold1;
		CXC_SAMPLE nThreshold2;
	}
	CXC_ACTION_FORMAT;

	typedef struct tag_CXC_ACTION_STATE
	{
		CXC_MIDL_HELP( "Last value received" )
		CXC_SAMPLE nLast;
		CXC_MIDL_HELP( "Minimum of values received since the last call to cxcGetStates" )
		CXC_SAMPLE nMin;
		CXC_MIDL_HELP( "Maximum of values received since the last call to cxcGetStates" )
		CXC_SAMPLE nMax;
		CXC_MIDL_HELP( "Number of values received since the last call to cxcGetStates" )
		int cSpan;
	}
	CXC_ACTION_STATE;

#include "poppack.h"

#endif // ! MIDL_MODULE

//-------------------------------------------------------------------

#if ! defined( __midl ) || defined( MIDL_MODULE )

	CXC_MIDL_ENTRY( _cxcInitialize@4 )
	CXC_API( BOOL )
	cxcInitialize( long nCxcVersion );

	CXC_MIDL_ENTRY( _cxcTerminate@0 )
	CXC_API( void )
	cxcTerminate( void );

	#ifdef __midl
	#define cxcConnectW cxcConnect
	#endif
	CXC_MIDL_ENTRY( _cxcConnectW@4 )
	CXC_API( BOOL )
	cxcConnectW( IN LPCWSTR szwServer );

	#ifndef __midl
	CXC_API( BOOL )
	cxcConnectA( IN LPCSTR szServer );
	#define cxcConnect CXC_FUN_AW(cxcConnect)
	#endif

	CXC_MIDL_ENTRY( _cxcDisconnect@0 )
	CXC_API( void )
	cxcDisconnect( void );

	CXC_MIDL_ENTRY( _cxcGetClientId@4 )
	CXC_API( BOOL )
	cxcGetClientId( OUT long * pnClientId );

	CXC_MIDL_ENTRY( _cxcGetFormats@12 )
	CXC_API( BOOL )
	cxcGetFormats( OUT CXC_ACTION_FORMAT * pFormats
		, IN OUT      int * pcFormats
		, IN OPTIONAL int iStart
		);

	CXC_MIDL_ENTRY( _cxcGetStates@12 )
	CXC_API( CXC_RESULT )
	cxcGetStates( OUT CXC_ACTION_STATE * pStates
		, IN OUT      int * pcStates
		, IN OPTIONAL int iStart
		);

	#if 0x200 <= CXC_VER

	CXC_MIDL_ENTRY( _cxcSetPort@4 )
	CXC_API( BOOL )
	cxcSetPort( long nPort );

	CXC_MIDL_ENTRY( _cxcGetSessionMode@4 )
	CXC_API( BOOL )
	cxcGetSessionMode( OUT /* CXCEX_SESSION_MODES */ long * pnSessionMode );

	CXC_MIDL_ENTRY( _cxcGetSessionState@4 )
	CXC_API( BOOL )
	cxcGetSessionState( OUT /* CXCEX_SESSION_STATES */ long * pnSessionState );

	#endif // 0x200 <= CXC_VER

	#if 0x300 <= CXC_VER

	CXC_MIDL_ENTRY( _cxcGetActionData@20 )
	CXC_API( BOOL )
	cxcGetActionData( IN int iAction
		, IN int cBufferSamples // size of buffer, in samples
		, OUT CXC_SAMPLE * pBuffer // pointer to buffer, cannot be NULL
		, OUT int * pcDataSamples // count of data samples returned in buffer, cannot be NULL
		, OUT int * pcLostSamples // count of samples overwritten by newer samples, can be NULL if don't care
		);

	CXC_MIDL_ENTRY( _cxcGetDataSampleRate@4 )
	CXC_API( int )
	cxcGetDataSampleRate( IN int iAction );

	#endif // 0x300 <= CXC_VER

#endif // MIDL_MODULE

//-------------------------------------------------------------------

#endif // CXC_H
