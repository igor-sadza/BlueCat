// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef AFX_STDAFX_H
#define AFX_STDAFX_H

#pragma once

#if defined( _UNICODE ) && ! defined( UNICODE )
	#define UNICODE
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
	#define _CRT_SECURE_NO_DEPRECATE
#endif

#define WINVER       0x0500
#define _WIN32_WINNT 0x0500
#define _WIN32_IE    0x0500

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include "windows.h"
#pragma comment( lib, "kernel32.lib" )
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#include "commctrl.h"
#pragma comment( lib, "comctl32.lib" )

// C RunTime Header Files
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "tchar.h"

// Local Header Files

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // ! AFX_STDAFX_H
