/////////////////////////////////////////////////////////////////////
//
// VcTestClient.cpp : Defines the entry point for the application.
/*
*********************************************************************

	COPYRIGHT (c) 2008 Thought Technology Ltd.

	Connection Client API Sample Application

	About:

		Simple application to demonstrate expected normal use of the
		Connection Client API in "immediate mode".

	Targets:

		Windows GUI application, multi-byte or unicode.

	Written for:

		Thought Technology Ltd.
		2180 Belgrave Ave.
		Montreal, Quebec H4A,2L8
		800-361-3651 (Voice)
		514-489-8255 (Fax)

	Written by:

		Mark Richards

*********************************************************************
*/
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "resource.h"

#if 1
#define CXC_VER 0x300 // we want new functions
#endif
#include "../../ConnectionTools/ConnectionClient.h"

#ifdef _M_X64
    #ifndef NDEBUG
        #pragma comment( lib, "../../ConnectionTools/x64/Debug/ConnectionClientD.lib" )
    #else
        #pragma comment( lib, "../../ConnectionTools/x64/Release/ConnectionClient.lib" )
    #endif
#else
    #ifndef NDEBUG
        #pragma comment( lib, "../../ConnectionTools/Debug/ConnectionClientD.lib" )
    #else
        #pragma comment( lib, "../../ConnectionTools/Release/ConnectionClient.lib" )
    #endif
#endif


/////////////////////////////////////////////////////////////////////
// Process thread

static HINSTANCE g_hModule = NULL;

static INT_PTR CALLBACK DlgProc( HWND, UINT, WPARAM, LPARAM );

int APIENTRY WinMain( HINSTANCE hInstance
	, HINSTANCE //hPrevInstance
	, LPSTR     //lpCmdLine
	, int       //nCmdShow
	)
{
	int nExitCode = EXIT_FAILURE;

	g_hModule = hInstance;

	INITCOMMONCONTROLSEX icc = { sizeof icc, ICC_INTERNET_CLASSES };
	::InitCommonControlsEx( &icc );

	/*
		USAGE cxcInitialize
	*/
	if ( ! cxcInitialize( CXC_VER ) )
	{
		::MessageBox( NULL
			, _T("Fatal error initializing connection client.")
			, _T("VcTestClient")
			, MB_OK | MB_ICONSTOP
			);
		return nExitCode;
	}

#ifdef USE_MODAL_DIALOG

	nExitCode = ::DialogBox( hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), NULL, DlgProc );

#else

	HWND hMainWnd = ::CreateDialog( hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), NULL, DlgProc );

	::ShowWindow( hMainWnd, SW_SHOWNORMAL );

	MSG msg = { 0, };

	while ( ::GetMessage( &msg, NULL, 0, 0 ) )
	{
		if ( ::IsDialogMessage( hMainWnd, &msg ) ) continue;
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}

	nExitCode = (int)msg.wParam;

#endif

	/*
		USAGE cxcTerminate
	*/
	cxcTerminate();

	return nExitCode;
}


/////////////////////////////////////////////////////////////////////
// Application specific

enum
{
	MAX_CHARS_SERVER = 32
,	MAX_CHARS_LABELS = 32
,	FRAME_RATE = 30 // Hz : imitate game rate, otherwise 16Hz is ok
,	FRAME_PERIOD = 1000 / FRAME_RATE // ms
};

POINT g_ptMinSize = { 0, };
SIZE g_sMargin = { 0, }; // left/right, and bottom margin
int g_dxSpaces = 0;
int g_dyLabels = 0;

LPCTSTR g_szDefaultServer = _T("127.0.0.1");
TCHAR g_szServer[ MAX_CHARS_SERVER ] = { 0, };
bool g_bConnected = false;
UINT_PTR g_uiFrameRateTimer = 0; // emulate a game's frame loop
CXC_ACTION_FORMAT g_aActionFormats[ CXC_MAX_ACTIONS ] = { 0, };
CXC_ACTION_STATE g_aActionStates[ CXC_MAX_ACTIONS ] = { 0, };
bool g_bPausedOrBlocked = false;
typedef TCHAR LABEL[ MAX_CHARS_LABELS ];
struct BAR { bool bEnabled; LABEL szLabelTop, szLabelBot; CXC_SAMPLE nYTop, nYBot, nYCur; };
BAR g_aBars[ CXC_MAX_ACTIONS ] = { 0, };

#if 0x300 <= CXC_VER

UINT g_uiPulseTimer = 0;
int g_cPulseSamples = 0;
enum { PULSE_ACTION = 0 };

static void PulseTimerStart( HWND hWnd );
static void PulseTimerStop( void );
static VOID CALLBACK OnPulseTimer( UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR );

#endif // 0x300 <= CXC_VER

static void DoConnect( HWND hWnd );
static void DoDisconnect( HWND hWnd );
static VOID CALLBACK OnFrameRateTimer( HWND hWnd, UINT, UINT_PTR, DWORD );
static void InvalidateFormat( HWND hWnd, int iAction, const CXC_ACTION_FORMAT & oFormat );
static void InvalidateState( HWND hWnd, int iAction, const CXC_ACTION_STATE & oState );
static void Message( HWND hWnd, LPCTSTR szFormat, ... );

#if 0x200 <= CXC_VER

inline LPCTSTR TextOf( CXCEX_SESSION_MODES eMode )
{
	switch ( eMode )
	{
	default :
	case CXCEX_SESSION_MODE_UNKNOWN : return _T("UNKNOWN");
	case CXCEX_SESSION_MODE_RECORD  : return _T("RECORD");
	case CXCEX_SESSION_MODE_REPLAY  : return _T("REPLAY");
	}
}

inline LPCTSTR TextOf( CXCEX_SESSION_STATES eState )
{
	switch ( eState )
	{
	default :
	case CXCEX_SESSION_STATE_UNKNOWN : return _T("UNKNOWN");
	case CXCEX_SESSION_STATE_RUNNING : return _T("RUNNING");
	case CXCEX_SESSION_STATE_PAUSED  : return _T("PAUSED");
	case CXCEX_SESSION_STATE_STOPPED : return _T("STOPPED");
	case CXCEX_SESSION_STATE_FROZEN  : return _T("FROZEN");
	}
}

#endif // 0x200 <= CXC_VER


static BOOL OnInitDialog( HWND hWnd, HWND /*hwndFocus*/, LPARAM /*lParam*/ )
{
	RECT rc = { 0, };
	::GetWindowRect( hWnd, &rc );
	g_ptMinSize.x = rc.right - rc.left;
	g_ptMinSize.y = rc.bottom - rc.top;

	RECT rc0 = { 0, };
	RECT rc1 = { 0, };

	for ( int iAction = 0; iAction < CXC_MAX_ACTIONS; ++iAction )
	{
		HWND hwndAction = ::GetDlgItem( hWnd, IDC_ACTION0 + iAction );
		::SetWindowLong( hwndAction, GWL_STYLE
			, SS_OWNERDRAW
			| ::GetWindowLong( hwndAction, GWL_STYLE )
			);

		switch ( iAction )
		{
		case 0 : ::GetWindowRect( hwndAction, &rc0 ); break;
		case 1 : ::GetWindowRect( hwndAction, &rc1 ); break;
		}

		CXC_ACTION_FORMAT & oFormat = g_aActionFormats[ iAction ];
		InvalidateFormat( hWnd, iAction, oFormat );
	}

	::MapWindowPoints( NULL, hWnd, reinterpret_cast< LPPOINT >( &rc1 ), 2 );
	::MapWindowPoints( NULL, hWnd, reinterpret_cast< LPPOINT >( &rc0 ), 2 );
	::GetClientRect( hWnd, &rc );

	g_sMargin.cx = rc0.left - rc.left; // left/right margin
	g_sMargin.cy = rc.bottom - rc0.bottom; // bottom margin
	g_dxSpaces = rc1.left - rc0.right; // target horizontal separation

#if 0x300 <= CXC_VER

	PulseTimerStart( hWnd );

#endif // 0x300 <= CXC_VER

	DoConnect( hWnd );

	if ( ! g_bConnected )
	{
		Message( hWnd,
				_T("\r\n1) Make sure BioGraph is running.")
				_T("\r\n2) Enter BioGraph's IP.")
				_T("\r\n3) Click on the check box.")
				_T("\r\n")
				);
	}

	::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );

	return TRUE;
}

static void OnClickConnected( HWND hWnd, HWND /*hwndCheck*/ )
{
	if ( g_bConnected )
	{
		DoDisconnect( hWnd );
	}
	else
	{
		DoConnect( hWnd );
	}
}

static LRESULT OnDestroy( HWND hWnd )
{
	if ( g_bConnected )
	{
		DoDisconnect( hWnd );
	}

#if 0x300 <= CXC_VER

	PulseTimerStop();

#endif // 0x300 <= CXC_VER

	return 0;
}


//-------------------------------------------------------------------

#if 0x300 <= CXC_VER

#pragma warning( disable : 4201 ) // nameless struct/union
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

static void PulseTimerStart( HWND hWnd )
{
	g_uiPulseTimer = ::timeSetEvent( 2000, 0, OnPulseTimer
		, (DWORD_PTR)( hWnd )
		, TIME_PERIODIC
		);
}

static void PulseTimerStop( void )
{
	::timeKillEvent( g_uiPulseTimer ); g_uiPulseTimer = 0;
}

static VOID CALLBACK OnPulseTimer( UINT uID, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR )
{
	if ( uID != g_uiPulseTimer ) return;

	HWND hWnd = (HWND)( dwUser );

	/*
		USAGE cxcGetActionData

		The purpose of this call to cxcGetActionData is to show that it
		can be called anytime from any thread, and that it will not
		interfere with cxcGetStates.

		The tally of samples gotten here for PULSE_ACTION is saved into
		g_cPulseSamples, to be taken into account when getting samples
		and checking tallies inside OnFrameRateTimer.
	*/
	enum { MAX_BUFFER = CXC_MAX_SAMPLING_RATE * 4 };
	CXC_SAMPLE aBuffer[ MAX_BUFFER ];
	int cRead = 0;
	int cLost = 0;
	cxcGetActionData( PULSE_ACTION, MAX_BUFFER, aBuffer, &cRead, &cLost );

	if ( 0 == cRead && 0 == cLost ) return;

	g_cPulseSamples += cLost + cRead;

	Message( hWnd, _T("Action %d: on pulse lost %d, read %d\r\n"), PULSE_ACTION
		, cLost, cRead
		);
}

#endif // 0x300 <= CXC_VER

static void DoConnect( HWND hWnd )
{
	// get IP address

	DWORD dwIpAddress = 0;
	HWND hwndIP = ::GetDlgItem( hWnd, IDC_IPADDRESS );
	int cFields = (int)::SendMessage( hwndIP, IPM_GETADDRESS, 0, (LPARAM)( &dwIpAddress ) );

	if ( 0 != cFields && 4 != cFields )
	{
		Message( hWnd, _T("\r\nIncomplete IP address.\r\n") );
		return;
	}

	if ( 0 == cFields )
		_tcsncpy( g_szServer, g_szDefaultServer, MAX_CHARS_SERVER );
	else
		_sntprintf( g_szServer, MAX_CHARS_SERVER, _T("%d.%d.%d.%d")
			, FIRST_IPADDRESS( dwIpAddress )
			, SECOND_IPADDRESS( dwIpAddress )
			, THIRD_IPADDRESS( dwIpAddress )
			, FOURTH_IPADDRESS( dwIpAddress )
			);

	/*
		USAGE cxcConnect
	*/
	HCURSOR oldCursor = ::SetCursor( ::LoadCursor( NULL, IDC_WAIT ) );
	BOOL bRetVal = cxcConnect( g_szServer );
	::SetCursor( oldCursor );

	if ( ! bRetVal )
	{
		Message( hWnd, _T("\r\nCould not connect to \"%s\".\r\n"), g_szServer );
		return;
	}

	Message( hWnd, _T("\r\nConnected to BioGraph at \"%s\".\r\n"), g_szDefaultServer );
	::SendMessage( ::GetDlgItem( hWnd, IDC_CHECK_CONNECTED ), BM_SETCHECK, BST_CHECKED, 0 );
	::EnableWindow( hwndIP, FALSE );

	g_bConnected = true;
	g_uiFrameRateTimer = ::SetTimer( hWnd, 1, FRAME_PERIOD, (TIMERPROC)( OnFrameRateTimer ) ); // typecast needed if PDK not installed
}

static void DoDisconnect( HWND hWnd )
{
	g_bConnected = false;
	::KillTimer( hWnd, g_uiFrameRateTimer ); 
	g_uiFrameRateTimer = 0;

	/*
		USAGE cxcDisconnect
	*/
	cxcDisconnect();

	Message( hWnd, _T("\r\nDisconnected from \"%s\".\r\n"), g_szServer );
	::SendMessage( ::GetDlgItem( hWnd, IDC_CHECK_CONNECTED ), BM_SETCHECK, BST_UNCHECKED, 0 );
	::EnableWindow( ::GetDlgItem( hWnd, IDC_IPADDRESS ), TRUE );

	// clear state and update display:

	g_bPausedOrBlocked = false;

	::ZeroMemory( g_aActionFormats, sizeof g_aActionFormats );
	::ZeroMemory( g_aActionStates, sizeof g_aActionStates );

	for ( int iAction = 0; iAction < CXC_MAX_ACTIONS; ++iAction )
	{
		CXC_ACTION_FORMAT & oFormat = g_aActionFormats[ iAction ];
		InvalidateFormat( hWnd, iAction, oFormat );
	}
}


//-------------------------------------------------------------------

static VOID CALLBACK OnFrameRateTimer( HWND hWnd, UINT, UINT_PTR, DWORD )
{
	if ( ! g_bConnected ) return; // can always be one call after KillTimer

	/*
		USAGE cxcGetStates
	*/
	int cStates = CXC_MAX_ACTIONS;
	long nResult = cxcGetStates( g_aActionStates, &cStates, 0 );

	// Checking for lost connections and errors:

	if ( nResult < 0 )
	{
		switch ( nResult )
		{
		case CXC_E_CONNECTION_LOST :
			Message( hWnd, _T("*** Lost connection to \"%s\".\r\n"), g_szServer );
			DoDisconnect( hWnd );
			return;
		case CXC_E_SESS_IN_PROGRESS :
			if ( ! g_bPausedOrBlocked )
			{
				Message( hWnd, _T("*** Session already in progress.\r\n")
					_T("Waiting for proper session start.\r\n")
					);
				g_bPausedOrBlocked = true; // Don't show message again
			}
			return;
		case CXC_E_SESS_PAUSED :
			if ( ! g_bPausedOrBlocked )
			{
				Message( hWnd, _T("Stream PAUSED.\r\n") );
				g_bPausedOrBlocked = true; // Don't show message again
			}
			return;
		default :
			return;
		}
	}

	// Checking for updates:

	if ( 0 != ( CXC_GOT_START & nResult ) )
	{
		g_bPausedOrBlocked = false;

		Message( hWnd, _T("Stream STARTED.\r\n") );
	}

	if ( 0 != ( CXC_GOT_RESUME & nResult ) )
	{
		g_bPausedOrBlocked = false;

		Message( hWnd, _T("Stream RESUMED.\r\n") );
	}

	if ( 0 != ( CXC_GOT_CLIENTID & nResult ) )
	{
		/*
			USAGE cxcGetClientId
		*/
		long nClientId = 0;
		cxcGetClientId( &nClientId );

		Message( hWnd, _T("Client ID is %ld.\r\n"), nClientId );
	}

#if 0x200 <= CXC_VER

	if ( 0 != ( CXC_GOT_SESSION_MODE & nResult ) )
	{
		/*
			USAGE cxcGetSessionMode
		*/
		long nSessionMode = CXCEX_SESSION_MODE_UNKNOWN;
		cxcGetSessionMode( &nSessionMode );

		Message( hWnd, _T("Session Mode is %s.\r\n"), TextOf( (CXCEX_SESSION_MODES)( nSessionMode ) ) );
	}

	if ( 0 != ( CXC_GOT_SESSION_STATE & nResult ) )
	{
		/*
			USAGE cxcGetSessionState
		*/
		long nSessionState = CXCEX_SESSION_STATE_UNKNOWN;
		cxcGetSessionState( &nSessionState );

		Message( hWnd, _T("Session State is %s.\r\n"), TextOf( (CXCEX_SESSION_STATES)( nSessionState ) ) );
	}

#endif // 0x200 <= CXC_VER

	if ( 0 != ( CXC_GOT_FORMAT & nResult ) )
	{
		/*
			USAGE cxcGetFormats
		*/
		int cFormats = CXC_MAX_ACTIONS;
		cxcGetFormats( g_aActionFormats, &cFormats, 0 );

		for ( int iAction = 0; iAction < cFormats; ++iAction )
		{
			CXC_ACTION_FORMAT & oFormat = g_aActionFormats[ iAction ];
			if ( 0 == oFormat.maskChanged ) continue;
			Message( hWnd, _T("Action %d's format updated.\r\n"), iAction );
			InvalidateFormat( hWnd, iAction , oFormat );
			oFormat.maskChanged = 0;
		}
	}

	if ( 0 != ( CXC_GOT_NEW_STATE & nResult ) )
	{
		for ( int iAction = 0; iAction < cStates; ++iAction )
		{
			CXC_ACTION_STATE & oState = g_aActionStates[ iAction ];
			if ( 0 == oState.cSpan ) continue;

#if 0x300 <= CXC_VER

			/*
				USAGE cxcGetActionData

				The purpose of this call to cxcGetActionData is to demonstrate
				that the tally of samples considered by cxcGetActionData and by
				cxcGetState are the same.

				Because of asynchrony, there will be some differences, but
				they should always return to 0.

				When there is no difference, we verify that the last, max, and
				min sample values in the buffer match the current state.
			*/
			enum { MAX_BUFFER = CXC_MAX_SAMPLING_RATE * 2 };
			CXC_SAMPLE aBuffer[ MAX_BUFFER ];
			int cRead = 0;
			int cLost = 0;
			BOOL bRes = cxcGetActionData( iAction, MAX_BUFFER, aBuffer, &cRead, &cLost );
			if (bRes)
			{
				if ( 0 < cLost || cRead + cLost != oState.cSpan )
				{
					static int acMissing[ CXC_MAX_ACTIONS ] = { 0, };

					acMissing[ iAction ] += oState.cSpan - cLost - cRead;

					Message( hWnd, _T("Action %d: span %d, lost %d, read %d, miss %d\r\n"), iAction
						, oState.cSpan
						, cLost, cRead
						, acMissing[ iAction ]
						- ( PULSE_ACTION == iAction ? g_cPulseSamples : 0 )
						);
				}
				else // 0 == cLost && cRead == oState.cSpan
				{
					CXC_SAMPLE nLast = 0, nMin = 0, nMax = 0;

					for ( int i = 0; i < cRead; ++i )
					{
						nLast = aBuffer[ i ];
						if ( 0 == i || nLast < nMin ) nMin = nLast;
						if ( 0 == i || nLast > nMax ) nMax = nLast;
					}

					assert( nLast == oState.nLast && nMin == oState.nMin && nMax == oState.nMax );
				}
			}
			else
				Message( hWnd, _T("Action %d: error reading the action data array\r\n"), iAction );

#endif // 0x300 <= CXC_VER

			InvalidateState( hWnd, iAction , oState );
			oState.cSpan = 0;
		}
	}
}


/////////////////////////////////////////////////////////////////////
// Drawing handlers

static void CreateGdiResources( HWND hWnd )
{
	HDC hDC = ::GetDC( hWnd );
	HGDIOBJ holdFont = ::SelectObject( hDC, (HGDIOBJ)( ::SendMessage( hWnd, WM_GETFONT, 0, 0 ) ) );
	TEXTMETRIC tm = { 0, };
	::GetTextMetrics( hDC, &tm );
	g_dyLabels = tm.tmHeight;
	::SelectObject( hDC, holdFont );
	::ReleaseDC( hWnd, hDC );
}

static void DeleteGdiResources( bool /*bFinalRelease*/ )
{
}

static LRESULT OnGetMinMaxInfo( HWND /*hWnd*/, LPMINMAXINFO pmmi )
{
#ifdef NDEBUG
	pmmi->ptMinTrackSize = g_ptMinSize;
#else
	pmmi; // allow any size, to test drawing code in extreme cases
#endif

	return 0;
}

static LRESULT OnSize( HWND hWnd, UINT nType, int /*cx*/, int /*cy*/ )
{
	if ( SIZE_RESTORED == nType || SIZE_MAXIMIZED == nType )
	{
		RECT rc = { 0, };
		::GetClientRect( hWnd, &rc );

		RECT rc0 = { 0, };
		::GetWindowRect( ::GetDlgItem( hWnd, IDC_ACTION0 ), &rc0 );
		::MapWindowPoints( NULL, hWnd, reinterpret_cast< LPPOINT >( &rc0 ), 2 );

		int cxExtra = rc.right + g_dxSpaces - rc.left - 2 * g_sMargin.cx;
		const int dx = cxExtra / CXC_MAX_ACTIONS;
		cxExtra -= dx * CXC_MAX_ACTIONS;

		const int cxBar = dx - g_dxSpaces;
		const int cyBar = rc.bottom - rc0.top - g_sMargin.cy;

		HDWP hDwp = ::BeginDeferWindowPos( CXC_MAX_ACTIONS );

		for	( int iAction = 0, x = g_sMargin.cx, y = rc0.top
			; iAction < CXC_MAX_ACTIONS
			; ++iAction, x += dx + ( 0 < cxExtra-- )
			)
		{
			hDwp = ::DeferWindowPos( hDwp
				, ::GetDlgItem( hWnd, IDC_ACTION0 + iAction )
				, NULL, x, y, cxBar, cyBar
				, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS
				);
		}

		::EndDeferWindowPos( hDwp );
	}

	return 0;
}

BOOL OnDrawItem( HWND hWnd, const DRAWITEMSTRUCT * pdi )
{
	if ( 0 == ( ODA_DRAWENTIRE & pdi->itemAction ) ) return TRUE;

	const BAR & bar = g_aBars[ pdi->CtlID - IDC_ACTION0 ];

	HGDIOBJ holdFont = ::SelectObject( pdi->hDC, (HGDIOBJ)( ::SendMessage( hWnd, WM_GETFONT, 0, 0 ) ) );
	UINT oldTextAlign = ::GetTextAlign( pdi->hDC );
	COLORREF oldBkColor = ::SetBkColor( pdi->hDC, ::GetSysColor( COLOR_3DFACE ) );

	if ( pdi->rcItem.bottom - pdi->rcItem.top < 2*g_dyLabels + 2 )
	{
		::ExtTextOut( pdi->hDC, 0, 0, ETO_OPAQUE, &pdi->rcItem, NULL, 0, NULL );
	}
	else
	{
		const int xCenter = ( pdi->rcItem.left + pdi->rcItem.right ) / 2;

		RECT rcTop = pdi->rcItem;
		rcTop.bottom = rcTop.top + g_dyLabels;
		if ( ::RectVisible( pdi->hDC, &rcTop ) )
		{
			::SetTextAlign( pdi->hDC, TA_CENTER | TA_TOP );
			::ExtTextOut( pdi->hDC, xCenter, rcTop.top, ETO_OPAQUE, &rcTop
				, bar.szLabelTop, lstrlen( bar.szLabelTop ), NULL
				);
		}

		RECT rcBot = pdi->rcItem;
		rcBot.top = rcBot.bottom - g_dyLabels;
		if ( ::RectVisible( pdi->hDC, &rcBot ) )
		{
			::SetTextAlign( pdi->hDC, TA_CENTER | TA_BOTTOM );
			::ExtTextOut( pdi->hDC, xCenter, rcBot.bottom, ETO_OPAQUE, &rcBot
				, bar.szLabelBot, lstrlen( bar.szLabelBot ), NULL
				);
		}

		RECT rcBar = { pdi->rcItem.left, rcTop.bottom, pdi->rcItem.right, rcBot.top };
		if ( ::RectVisible( pdi->hDC, &rcBar ) )
		{
			if ( bar.bEnabled )
			{
				const int cy = rcBar.bottom - rcBar.top;
				const int y
					= bar.nYTop <= bar.nYCur ? cy
					: bar.nYCur <= bar.nYBot ? 0
					: (int)( cy * ( bar.nYCur - bar.nYBot ) / ( bar.nYTop - bar.nYBot ) )
					;
				if ( y < cy )
				{
					RECT rc = rcBar;
					rc.bottom = rc.top + ( cy - y );
					if ( ::RectVisible( pdi->hDC, &rc ) )
					{
						::SetBkColor( pdi->hDC, ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
						::ExtTextOut( pdi->hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
					}
				}
				if ( 0 < y )
				{
					RECT rc = rcBar;
					rc.top = rc.bottom - y;
					if ( ::RectVisible( pdi->hDC, &rc ) )
					{
						::SetBkColor( pdi->hDC, ::GetSysColor( COLOR_HIGHLIGHT ) );
						::ExtTextOut( pdi->hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
					}
				}
			}
			else
			{
				::DrawEdge( pdi->hDC, &rcBar, EDGE_SUNKEN, BF_RECT | BF_FLAT | BF_ADJUST );
				::ExtTextOut( pdi->hDC, 0, 0, ETO_OPAQUE, &rcBar, NULL, 0, NULL );
			}
		}
	}

	::SetBkColor( pdi->hDC, oldBkColor );
	::SetTextAlign( pdi->hDC, oldTextAlign );
	::SelectObject( pdi->hDC, holdFont );

	return TRUE;
}

static void InvalidateFormat( HWND hWnd, int iAction, const CXC_ACTION_FORMAT & oFormat )
{
	BAR & bar = g_aBars[ iAction ];

	bar.bEnabled = true;

	LPCTSTR szUnits = _T("");

	switch ( oFormat.eType )
	{
	case CXC_FORMAT_FULL :
		bar.nYTop = oFormat.nScaleMax;
		bar.nYBot = oFormat.nScaleMin;
		break;
	case CXC_FORMAT_PERCENTAGE :
		bar.nYTop = 100;
		bar.nYBot = 0;
		szUnits = _T("%");
		break;
	case CXC_FORMAT_BOOLEAN :
		bar.nYTop = 1;
		bar.nYBot = 0;
		break;
	default:
		bar.bEnabled = false;
		_tcsncpy( bar.szLabelTop, _T("-"), MAX_CHARS_LABELS );
		_tcsncpy( bar.szLabelBot, _T("-"), MAX_CHARS_LABELS );
		break;
	}

	if ( bar.bEnabled )
	{
		_sntprintf( bar.szLabelTop, MAX_CHARS_LABELS, _T("%g%s"), bar.nYTop, szUnits );
		_sntprintf( bar.szLabelBot, MAX_CHARS_LABELS, _T("%g%s"), bar.nYBot, szUnits );
	}

	::InvalidateRect( ::GetDlgItem( hWnd, IDC_ACTION0 + iAction ), NULL, FALSE );
}

static void InvalidateState( HWND hWnd, int iAction, const CXC_ACTION_STATE & oState )
{
	BAR & bar = g_aBars[ iAction ];

	bar.nYCur = oState.nLast;

	HWND hwndAction = ::GetDlgItem( hWnd, IDC_ACTION0 + iAction );

	RECT rc = { 0, };
	::GetClientRect( hwndAction, &rc );
	rc.top += g_dyLabels;
	rc.bottom -= g_dyLabels;
	if ( ::IsRectEmpty( &rc ) ) return;

	::InvalidateRect( ::GetDlgItem( hWnd, IDC_ACTION0 + iAction ), NULL, FALSE );
}


/////////////////////////////////////////////////////////////////////
// Helper functions

static void Message( HWND hWnd, LPCTSTR szFormat, ... )
{
	enum { MAX_TEXT = 256 };
	TCHAR szText[ MAX_TEXT ] = { 0, };
	va_list arglist;
	va_start( arglist, szFormat );
	int cchText = _vsntprintf( szText, MAX_TEXT, szFormat, arglist );
	va_end( arglist );
	if ( 0 == cchText ) return;
	if ( 0 > cchText ) szText[ cchText = MAX_TEXT - 1 ] = 0;

	HWND hwndEdit = ::GetDlgItem( hWnd, IDC_EDIT_MESSAGES );
	if ( NULL == hwndEdit ) return;

	::SendMessage( hwndEdit, EM_EMPTYUNDOBUFFER, 0, 0 );
	int cchEdit = (int)::SendMessage( hwndEdit, WM_GETTEXTLENGTH, 0, 0 );
	::SendMessage( hwndEdit, EM_SETSEL, cchEdit, cchEdit );
	::SendMessage( hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)( szText ) );

	// check for EN_ERRSPACE / EN_MAXTEXT undo, and handle by making space etc.

	int iSelEnd = 0;
	::SendMessage( hwndEdit, EM_GETSEL, 0, (LPARAM)( &iSelEnd ) );
	if ( iSelEnd < cchEdit + cchText )
	{
		int iLine = (int)::SendMessage( hwndEdit, EM_LINEFROMCHAR, cchEdit / 2, 0 );
		int iChar = (int)::SendMessage( hwndEdit, EM_LINEINDEX, iLine, 0 );
		::SendMessage( hwndEdit, EM_SETSEL, 0, iChar );
		::SendMessage( hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)( _T("") ) );
		cchEdit -= iChar;
		::SendMessage( hwndEdit, EM_SETSEL, cchEdit, cchEdit );
		::SendMessage( hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)( szText ) );
	}
}


/////////////////////////////////////////////////////////////////////
// Dialog Procedure

INT_PTR CALLBACK DlgProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;

	switch ( nMsg ) 
	{
	case WM_INITDIALOG :
		{
			::SendMessage( hWnd
				, WM_SETICON
				, TRUE
				, (LPARAM)::LoadImage( g_hModule, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON
					, ::GetSystemMetrics( SM_CXICON )
					, ::GetSystemMetrics( SM_CYICON )
					, LR_DEFAULTCOLOR
					)
				);
			::SendMessage( hWnd
				, WM_SETICON
				, FALSE
				, (LPARAM)::LoadImage( g_hModule, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON
					, ::GetSystemMetrics( SM_CXSMICON )
					, ::GetSystemMetrics( SM_CYSMICON )
					, LR_DEFAULTCOLOR
					)
				);

			CreateGdiResources( hWnd );
		}
		return OnInitDialog( hWnd, (HWND)( wParam ), lParam ); // WM_INITDIALOG

	case WM_DESTROY :
		{
			lResult = OnDestroy( hWnd );
		}
		break; // WM_DESTROY

	case WM_NCDESTROY :
		{
			DeleteGdiResources( true );
		}
		break; // WM_NCDESTROY

	//-------------------------------------------------------------------

	case WM_SETTINGCHANGE :
		{
			DeleteGdiResources( false );
			CreateGdiResources( hWnd );
		}
		break; // WM_SETTINGCHANGE

	case WM_GETMINMAXINFO :
		{
			lResult = OnGetMinMaxInfo( hWnd, (LPMINMAXINFO)( lParam ) );
		}
		break; // WM_GETMINMAXINFO

	case WM_SIZE :
		{
			lResult = OnSize( hWnd, (UINT)( wParam ), LOWORD( lParam ), HIWORD( lParam ) );
		}
		break; // WM_SIZE

	case WM_DRAWITEM :
		{
			if ( OnDrawItem( hWnd, (const DRAWITEMSTRUCT *)( lParam ) ) )
			{
				::SetWindowLongPtr( hWnd, DWLP_MSGRESULT, TRUE );
				return TRUE;
			}
		}
		break; // WM_DRAWITEM

	//-------------------------------------------------------------------

	case WM_COMMAND :

		switch ( LOWORD( wParam ) )
		{
		case IDC_CHECK_CONNECTED :

			switch ( HIWORD( wParam ) )
			{
			case BN_CLICKED :
				{
					OnClickConnected( hWnd, (HWND)( lParam ) );
				}
				return TRUE;
			}
			break; // WM_COMMAND - button control notifications

		case IDC_EDIT_MESSAGES :

			switch ( HIWORD( wParam ) )
			{
			case EN_ERRSPACE :
			case EN_MAXTEXT :
				{
					::SendMessage( (HWND)( lParam ), EM_UNDO, 0, 0 );
				}
				return TRUE;
			}
			break; // WM_COMMAND - edit control notifications

		case IDCANCEL :
			{
			#ifdef USE_MODAL_DIALOG
				::EndDialog( hWnd, EXIT_SUCCESS /*LOWORD( wParam )*/ );
			#else
				::DestroyWindow( hWnd );
				::PostQuitMessage( EXIT_SUCCESS /*LOWORD( wParam )*/ );
			#endif
			}
			return TRUE; // WM_COMMAND - IDCANCEL
		}
		break; // WM_COMMAND
	}

	if ( 0 == lResult )
	{
		::SetWindowLongPtr( hWnd, DWLP_MSGRESULT, 0 );
		return TRUE;
	}

	return FALSE;
}
