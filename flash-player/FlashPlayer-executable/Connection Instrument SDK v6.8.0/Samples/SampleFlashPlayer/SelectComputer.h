#if !defined(AFX_SELECTCOMPUTERDIALOG_H__E58DE999_B923_409B_87EA_39C71827BC95__INCLUDED_)
#define AFX_SELECTCOMPUTERDIALOG_H__E58DE999_B923_409B_87EA_39C71827BC95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectComputer.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSelectComputerDialog dialog
#include "resource.h"

#include "SelectNetwork.h"
//std lib
#pragma warning( push, 3 )
#include <vector>
#include <string>
#include <utility>
#pragma warning( pop )
//~std lib

using namespace std;

struct TIPAddress
{
    char strComputerName[256];
    char strDomain[256];

    char strIP[20];
    int a1, a2, a3, a4;
};


/////////////////////////////////////////////////////////////////////////////
// CSelectComputerDialog dialog

class CSelectComputerDialog : public CDialog
{
// Construction
public:
	CSelectComputerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectComputerDialog)
	enum { IDD = IDD_SELECT_COMPUTER };
	CButton		m_btnSelfConnect;
	CListCtrl	m_lstComputers;
	CComboBoxEx	m_cmbWorkgroups;
	CButton		m_btnOK;
 	CButton		m_btnListComputers; 
	BOOL		m_bSelfConnect;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectComputerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectComputerDialog)
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnListComputers();
	virtual void OnOK();
	afx_msg void OnSelfConnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const pair<CString, CString> & GetComputerName() const
	{
		return m_Names[0];
	}

	const vector<pair<CString, CString> >& GetComputerNames() const
	{
		return m_Names;
	}

    bool GetIPAddress(TIPAddress* pIPAddr);

private:
	vector<pair<CString, CString> > m_Names;
  
	bool FillComputers(CString strWorkgroup);
 	void InitCaptions();
	bool InitComboBox();
   	vector<basic_string<TCHAR> > m_vecstrDomains;
	vector<basic_string<TCHAR> > m_vecstrServers;

	CSelectComputerNetwork		m_Network;
	CImageList					m_imagelist;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCOMPUTERDIALOG_H__E58DE999_B923_409B_87EA_39C71827BC95__INCLUDED_)
