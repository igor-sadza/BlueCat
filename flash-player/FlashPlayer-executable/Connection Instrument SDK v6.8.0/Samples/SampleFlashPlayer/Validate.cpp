// Validate.cpp : implementation file
//

#include "Validate.h"
#include "afxdialogex.h"

#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

TCHAR* Validate::getCpuID()
{
	unsigned long s1 = 0;
	unsigned long s2 = 0;
	unsigned long s3 = 0;
	unsigned long s4 = 0;
	__asm
	{
		mov eax, 00h
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	__asm
	{
		mov eax, 01h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s3, edx
		mov s4, ecx
	}

	static char buf[100];
	sprintf_s(buf, "%08X%08X%08X%08X", s1, s2, s3, s4);
	return buf;
}

// Validate dialog

IMPLEMENT_DYNAMIC(Validate, CDialogEx)

Validate::Validate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VALIDATE_DIALOG, pParent)
{

}

Validate::~Validate()
{
}

void Validate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VALIDATE_TEXT, m_static_validate_text);
	DDX_Control(pDX, IDC_EDIT_VALIDATE, m_edit_validate);
	DDX_Control(pDX, IDC_EDIT_BOX_PROCESSOR_ID, m_edit_static_processor_id);
	DDX_Control(pDX, IDOK, m_button_ok);
}

BOOL Validate::OnInitDialog() {

	CDialogEx::OnInitDialog();

	m_edit_static_processor_id.SetWindowText(_bstr_t("CLIENT ID:  ") + _bstr_t(getCpuID()));
	m_static_validate_text.SetWindowTextA("PRODUCT SERIAL KEY:");

	if (this->ValidateRegistry()) {
		EndDialog(0);
	}

	return TRUE;
}

inline void TESTHR(HRESULT x) { if FAILED(x) _com_issue_error(x); };

BOOL Validate::ValidateRegistry() {
	bool bResult_fullActivated(false),
		bResult_isRegisteredMatch(false),
		bResult_isProductKeyMatch(false),
		bResult_isProcessorIDMatch(false);

	HRESULT hr = S_OK;
	CRegKey cKey;
	_bstr_t connectionString("driver={SQL Server};server=SERWER,PORT;uid=USER;pwd=PASSWORD;database=IS-DEV");
	_RecordsetPtr pRstActivated = NULL;

	_bstr_t sql_key_set_registered = ("UPDATE dbo.ISFlashPlayer SET registered ='1' WHERE product_key = '" + _bstr_t(m_user_input) + "';");
	_bstr_t sql_key_set_date = ("UPDATE dbo.ISFlashPlayer SET register_date ='" + _bstr_t(CTime::GetCurrentTime().Format("%d-%m-%Y")) + "' WHERE product_key = '" + _bstr_t(m_user_input) + "';");
	_bstr_t sql_key_set_processor_id = ("UPDATE dbo.ISFlashPlayer SET processor_id = '" + _bstr_t(getCpuID()) + "' WHERE product_key = '" + _bstr_t(m_user_input) + "';");

	_ConnectionPtr pConnection = NULL;
	_CommandPtr pCMD = NULL;

	TESTHR(pConnection.CreateInstance(__uuidof(Connection)));
	pConnection->Open(connectionString, "", "", adConnectUnspecified);

	TESTHR(pCMD.CreateInstance(__uuidof(Command)));
	pCMD->ActiveConnection = pConnection;

	hr = pRstActivated.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr)) {
		MessageBox("Failed creating record set instance\n", "Error");
		return 0;
	}

	_bstr_t result;
	DWORD dwValue = 0;
	char pszProductKey[100];
	ULONG pszValueProductKey = _countof(pszProductKey);
	if (ERROR_SUCCESS == cKey.Open(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"))) {

		if (ERROR_SUCCESS == cKey.QueryStringValue(_T("product_key"), pszProductKey, &pszValueProductKey)) {
			pRstActivated->Open("SELECT * FROM dbo.ISFlashPlayer WHERE product_key ='" + _bstr_t(pszProductKey) + "';"
				, connectionString
				, adOpenStatic
				, adLockReadOnly
				, adCmdText);
		}
		else {
			pRstActivated->Open("SELECT * FROM dbo.ISFlashPlayer WHERE product_key ='" + _bstr_t(m_user_input) + "';"
				, connectionString
				, adOpenStatic
				, adLockReadOnly
				, adCmdText);
		}

	}
	else {
		pRstActivated->Open("SELECT * FROM dbo.ISFlashPlayer WHERE product_key ='" + _bstr_t(m_user_input) + "';"
			, connectionString
			, adOpenStatic
			, adLockReadOnly
			, adCmdText);
	}
	//pRstActivated->MoveFirst();

	if (!pRstActivated->EndOfFile)
	{
		while (!pRstActivated->EndOfFile)
		{
			char pszProductKey[100];
			ULONG pszValueProductKey = _countof(pszProductKey);
			if (ERROR_SUCCESS == cKey.Open(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"))) {
				cKey.QueryStringValue(_T("product_key"), pszProductKey, &pszValueProductKey);
				if (pszProductKey != "") {
					result = pRstActivated->Fields->GetItem("product_key")->Value;
					if (result == _bstr_t(pszProductKey)) {
						bResult_isProductKeyMatch = true;
					}
				}
			}

			char pszRegistered[100];
			ULONG nValueLengthRegistered = _countof(pszRegistered);
			if (ERROR_SUCCESS == cKey.Open(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"))) {
				cKey.QueryStringValue(_T("registered"), pszRegistered, &nValueLengthRegistered);
				if (pszRegistered != "") {
					result = pRstActivated->Fields->GetItem("registered")->Value;
					if (result == _bstr_t(pszRegistered)) {
						bResult_isRegisteredMatch = true;
					}
					else {
						cKey.SetStringValue(_T("registered"), _T("1"));
						bResult_isRegisteredMatch = true;
					}
				}
			}

			char pszProcessorID[100];
			ULONG pszValueProcessorID = _countof(pszProcessorID);
			if (ERROR_SUCCESS == cKey.Open(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"))) {
				cKey.QueryStringValue(_T("processor_id"), pszProcessorID, &pszValueProcessorID);
				if (pszProcessorID != "") {
					result = pRstActivated->Fields->GetItem("processor_id")->Value;
					if (result == _bstr_t(pszProcessorID)) {
						bResult_isProcessorIDMatch = true;
					}
					else {
						cKey.SetStringValue(_T("processor_id"), _T("1"));
						bResult_isProcessorIDMatch = true;
					}
				}
			}

			if (bResult_isProductKeyMatch && bResult_isRegisteredMatch && bResult_isProcessorIDMatch) {
				return true;
			}

			//First check product key is valid
			result = pRstActivated->Fields->GetItem("product_key")->Value;
			if (result == _bstr_t(m_user_input)) {
				//Check producet keys is registered
				result = pRstActivated->Fields->GetItem("registered")->Value;
				if (result == _bstr_t("0")) {
					//IF everything is ok start registration
					result = pRstActivated->Fields->GetItem("product_key")->Value;
					cKey.Create(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"));
					cKey.SetStringValue(_T("product_key"), _T(result));
					//Set registered form 0 to 1
					//Change in sql
					pCMD->CommandText = sql_key_set_registered;
					pCMD->Execute(NULL, NULL, adCmdText);

					//Change in local registry
					cKey.Create(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"));
					cKey.SetStringValue(_T("registered"), _T("1"));

					//Set registered date
					//Change in sql
					pCMD->CommandText = sql_key_set_date;
					pCMD->Execute(NULL, NULL, adCmdText);

					//Change in local registry
					cKey.Create(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"));
					cKey.SetStringValue(_T("register_date"), _T(CTime::GetCurrentTime().Format("%d-%m-%Y")));

					//Set processor ID
					//Change in sql
					pCMD->CommandText = sql_key_set_processor_id;
					pCMD->Execute(NULL, NULL, adCmdText);

					//Change in local registry
					cKey.Create(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"));
					cKey.SetStringValue(_T("processor_id"), _T(getCpuID()));

					MessageBox("The product activated!", "Information");
					bResult_fullActivated = true;
				}
				else {
					MessageBox("The product key is used!", "Information");
					bResult_fullActivated = false;
				}
			}
			pRstActivated->MoveNext();
		}
	}
	else {
		if (ERROR_SUCCESS == cKey.Open(HKEY_CURRENT_USER, _T("Software\\Thought Technology\\ISFlashPlayer"))) {
			cKey.DeleteValue("product_key");
			bResult_fullActivated = false;
		}
	}
	return bResult_fullActivated;
}

BEGIN_MESSAGE_MAP(Validate, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &Validate::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_VALIDATE, &Validate::OnEnChangeEditValidate)
	ON_BN_CLICKED(IDOK, &Validate::OnBnClickedOk)
END_MESSAGE_MAP()


// Validate message handlers


void Validate::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	PostQuitMessage(0);
}


void Validate::OnEnChangeEditValidate()
{

	m_edit_validate.GetWindowText(m_user_input);

	if (ValidateRegistry()) EndDialog(0);
}

void Validate::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here	
	if (this->ValidateRegistry()) {
		CDialogEx::OnOK();
	}
}
