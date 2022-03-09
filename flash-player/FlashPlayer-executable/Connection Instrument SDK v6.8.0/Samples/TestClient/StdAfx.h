// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AF15C913_CD58_4E31_AA9C_381A80CEB553__INCLUDED_)
#define AFX_STDAFX_H__AF15C913_CD58_4E31_AA9C_381A80CEB553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#include <atlbase.h> // for USES_CONVERSION

#if _MSC_VER < 1400 // before VC2005:
	// Mark would like to be able to compile with VS6 if necessary
	#ifdef _UNICODE
	#pragma comment( linker, "/ENTRY:wWinMainCRTStartup" )
	#endif
#else // 1400 <= _MSC_VER (VC2005)
#endif // 1400 <= _MSC_VER

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AF15C913_CD58_4E31_AA9C_381A80CEB553__INCLUDED_)
