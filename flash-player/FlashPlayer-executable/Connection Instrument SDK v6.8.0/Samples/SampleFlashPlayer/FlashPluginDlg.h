// FlashPluginDlg.h : header file
//
//{{AFX_INCLUDES()
#include "CShockwaveFlash.h"
//}}AFX_INCLUDES

#if !defined(AFX_FlashPluginDLG_H__C1CE2203_46EA_48CD_998E_36A9281AF88B__INCLUDED_)
#define AFX_FlashPluginDLG_H__C1CE2203_46EA_48CD_998E_36A9281AF88B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_INDICATOR_CONNECTION 1002039
#define ID_INDICATOR_FLASHFORMAT 1002040

#define CXC_VER 0x200 // we want new functions
#include "../../ConnectionTools/ConnectionClient.h"
#include "sensapi.h"

/////////////////////////////////////////////////////////////////////////////
// CFlashPluginDlg dialog

class CFlashPluginDlg : public CDialog
{
// Construction
public:
	CFlashPluginDlg(TCHAR *szCmdLine = NULL, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashPluginDlg)
	enum { IDD = IDD_FLASHPLUGIN_DIALOG };
	//}}AFX_DATA

    void SetMovie(CString csPath);
	void DoDisconnect();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashPluginDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFlashPluginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg void OnOK();
	afx_msg void OnDestroy();
    afx_msg void OnFileOpen();
    afx_msg void OnConnectToPC();
	afx_msg void OnHelpAbout();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileMenuDisconnect();
	afx_msg void OnFileMenuExit();
	afx_msg void OnFSCommandShockwaveflash(LPCTSTR command, LPCTSTR args);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DoConnect();
	UINT_PTR m_iTimer1;
	u_short m_uiPort;
	CStatusBar m_StatusBar;
	void UpdateStatusBar(bool bConnected);
	bool m_bConnected;
	long m_nClientID;
	long m_nSessionMode;
	long m_nSessionState;
	bool m_bGotClientID;
	bool m_bGotSessionMode;
	CString m_strFlashName;//name of the flash file that came with command line as argument.
	bool m_bStartedExternally;//true, if SampleFlashPlayer was started with provided flash(as command line argument)
	void ResetAllValues();
	CString m_strSignal[CXCEX_MAX_ACTIONS];
	CString m_strThreshold1[CXCEX_MAX_ACTIONS];
	CString m_strThreshold2[CXCEX_MAX_ACTIONS];
	CString m_strScaleMin[CXCEX_MAX_ACTIONS];
	CString m_strScaleMax[CXCEX_MAX_ACTIONS];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line

#endif // !defined(AFX_FlashPluginDLG_H__C1CE2203_46EA_48CD_998E_36A9281AF88B__INCLUDED_)
