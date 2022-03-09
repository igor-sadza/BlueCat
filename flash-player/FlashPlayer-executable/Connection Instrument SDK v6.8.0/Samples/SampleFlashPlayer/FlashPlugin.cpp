// FlashPlugin.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FlashPlugin.h"
#include "FlashPluginDlg.h"
#include "Validate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CFlashPluginApp

BEGIN_MESSAGE_MAP(CFlashPluginApp, CWinApp)
	//{{AFX_MSG_MAP(CFlashPluginApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashPluginApp construction

CFlashPluginApp::CFlashPluginApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFlashPluginApp object

CFlashPluginApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFlashPluginApp initialization

BOOL CFlashPluginApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

  /* 
	#ifdef _AFXDLL
	    Enable3dControls();			// Call this when using MFC in a shared DLL
    #else
	    Enable3dControlsStatic();	// Call this when linking to MFC statically
    #endif
*/
	Validate validateDlg;
	validateDlg.DoModal();


	CFlashPluginDlg dlg(m_lpCmdLine);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
