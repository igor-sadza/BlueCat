////////////////////////////////////////////////////////////////
// SelectComputer.cpp : implementation file
// Started by Pierre , continued by Mikhail.
// This dialog list all computers from selected workgroup/domain 
// on Local Area Network
/////////////////////////////////////////////////////////////////
 
#include "stdafx.h"

#include "SelectComputer.h"
//#include "winsock2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( push, 3 )
#include <iterator>
#include <algorithm>
#pragma warning( pop )

#define LINKFILE



/////////////////////////////////////////////////////////////////////////////
// CSelectComputerDialog dialog
CSelectComputerDialog::CSelectComputerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectComputerDialog::IDD, pParent)
{
   	//{{AFX_DATA_INIT(CSelectComputerDialog)
	m_bSelfConnect = FALSE;
	//}}AFX_DATA_INIT
}


void CSelectComputerDialog::DoDataExchange(CDataExchange* pDX)
{ 
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectComputerDialog)
	DDX_Control(pDX, IDC_SELF_CONNECT, m_btnSelfConnect);
	DDX_Control(pDX, IDC_LIST_COMPUTERS, m_lstComputers);
	DDX_Control(pDX, IDC_WORKGROUPS, m_cmbWorkgroups);
	DDX_Control(pDX, IDOK,	m_btnOK);
 	DDX_Control(pDX, IDC_COMPUTERS, m_btnListComputers);
	DDX_Check(pDX, IDC_SELF_CONNECT, m_bSelfConnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectComputerDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectComputerDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPUTERS, OnItemchangedList)
    ON_BN_CLICKED(IDC_COMPUTERS, OnListComputers)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_SELF_CONNECT, OnSelfConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectComputerDialog message handlers

void CSelectComputerDialog::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);

	UINT uiSelectedCount = m_lstComputers.GetSelectedCount();
	
	if(uiSelectedCount > 0)
		m_btnOK.EnableWindow(TRUE);
	else
		m_btnOK.EnableWindow(FALSE);

	*pResult = 0;
}
 
BOOL CSelectComputerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

		//create image list	
	m_imagelist.Create(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), ILC_COLOR, 0, 0);	
	//add two images to image list
	m_imagelist.Add(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_COMPUTER)));
	m_imagelist.Add(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_WORKGROUP)));
 
	CWaitCursor cur;
 
    if (!InitComboBox())
	{ 
		CString strMessage = _T("No Workgroups found on your local network!");
		MessageBox(strMessage, _T("SampleFlash Player"), MB_ICONEXCLAMATION);
	}

	CString strCaption;
	strCaption = _T("Computer Name");
	m_lstComputers.InsertColumn(0, strCaption, LVCFMT_LEFT, 460);
 
	m_lstComputers.SetImageList(&m_imagelist, LVSIL_SMALL);

 	InitCaptions();

	m_btnOK.EnableWindow(FALSE);  
	m_btnSelfConnect.SetCheck(BST_UNCHECKED);
	return 	TRUE;
}

void CSelectComputerDialog::InitCaptions()
{
	CString strCaption;
	CWnd * pControlWnd;

	SetWindowText(_T("Select Computer"));

	pControlWnd = GetDlgItem(IDC_SELF_CONNECT);
	pControlWnd->SetWindowText(_T("Connect to my computer "));

	pControlWnd = GetDlgItem(IDC_WORKGROUPS_LABEL);
	pControlWnd->SetWindowText(_T("Choose workgroup:"));

	pControlWnd = GetDlgItem(IDC_COMPUTERS);
	pControlWnd->SetWindowText(_T("List computers"));

	pControlWnd = GetDlgItem(IDOK);		
	pControlWnd->SetWindowText(_T("OK"));
	
	pControlWnd = GetDlgItem(IDCANCEL);	
	pControlWnd->SetWindowText(_T("Cancel"));
	 
}
bool CSelectComputerDialog::FillComputers(CString strWorkgroup)
{
	const basic_string<TCHAR>& strDomain = static_cast<LPCTSTR>(strWorkgroup);

	m_lstComputers.DeleteAllItems();

	//we need all users from all domains
	vector<basic_string<TCHAR> >::const_iterator bci, eci;
	bci = eci = find(m_vecstrDomains.begin(), m_vecstrDomains.end(), strDomain);
	++eci;
 
	for (; bci != eci; ++bci)
	{
		m_vecstrServers.clear();

		bool bResult = m_Network.GetServers(*bci, m_vecstrServers);
		if(!bResult)
			return false;

		//////////////////////////////////////////////////////////////////////////////	
		//Insert items (user names) into list view
		for (std::vector<basic_string<TCHAR> >::const_iterator ci = m_vecstrServers.begin();
			ci != m_vecstrServers.end(); ++ci)
		{
			m_lstComputers.InsertItem(m_lstComputers.GetItemCount(), ci->c_str(), 0);
			m_lstComputers.SetItemText(m_lstComputers.GetItemCount() - 1, 1, bci->c_str());
		}
		//~Insert items (user names)  into list view
	}
	return true;
}

void CSelectComputerDialog::OnListComputers() 
{
	CString strWorkgroup;
	m_btnOK.EnableWindow(FALSE);  
 	if( m_cmbWorkgroups.GetCurSel() == CB_ERR)//or -1 i.e. nothing is selected.
		return;
	m_cmbWorkgroups.GetLBText(m_cmbWorkgroups.GetCurSel(), strWorkgroup);
	CWaitCursor cur;
	if(!FillComputers(strWorkgroup))
	{
		CString strTemp = _T("Could not list computers.There is a problem with the network.\nPlease, contact your network administrator.");
 		MessageBox(strTemp, _T("SampleFlash Player"), MB_ICONEXCLAMATION);
	}
}
 
bool CSelectComputerDialog::InitComboBox()
{
 	bool bResult = m_Network.GetDomains(m_vecstrDomains);
	if(!bResult)
		return false;

 	//set image list for combobox control
	m_cmbWorkgroups.SetImageList(&m_imagelist);

	COMBOBOXEXITEM cbItem;
	cbItem.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	cbItem.iImage = cbItem.iSelectedImage = 1;
	cbItem.iItem = -1;
 
	if (m_vecstrDomains.empty())
		return false;
	
//Add Items (domains names) into ComboBox 
	for (vector<basic_string<TCHAR> >::const_iterator ci = m_vecstrDomains.begin(); 
		 ci != m_vecstrDomains.end(); ++ci)
	{
		cbItem.pszText = const_cast<LPTSTR>(ci->c_str());
		m_cmbWorkgroups.InsertItem(&cbItem);
	}
//~Add Items (domains names) into ComboBox
	
///////////////////////////////////////////////////////////////////////////////
//Set Selection in combo box item to the First(0) Item ..
	m_cmbWorkgroups.SetCurSel(0);

	return true;
}

bool CSelectComputerDialog::GetIPAddress(TIPAddress* ptIPAddr)
{
	
    if (!ptIPAddr) return false;
    memset(ptIPAddr, NULL, sizeof(ptIPAddr));
 
    if (m_bSelfConnect) 
    {	//127.0.0.1 is the standard IP address used for a loopback network connection.By other words, it is me!!!
        strcpy_s(ptIPAddr->strIP, 10, "127.0.0.1");
        gethostname(ptIPAddr->strComputerName, 256);
        ptIPAddr->a1 = 127;
        ptIPAddr->a2 = 0;
        ptIPAddr->a3 = 0;
        ptIPAddr->a4 = 1;
        return true;
    }
        
    struct hostent *host;
    struct in_addr *ptr; // To retrieve the IP Address 

    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1),&wsaData);

    //get computer name
    vector<pair<CString, CString> >::const_iterator ci = GetComputerNames().begin();

    //Write IP
    char strHostName[256];
    char strDomain[256];
	memset(strHostName, NULL, 20);
	memset(strDomain, NULL, 20);
	size_t Converted;//number of char converted
#ifdef _UNICODE
    //wcstombs(strHostName, ci->first, max(ci->first.GetLength(), 256));
	 wcstombs_s(&Converted, strHostName, 256, ci->first, 256);
//    wcstombs(strDomain, ci->second, max(ci->second.GetLength(), 256));
	 wcstombs_s(&Converted,strDomain, 256, ci->second, 256);
#else
    CString strTmp = ci->first;
	strcpy_s(strHostName, strTmp.GetLength()+1, strTmp.GetBuffer(0));
    strTmp = ci->second;
    strcpy_s(strDomain, strTmp.GetLength()+1, strTmp.GetBuffer(0));
#endif

    host = gethostbyname(strHostName);
    if(host == NULL) return false;

    ptr = (struct in_addr *)host->h_addr_list[0];                    

    // Eg. 211.40.35.76 split up like this.             
    ptIPAddr->a1 = ptr->S_un.S_un_b.s_b1;  // 211           
    ptIPAddr->a2 = ptr->S_un.S_un_b.s_b2;  // 40
    ptIPAddr->a3 = ptr->S_un.S_un_b.s_b3;  // 35
    ptIPAddr->a4 = ptr->S_un.S_un_b.s_b4;  // 76

    sprintf_s(ptIPAddr->strIP, 20, "%d.%d.%d.%d", 
        ptIPAddr->a1,ptIPAddr->a2,ptIPAddr->a3,ptIPAddr->a4);

    strcpy_s(ptIPAddr->strComputerName,_countof(strHostName), strHostName);
    strcpy_s(ptIPAddr->strDomain,_countof(strDomain), strDomain);

    WSACleanup();

    return true;

}

void CSelectComputerDialog::OnOK() 
{
	m_bSelfConnect= m_btnSelfConnect.GetCheck() == 1 ? TRUE : FALSE;

	if(!m_bSelfConnect)
	{
 		UINT uiSelectedCount = m_lstComputers.GetSelectedCount();
		if (uiSelectedCount >0)
		{
			m_Names.clear();
			POSITION pos = m_lstComputers.GetFirstSelectedItemPosition();
			while (pos)
			{
				const int nItem = m_lstComputers.GetNextSelectedItem(pos);
				m_Names.push_back(make_pair( m_lstComputers.GetItemText(nItem, 0), m_lstComputers.GetItemText(nItem, 1)));
			}
		}
	}
	CDialog::OnOK();
}

void CSelectComputerDialog::OnSelfConnect() 
{
	CWnd * pControlWnd;

	if(m_btnSelfConnect.GetCheck())
	{// connect to myself
		m_btnOK.EnableWindow(TRUE);

 		pControlWnd = GetDlgItem(IDC_WORKGROUPS_LABEL);
		pControlWnd->EnableWindow(FALSE);
 		
		pControlWnd = GetDlgItem(IDC_COMPUTERS);
		pControlWnd->EnableWindow(FALSE);

		pControlWnd = GetDlgItem(IDC_LIST_COMPUTERS);
		pControlWnd->EnableWindow(FALSE);

		pControlWnd = GetDlgItem(IDC_WORKGROUPS);
		pControlWnd->EnableWindow(FALSE);
	}
	else // select from network
	{
		if(m_lstComputers.GetSelectedCount() > 0)//if we have any selection
			m_btnOK.EnableWindow(TRUE);
		else
			m_btnOK.EnableWindow(FALSE);

		pControlWnd = GetDlgItem(IDC_WORKGROUPS_LABEL);
		pControlWnd->EnableWindow(TRUE);
 		
		pControlWnd = GetDlgItem(IDC_COMPUTERS);
		pControlWnd->EnableWindow(TRUE);

		pControlWnd = GetDlgItem(IDC_LIST_COMPUTERS);
		pControlWnd->EnableWindow(TRUE);

		pControlWnd = GetDlgItem(IDC_WORKGROUPS);
		pControlWnd->EnableWindow(TRUE);

	}	
}
 
  
 