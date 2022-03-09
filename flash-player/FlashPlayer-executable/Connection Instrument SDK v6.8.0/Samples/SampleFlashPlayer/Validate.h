#pragma once

#include "stdafx.h"
#include "resource.h" 
#include <afxdialogex.h>
// Validate dialog

class Validate : public CDialogEx
{
	DECLARE_DYNAMIC(Validate)

public:
	Validate(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Validate();
	virtual BOOL OnInitDialog();
	virtual BOOL ValidateRegistry();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VALIDATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	TCHAR* getCpuID();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static_validate_text;
	CStatic m_static_cpu_id;
	CEdit	m_edit_validate;
	CEdit	m_edit_static_processor_id;
	CString	m_user_input;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditValidate();
	
	CButton m_button_ok;
	afx_msg void OnBnClickedOk();
};
