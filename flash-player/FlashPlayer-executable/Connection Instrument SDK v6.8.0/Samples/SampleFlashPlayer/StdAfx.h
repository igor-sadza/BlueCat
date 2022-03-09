// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C065CD79_1BF7_46B1_9AE8_8733D758B3D2__INCLUDED_)
#define AFX_STDAFX_H__C065CD79_1BF7_46B1_9AE8_8733D758B3D2__INCLUDED_

#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef WINVER
#define WINVER 0x0502
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxdisp.h>

#import "msado15.dll" no_namespace rename("EOF", "EndOfFile")
#import "Flash11e.ocx" no_namespace rename("EOF", "EndOfFile")

#if _MSC_VER < 1400 // before VC2005:
	// Mark would like to be able to compile with VS6 if necessary
	#pragma warning( disable : 4786 ) // identifier was truncated to '255' characters
	#define wcstombs_s(pr,d,nd,s,ns) ((pr),wcstombs(d,s,ns))
	#define _tcscpy_s(d,n,s) _tcsncpy(d,s,n)
	#define strcpy_s(d,n,s) strncpy(d,s,n)
	#define sprintf_s _snprintf
	#define _countof(a) ((sizeof a)/(sizeof a[0]))
	#ifdef _UNICODE
	#pragma comment( linker, "/ENTRY:wWinMainCRTStartup" )
	#endif
#else // 1400 <= _MSC_VER (VC2005)
#endif // 1400 <= _MSC_VER

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C065CD79_1BF7_46B1_9AE8_8733D758B3D2__INCLUDED_)
