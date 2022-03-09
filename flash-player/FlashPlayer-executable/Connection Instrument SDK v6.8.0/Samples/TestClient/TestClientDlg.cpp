/****************************************************************************
*
* Company:  Thought Technology Ltd.
* Website:	http://www.thoughttechnology.com 
* Date:		July 20, 2010
*
* Project:	Biograph  Infiniti: ConnectionInstrument SDK
*
* Purpose:	This application is a sample on how to use ConnectionClient.dll. 
*			ConnectionClient.dll provides network connection between this
*			application and Connection Instrument from Biorgaph.
*			You can use up to 64 action connections(channles/actions)  to connect to 
*			Connection Instrument.
*			In this sample I create only 8 Edit controls to display data for 8 actions at a time. 
*
*****************************************************************************/

// TestClientDlg.cpp : implementation file
//
#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"

#if 1
#define CXC_VER 0x300 // we want new functions
#endif
#include "../../ConnectionTools/ConnectionClient.h"

#ifdef _M_X64
    #ifndef NDEBUG
        #pragma comment( lib, "../../ConnectionTools/x64/Debug/ConnectionClientD.lib" )
    #else
        #pragma comment( lib, "../../ConnectionTools/x64/Release/ConnectionClient.lib" )
    #endif
#else
    #ifndef NDEBUG
        #pragma comment( lib, "../../ConnectionTools/Debug/ConnectionClientD.lib" )
    #else
        #pragma comment( lib, "../../ConnectionTools/Release/ConnectionClient.lib" )
    #endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////
// Application specific

enum
{
	FRAME_RATE = 16 // Hz
,	FRAME_PERIOD = 1000 / FRAME_RATE // ms
};
#define FRAME_RATE_TIMER		1
	
enum { MAX_BUFFER = CXC_MAX_SAMPLING_RATE * 4 }; 

CXC_ACTION_FORMAT g_aActionFormats[ CXCEX_MAX_ACTIONS ] = { 0, };
CXC_ACTION_STATE g_aActionStates[ CXCEX_MAX_ACTIONS ] = { 0, };
CXC_SAMPLE g_aActionData[CXCEX_MAX_ACTIONS][MAX_BUFFER];
int g_aInSamples[CXCEX_MAX_ACTIONS] = { 0, };

CString g_strDefaultServer = _T("127.0.0.1");

#define GREEN_COLOR			RGB(117,226,4)
#define RED_COLOR			RGB(255,0,0)

#define ID_INDICATOR_1 1002039
#define ID_INDICATOR_2 1002040

#define STATUS_BAR_OUTPUT_PANES 2 //number of output panes(indicators) for a status bar.
static UINT BASED_CODE indicators[] =
{
    ID_INDICATOR_1,
    ID_INDICATOR_2
};
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg dialog

CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestClientDlg)
	m_nBiographLocation = 1;
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//may be get ip of local pc here?
	m_strDestinationIP = "192.168.1.101";
	m_iTimerFrameRate = 0;
	m_bConnected = false;
	m_strConnectedServerIP = _T("");
	m_uiPort = 1000;
	//}}AFX_DATA_INIT
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestClientDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_CONNECTION_LOG, m_editLogMessages);

	DDX_Control(pDX, IDC_SIGNAL_0, m_editSignals[0]);
	DDX_Control(pDX, IDC_SIGNAL_1, m_editSignals[1]);
	DDX_Control(pDX, IDC_SIGNAL_2, m_editSignals[2]);
	DDX_Control(pDX, IDC_SIGNAL_3, m_editSignals[3]);
	DDX_Control(pDX, IDC_SIGNAL_4, m_editSignals[4]);
	DDX_Control(pDX, IDC_SIGNAL_5, m_editSignals[5]);
	DDX_Control(pDX, IDC_SIGNAL_6, m_editSignals[6]);
	DDX_Control(pDX, IDC_SIGNAL_7, m_editSignals[7]);

	DDX_Control(pDX, IDC_THRSH1_0, m_editThresh1[0]);
	DDX_Control(pDX, IDC_THRSH1_1, m_editThresh1[1]);
	DDX_Control(pDX, IDC_THRSH1_2, m_editThresh1[2]);
	DDX_Control(pDX, IDC_THRSH1_3, m_editThresh1[3]);
	DDX_Control(pDX, IDC_THRSH1_4, m_editThresh1[4]);
	DDX_Control(pDX, IDC_THRSH1_5, m_editThresh1[5]);
	DDX_Control(pDX, IDC_THRSH1_6, m_editThresh1[6]);
	DDX_Control(pDX, IDC_THRSH1_7, m_editThresh1[7]);

	DDX_Control(pDX, IDC_THRSH2_0, m_editThresh2[0]);
	DDX_Control(pDX, IDC_THRSH2_1, m_editThresh2[1]);
	DDX_Control(pDX, IDC_THRSH2_2, m_editThresh2[2]);
	DDX_Control(pDX, IDC_THRSH2_3, m_editThresh2[3]);
	DDX_Control(pDX, IDC_THRSH2_4, m_editThresh2[4]);
	DDX_Control(pDX, IDC_THRSH2_5, m_editThresh2[5]);
	DDX_Control(pDX, IDC_THRSH2_6, m_editThresh2[6]);
	DDX_Control(pDX, IDC_THRSH2_7, m_editThresh2[7]);

	DDX_Control(pDX, IDC_MIN_0, m_editMin[0]);
	DDX_Control(pDX, IDC_MIN_1, m_editMin[1]);
	DDX_Control(pDX, IDC_MIN_2, m_editMin[2]);
	DDX_Control(pDX, IDC_MIN_3, m_editMin[3]);
	DDX_Control(pDX, IDC_MIN_4, m_editMin[4]);
	DDX_Control(pDX, IDC_MIN_5, m_editMin[5]);
	DDX_Control(pDX, IDC_MIN_6, m_editMin[6]);
	DDX_Control(pDX, IDC_MIN_7, m_editMin[7]);

	DDX_Control(pDX, IDC_MAX_0, m_editMax[0]);
	DDX_Control(pDX, IDC_MAX_1, m_editMax[1]);
	DDX_Control(pDX, IDC_MAX_2, m_editMax[2]);
	DDX_Control(pDX, IDC_MAX_3, m_editMax[3]);
	DDX_Control(pDX, IDC_MAX_4, m_editMax[4]);
	DDX_Control(pDX, IDC_MAX_5, m_editMax[5]);
	DDX_Control(pDX, IDC_MAX_6, m_editMax[6]);
	DDX_Control(pDX, IDC_MAX_7, m_editMax[7]);

	DDX_Control(pDX, IDC_COMBO0, m_cbAction[0]);
	DDX_Control(pDX, IDC_COMBO1, m_cbAction[1]);
	DDX_Control(pDX, IDC_COMBO2, m_cbAction[2]);
	DDX_Control(pDX, IDC_COMBO3, m_cbAction[3]);
	DDX_Control(pDX, IDC_COMBO4, m_cbAction[4]);
	DDX_Control(pDX, IDC_COMBO5, m_cbAction[5]);
	DDX_Control(pDX, IDC_COMBO6, m_cbAction[6]);
	DDX_Control(pDX, IDC_COMBO7, m_cbAction[7]);

	DDX_Control(pDX, IDC_SESSION_STATUS, m_editSessionStatus);
	DDX_Radio(pDX, IDC_NETWORK, m_nBiographLocation);
	//}}AFX_DATA_MAP
}
void  CTestClientDlg::AddTextInLog(const CString strText)
{
	m_editLogMessages.SetSel(m_editLogMessages.GetWindowTextLength(), m_editLogMessages.GetWindowTextLength());
 	m_editLogMessages.ReplaceSel(strText);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialog)
	//{{AFX_MSG_MAP(CTestClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLEAR_MESSAGES, OnClearMessages)
	ON_BN_CLICKED(IDC_CONNECT, OnBnClickedReConnect)
	ON_BN_CLICKED(IDC_NETWORK, OnLocalNetwork)
	ON_BN_CLICKED(IDC_SAME_PC, OnSamePC)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DISCONNECT, CTestClientDlg::OnBnClickedDisconnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg message handlers

BOOL CTestClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	/*
		USAGE cxcInitialize
	*/
    BOOL ret = cxcInitialize( CXC_VER );//we must call cxcInitialize first!
	if( !ret )
	{
		AfxMessageBox(_T("cxcInitialize Failed."));//probably ws2_32.dll is missing
		EndDialog(IDCANCEL);
		return TRUE;// "Fatal error initializing connection client."
	}

	m_IPAddress.SetWindowText(m_strDestinationIP);//default

 // Create status bar
    m_StatusBar.Create(this);
    m_StatusBar.SetIndicators(indicators,STATUS_BAR_OUTPUT_PANES);

    CRect rect;
    GetClientRect(&rect);
    //Size the two panes
    m_StatusBar.SetPaneInfo(0,ID_INDICATOR_1,SBPS_NORMAL,rect.Width() - 300);      
    m_StatusBar.SetPaneInfo(1,ID_INDICATOR_2,SBPS_STRETCH ,0);
    m_StatusBar.GetStatusBarCtrl().SetBkColor(0xD8E9EC);//default color of the dialog.

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_2);

	if(m_nBiographLocation == 0)
		OnLocalNetwork();
	else
		OnSamePC() ;

	//populate combo boxes:
	CString strTemp;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < CXCEX_MAX_ACTIONS; j++)
		{
			strTemp.Format(_T("%d"),j)	;
			m_cbAction[i].AddString(strTemp);
			m_cbAction[i].SetItemData(j,j);
		}
		m_cbAction[i].SetCurSel(i);
	}

	SetWindowPos( & wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );
	ZeroMemory (g_aActionData, sizeof (g_aActionData));

	DoConnect(); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//this is "EXIT" button!
void CTestClientDlg::OnOK() 
{
	OnBnClickedDisconnect();
	
	/*
		USAGE cxcTerminate
	*/
	cxcTerminate();

	::DestroyWindow(m_StatusBar);

	CDialog::OnOK();
}
void CTestClientDlg::OnCancel()
{
	//Do not exit on escape or on alt+f4. 
	//MUST CLICK ON BUTTON "Exit". 
	//So just we come to the same function in all cases
	//CDialog::OnCancel();
}

void CTestClientDlg::OnClearMessages() 
{
	ZeroMemory( g_aActionFormats, sizeof g_aActionFormats );
	ZeroMemory( g_aActionStates, sizeof g_aActionStates );

	for(int i = 0; i < 8; i++)
	{
		m_editSessionStatus.SetWindowText(_T(" "));

		m_editSignals[i].SetSel(0, -1);
		m_editSignals[i].Clear();
		m_editMin[i].SetSel(0, -1);
		m_editMin[i].Clear();
		m_editMax[i].SetSel(0, -1);
		m_editMax[i].Clear();
		m_editThresh1[i].SetSel(0, -1);
		m_editThresh1[i].Clear();
		m_editThresh2[i].SetSel(0, -1);
		m_editThresh2[i].Clear();
	}
	
	m_editLogMessages.SetSel(0, -1);
	m_editLogMessages.Clear();
}

void CTestClientDlg::DoConnect() 
{
	KillTimer(m_iTimerFrameRate);
	// By default, we try to connect  to 127.0.0.1(loopback device).
    // It means that both the producer and consumer are running on the same computer.
	// Keep in mind, that Biograph acts as a server.
	// That means that if you close the game and then open it, connections will be restored automatically.
	// But if you close Biograph and then open it again and connection with the game will be lost and you have to restart your game(i.e recreate your ConnectionServer again).
	CString strTargetIP;
	if(m_nBiographLocation == 0)//Network
	{
		//read IP from dialog
		m_IPAddress.GetWindowText(m_strDestinationIP);
		strTargetIP = m_strDestinationIP;
	}
	else // same PC
		strTargetIP = g_strDefaultServer;//loop back ip

	AddTextInLog(_T("Connecting to Server...\r"));
	/*
	USAGE cxcSetPort
	*/
	cxcSetPort( m_uiPort );//if we do not do it , default port used by Server is 1000. But it could be also changed for Server.Read SDK for more info.
	/*
	USAGE cxcConnect
	*/
	BOOL bRetVal;
	CWaitCursor cw;//Show Wait Cursor,as if connecting to remote host, it could take some time.
	bRetVal = cxcConnect(strTargetIP);
	if(!bRetVal)
	{
		DWORD dwError = GetLastError();
		if(dwError == CXC_ERROR_NOTINITIALIZED)
			AddTextInLog(_T("\nERROR! Either Missing or Failed call to cxcInitialize() !\r\n"));
		else
			AddTextInLog(_T("\nERROR! Could not connect to Server!\r\nProbably it is not running or destination IP is not correct!\r\nTry to start Server or enter correct IP and then try to Reconnect !\r\n"));
		m_bConnected = false;
		UpdateStatusBar(m_bConnected);
		m_strConnectedServerIP.Empty();
		return;
	}
	m_strConnectedServerIP = strTargetIP;
	m_bConnected = true;
	AddTextInLog(_T("Connected!\r\n"));
	AddTextInLog(_T("Waiting for data from server....\r\n"));

	/* this can be called any place and time you want.After connecting, of course!
		USAGE cxcGetClientId
	*/
	long nClientId = 0;
	BOOL bRet = cxcGetClientId( &nClientId );
	if(bRet)
	{//it is possible that we do not know Client ID yet.As we ask right away.You could ask again later if you need!
		CString strText;
		strText.Format(_T("We got \"ClientID\" from server:%ld.\r\n"),nClientId);
		AddTextInLog(strText);
	}
	else
	{
		DWORD dwError = GetLastError();
		if(dwError == CXC_ERROR_NOTATTHISTIME)
		{
			CString strText;
			strText.Format(_T("We could not get ClientID from server.\r\nServer does not have it yet.Try a bit later.\r\n"),nClientId);
			AddTextInLog(strText);
		}
	}

	/* this can be called any place and time you want.After connecting, of course!
		USAGE cxcGetSessionMode
	*/
	long nSessionMode = CXCEX_SESSION_MODE_UNKNOWN;
	bRet = cxcGetSessionMode( &nSessionMode );
	if(bRet)
	{//it is possible that we do not know it yet.We could ask again later if you need!
		CString strText;
		if(nSessionMode == CXCEX_SESSION_MODE_RECORD)
			strText.Format(_T("Session mode is \"Recording\".\r\n"));
		if(nSessionMode == CXCEX_SESSION_MODE_REPLAY)
			strText.Format(_T("Session mode is \"Replay\".\r\n"));
		AddTextInLog(strText);
	}

	m_iTimerFrameRate = SetTimer(FRAME_RATE_TIMER, FRAME_PERIOD, NULL);
}

void CTestClientDlg::OnBnClickedReConnect() 
{
	//check: if we were connected, disconnect first!
	if(m_bConnected)//if we were connected, we disconnect and connect again!
		OnBnClickedDisconnect();
	//now try to connect..
	AddTextInLog(_T("*****************************\r\n"));
	DoConnect();
}

void CTestClientDlg::OnLocalNetwork()  
{
	CWnd * pControlWnd = GetDlgItem(IDC_IPADDRESS); 
	pControlWnd->EnableWindow(TRUE);
	m_IPAddress.SetWindowText(m_strDestinationIP);

	UpdateData(true);
}

void CTestClientDlg::OnSamePC() 
{
	m_IPAddress.GetWindowText(m_strDestinationIP);//save what is there
 	CWnd * pControlWnd = GetDlgItem(IDC_IPADDRESS); 
	pControlWnd->EnableWindow(FALSE);
	m_IPAddress.SetWindowText(g_strDefaultServer);//local host

	UpdateData(true);
}

void CTestClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == m_iTimerFrameRate)
	{
		UpdateStatusBar(m_bConnected);
		if ( ! m_bConnected )
			return;

		//first check for the state of connection and other state info:
		/*
			USAGE cxcGetStates
		*/
		int cStates = CXCEX_MAX_ACTIONS;
		long nResult = cxcGetStates( g_aActionStates, &cStates, 0 );

		// Checking for lost connections and errors:
		if ( nResult < 0 )
		{
			switch ( nResult )
			{
			case CXC_E_NOT_CONNECTED:
				AddTextInLog(_T("*** Not connected to Server.\r\n"));
				break;
			case CXC_E_CONNECTION_LOST :
				AddTextInLog(_T("*** Lost connection to Server.\r\n"));
				OnBnClickedDisconnect();
				break;
			}
			return; //we do not continue for all "negative" cases.As we are either not running,not getting data or not connected.
		}

		//if we reached here, it means every thing is ok(nResult >=0) and session is running.
		
		// Checking for any updates:
		if ( 0 != ( CXC_GOT_CLIENTID & nResult ) )
		{
			/*
				USAGE cxcGetClientId
			*/
			long nClientId = 0;
			BOOL bRet = cxcGetClientId( &nClientId );
			if(bRet)
			{//it is possible that we do not know Client ID yet.You could ask again later if you need!
				CString strText;
				strText.Format(_T("We got \"ClientID\" from server:%ld.\r\n"),nClientId);
				AddTextInLog(strText);
			}
		}

		if ( 0 != ( CXC_GOT_SESSION_MODE & nResult ) )
		{
			/* this can be called any place and time you want.After connecting, of course!
				USAGE cxcGetSessionMode
			*/
			long nSessionMode = CXCEX_SESSION_MODE_UNKNOWN;
			BOOL bRet = cxcGetSessionMode( &nSessionMode );
			if(bRet)
			{//it is possible that we do not know it yet.You could ask again later if you need!
				CString strText;
				if(nSessionMode == CXCEX_SESSION_MODE_RECORD)
					strText.Format(_T("Session mode is \"Recording\".\r\n"));
				if(nSessionMode == CXCEX_SESSION_MODE_REPLAY)
					strText.Format(_T("Session mode is \"Replay\".\r\n"));
				AddTextInLog(strText);
			}
		}

		if ( 0 != ( CXC_GOT_FORMAT & nResult ) )
		{
			/*
				USAGE cxcGetFormats
			*/
			int cFormats = CXCEX_MAX_ACTIONS;
			cxcGetFormats( g_aActionFormats, &cFormats, 0 );

			for ( int iAction = 0; iAction < cFormats; ++iAction )
			{
				CXC_ACTION_FORMAT & oFormat = g_aActionFormats[ iAction ];
				if ( 0 == oFormat.maskChanged ) 
					continue;// no changes to formats(Min,Max,Thresholds) 
				//do whatever you need here with this changed format.
				oFormat.maskChanged = 0;//reset mask.
			}
		}

		if ( 0 != ( CXC_GOT_NEW_STATE & nResult ) )
		{
			for ( int iAction = 0; iAction < cStates; ++iAction )
			{
				CXC_ACTION_STATE & oState = g_aActionStates[ iAction ];
				if ( 0 == oState.cSpan ) 
					continue;//no changes to signal value
				//do whatever you need here with this specific signal.
				oState.cSpan = 0;//reset span.
			}
		}

		int cRead, cLost;
		CXC_SAMPLE aBuffer[ MAX_BUFFER ];
		for ( int iAction = 0; iAction < cStates; ++iAction )
		{
			/*
				USAGE cxcGetActionData
				- in order to get the whole sample array you can call cxcGetActionData (see 
					example implementation in VcTestClient);
				- to draw a graph line use aBuffer containing cRead samples and spread 
					them along the Time axis with dInterval:
					double dInterval = 1.0 / 64;
					Adapt this interval if cLost > 0 or restore lost samples with an interpolation 
					algorithm of your choice. Actual implementation depends on your chart control.
			*/
			cRead = 0;
			cLost = 0;
			BOOL bRet = cxcGetActionData( iAction, MAX_BUFFER, aBuffer, &cRead, &cLost );	//get action array data
			if (bRet)
			{
				g_aInSamples[iAction] = cRead;
				CopyMemory(g_aActionData[iAction], aBuffer, cRead * sizeof(float));
				ASSERT (cLost == 0);	//confirmation if correct, othervise error message
			}
			else
				g_aInSamples[iAction] = 0;
		}

		if ( 0 != ( CXC_GOT_SESSION_STATE & nResult ) )
		{
			CString strTextOut(_T(""));
			/*
			USAGE cxcGetSessionState
			*/
			long nSessionState = CXCEX_SESSION_STATE_UNKNOWN;
			BOOL bRet = cxcGetSessionState( &nSessionState );
			bRet;
			switch(nSessionState)
			{
			case CXCEX_SESSION_STATE_UNKNOWN:
				strTextOut = _T("UNKNOWN");
				break;
			case CXCEX_SESSION_STATE_RUNNING:
				strTextOut = _T("RUNNING");
				break;
			case CXCEX_SESSION_STATE_PAUSED:
				strTextOut = _T("PAUSED");
				break;
			case CXCEX_SESSION_STATE_STOPPED:
				strTextOut = _T("STOPPED");
				break;
			case CXCEX_SESSION_STATE_FROZEN :
				strTextOut = _T("FROZEN");
				break;
			default:
				strTextOut = _T("UNKNOWN");
			}
			m_editSessionStatus.SetWindowText(strTextOut);
		}
		// we update our display controls everytime.Even if they have not changed!
		for(int i = 0; i < 8; i++)
		{
			int iSel = m_cbAction[i].GetCurSel();
			DWORD_PTR index = m_cbAction[i].GetItemData(iSel);
			CString strText;
			strText.Format(_T("%.2f"), g_aActionStates[index].nLast);
			m_editSignals[i].SetWindowText(strText);

			strText.Format(_T("%.2f"), g_aActionFormats[index].nScaleMin);
			m_editMin[i].SetWindowText(strText);

			strText.Format(_T("%.2f"), g_aActionFormats[index].nScaleMax);
			m_editMax[i].SetWindowText(strText);

			strText.Format(_T("%.2f"), g_aActionFormats[index].nThreshold1);
			m_editThresh1[i].SetWindowText(strText);

			strText.Format(_T("%.2f"), g_aActionFormats[index].nThreshold2);
			m_editThresh2[i].SetWindowText(strText);	
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CTestClientDlg::OnBnClickedDisconnect()
{
	if(!m_bConnected)
	{
		AddTextInLog(_T("\rNot Connected!\r\n"));
		return;
	}
	m_bConnected = false;
	m_strConnectedServerIP.Empty();
	KillTimer(m_iTimerFrameRate);
	UpdateStatusBar(false);
	/*
		USAGE cxcDisconnect
	*/
	cxcDisconnect();
	AddTextInLog(_T("\rDisconnected from server.\r\n"));

	OnClearMessages();
}

void CTestClientDlg::UpdateStatusBar(bool bConnected)
{
	if(IsWindow(m_StatusBar.m_hWnd))
	{//but put critical section anyway! because we could be closing!!!
		if(bConnected)
		{
			CString strName = GetPCNamefromIP(m_strConnectedServerIP);
			strName = _T("Connected to: ") + m_strConnectedServerIP + _T(" (") + strName + _T(")");
			m_StatusBar.GetStatusBarCtrl().SetText(strName , 0, SBT_POPOUT);
			m_StatusBar.GetStatusBarCtrl().SetBkColor(GREEN_COLOR);
		}
		else
		{
			m_StatusBar.GetStatusBarCtrl().SetText(_T("Disconnected"), 0, SBT_POPOUT);
			m_StatusBar.GetStatusBarCtrl().SetBkColor(RED_COLOR);
		}
	}
}
CString CTestClientDlg::GetPCNamefromIP(CString strIP)
{
	if(strIP.IsEmpty())
		return _T("");
	CString strPCName;
	/* GetNameInfo is new function that MS is recommending to use, but it does not work well.On XP it did not work!
	// I used gethostbyaddr.See code below.
	TCHAR hostName[NI_MAXHOST];
	TCHAR servInfo[NI_MAXSERV];
	struct sockaddr_in saGNI;
	saGNI.sin_family = AF_INET;
	saGNI.sin_addr.s_addr =inet_addr(strIP);
	saGNI.sin_port = htons(m_uiPort);
	DWORD dwRetval = GetNameInfo((SOCKADDR *)&saGNI,
								sizeof(SOCKADDR),
								hostName,//hostName
								NI_MAXHOST, // NI_MAXHOST must be max 1025
								servInfo, 
								NI_MAXSERV, 
								NI_NOFQDN);
	*/
	USES_CONVERSION;
	unsigned int addr;
	addr = inet_addr( T2CA( (LPCTSTR)strIP ) );
	hostent* remoteHost;
	remoteHost = gethostbyaddr((char *)&addr, 4, AF_INET);
	if(remoteHost)
	{
		CString strTemp(remoteHost->h_name);
		strPCName = strTemp;
	}
	else
	{//could not get it.
		strPCName =  _T("");
	}

	return strPCName;
}

