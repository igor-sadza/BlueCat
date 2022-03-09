// TestClientDlg.h : header file
//

#if !defined(AFX_TESTCLIENTDLG_H__C0FC5773_C1EE_4FDD_BC5C_03DEB3B9768E__INCLUDED_)
#define AFX_TESTCLIENTDLG_H__C0FC5773_C1EE_4FDD_BC5C_03DEB3B9768E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg dialog

class CTestClientDlg : public CDialog
{
private:
	CString m_strDestinationIP;
	void DoConnect();
	void AddTextInLog(const CString strText);
	void UpdateStatusBar(bool bConnected);
	bool m_bConnected;
	CStatusBar m_StatusBar;
	CString m_strConnectedServerIP;
	CString GetPCNamefromIP(CString strIP);
	u_short m_uiPort;
 // Construction
public:
	CTestClientDlg(CWnd* pParent = NULL);	// standard constructor
	CEdit m_editSignals[8];
	CEdit m_editMin[8];
	CEdit m_editMax[8];
	CEdit m_editThresh1[8];
	CEdit m_editThresh2[8];
	CComboBox m_cbAction[8];
	CEdit m_editSessionStatus;
	CEdit m_editLogMessages;
	UINT_PTR m_iTimerFrameRate;//main timer to update all data
// Dialog Data
	//{{AFX_DATA(CTestClientDlg)
	enum { IDD = IDD_TESTCLIENT_DIALOG };
	CIPAddressCtrl	m_IPAddress;
	int		m_nBiographLocation;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClearMessages();
	afx_msg void OnBnClickedReConnect();
	afx_msg void OnLocalNetwork();
	afx_msg void OnSamePC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDisconnect();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCLIENTDLG_H__C0FC5773_C1EE_4FDD_BC5C_03DEB3B9768E__INCLUDED_)
