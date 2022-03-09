// FlashPlugin.h : main header file for the FlashPlugin application
//

#if !defined(AFX_FlashPlugin_H__AD5E8CC9_3E48_4FAE_80C7_EFAA67BBDD68__INCLUDED_)
#define AFX_FlashPlugin_H__AD5E8CC9_3E48_4FAE_80C7_EFAA67BBDD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFlashPluginApp:
// See FlashPlugin.cpp for the implementation of this class
//

class CFlashPluginApp : public CWinApp
{
public:
	CFlashPluginApp();
	HICON m_hIcon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashPluginApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFlashPluginApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FlashPlugin_H__AD5E8CC9_3E48_4FAE_80C7_EFAA67BBDD68__INCLUDED_)
