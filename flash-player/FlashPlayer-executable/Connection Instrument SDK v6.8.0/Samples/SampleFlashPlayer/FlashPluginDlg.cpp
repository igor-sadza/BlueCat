/****************************************************************************
*
* Company:  Thought Technology Ltd.
* Website:	http://www.thoughttechnology.com 
* Created:	September 23, 2010
*
* Project:	Biograph  Infiniti: ConnectionInstrument SDK
*
* Purpose:	This is a sample application using ConnectionClient.dll. 
*			ConnectionClient.dll provides network connection between this
*			application and Connection Instrument from Biorgaph.
*			This application can play any Flash movie and pass 
*			necessary parameters(signals) from Biograph(thru Connection Instrument)
*			to Flash animation.For more information see ConnectionInstrument SDK.
*	 
*****************************************************************************/
// FlashPluginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlashPlugin.h"
#include "FlashPluginDlg.h"
#include "SelectComputer.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <Sensapi.h>
#pragma comment(lib, "Sensapi.lib")


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

enum
{
	FRAME_RATE = 60 // Hz
,	FRAME_PERIOD = 1000 / FRAME_RATE // ms
};

CXC_ACTION_FORMAT g_aActionFormats[ CXCEX_MAX_ACTIONS ] = { 0, };
CXC_ACTION_STATE g_aActionStates[ CXCEX_MAX_ACTIONS ] = { 0, };
CString g_strConnection[CXCEX_MAX_ACTIONS];

struct TConnectionSettings
{
    CString strTargetIP;
	CString strComputerName;
};

// -- GLOBAL --------------------
CShockwaveFlash g_FlashPlayer;
TConnectionSettings g_ConnectionSettings;

#define TIMER_WORK		1

#define GREEN_COLOR			0x99CC00
#define RED_COLOR			0x0000CC

#define INDICATORS_SIZE 2
static UINT BASED_CODE indicators[] =
{
    ID_INDICATOR_CONNECTION,
    ID_INDICATOR_FLASHFORMAT
};

////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CFlashPluginDlg dialog

CFlashPluginDlg::CFlashPluginDlg(TCHAR *szCmdLine/*= NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CFlashPluginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashPluginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// by default: we try to connect to local PC :
	g_ConnectionSettings.strTargetIP = "127.0.0.1";
	g_ConnectionSettings.strComputerName = _T("Local PC");
	m_strFlashName = szCmdLine;
	m_bStartedExternally = false;
	m_iTimer1 = 0;
	m_uiPort = 1000;
	m_bConnected = false;
	m_nClientID = -1;
	m_nSessionMode = CXCEX_SESSION_MODE_UNKNOWN;
	m_nSessionState = CXCEX_SESSION_STATE_UNKNOWN;
	m_bGotClientID = false;
	m_bGotSessionMode = false;
	for (UINT i = 0; i < CXCEX_MAX_ACTIONS; i++)
		g_strConnection[i].Format(_T("Connection%d"), i);
}

void CFlashPluginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashPluginDlg)
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH, g_FlashPlayer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashPluginDlg, CDialog)
	//{{AFX_MSG_MAP(CFlashPluginDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
    ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
    ON_COMMAND(IDM_CONNECT_TO_PC, OnConnectToPC)
	ON_COMMAND(IDM_HELP_ABOUT, OnHelpAbout)
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_DISCONNECT, OnFileMenuDisconnect)
	ON_COMMAND(IDM_FILE_EXIT, OnFileMenuExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
BEGIN_EVENTSINK_MAP(CFlashPluginDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFlashPluginDlg)
	ON_EVENT(CFlashPluginDlg, IDC_SHOCKWAVEFLASH, 150 /* FSCommand */, OnFSCommandShockwaveflash, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CFlashPluginDlg::OnInitDialog()
{
	DWORD dwSens;
	if (IsNetworkAlive(&dwSens) == FALSE) {
		MessageBox("Check your ethernet connection!");
		EndDialog(0);
	}


	ShowWindow(SW_MAXIMIZE);
    CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
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

	ResetAllValues();

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

    // Create status bar
    m_StatusBar.Create(this);
    m_StatusBar.SetIndicators(indicators,INDICATORS_SIZE);

    CRect rect;
    GetClientRect(&rect);
    //Size the two panes
    m_StatusBar.SetPaneInfo(0,ID_INDICATOR_CONNECTION, SBPS_NORMAL,rect.Width() - 300);      
    m_StatusBar.SetPaneInfo(1,ID_INDICATOR_FLASHFORMAT,SBPS_STRETCH ,0);

    m_StatusBar.GetStatusBarCtrl().SetBkColor(0xD8E9EC);//default color of the dialog.


    //This is where we actually draw it on the screen
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_FLASHFORMAT);

    // Attempt to connect to local PC immidietly after creation.
    DoConnect();

	// take default flash from the current directory:
	TCHAR cApplicationDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, cApplicationDir);

	CString strSource;
	strSource.Format(_T("%s\\%s"),cApplicationDir,m_strFlashName);

	//If we have some parameters, check if this file exists!
	if(!m_strFlashName.IsEmpty())
	{
		bool bExists = true;
		WIN32_FIND_DATA fd;
		HANDLE h = FindFirstFile(strSource, &fd);
		if (INVALID_HANDLE_VALUE == h)
		{
			bExists = false;
			CString strTemp;
			strTemp.Format(_T("Could not load provided Flash movie!\nFile: \"%s\" does not exist!"),strSource);
			AfxMessageBox(strTemp);
		}
		else
		{
			::FindClose(h);
			bExists = true;
		}
		if(bExists)
		{
			SetMovie(strSource);
			m_bStartedExternally = true;
			GetMenu()->EnableMenuItem(IDM_FILE_OPEN, MF_GRAYED |  MF_BYCOMMAND);
		}
	}

	SetWindowPos(&wndTopMost, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE);

	m_StatusBar.SetWindowPos(NULL, 0, 0, rect.Width(), 15, SWP_SHOWWINDOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlashPluginDlg::OnHelpAbout() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CFlashPluginDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CFlashPluginDlg::UpdateStatusBar(bool bConnected)
{
	if(IsWindow(m_StatusBar.m_hWnd))
	{//but put critical section anyway! because we could be closing!!!
		if(bConnected)
		{
			CString strName;
			strName = _T(" Connected to: ") + g_ConnectionSettings.strComputerName;
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

//We need this timer to check status of connection to server.
//As we cannot depend on callbacks from Flash to update status!
//for example there could be the case when no flash is loaded AT ALL!!
//And in this case this timer would tell us the status of connection.
//and since we make calls anyway,I save last data locally(which I will pass to Flash when Flash requests it!)
void CFlashPluginDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_WORK)
	{
		UpdateStatusBar(m_bConnected);
		if ( ! m_bConnected )
			return;
		
		//first check for the state of connection and other state info:
		int cStates = CXCEX_MAX_ACTIONS;
		long nResult = cxcGetStates( g_aActionStates, &cStates, 0 );
		// Checking for lost connections and errors:
		if ( nResult < 0 )
		{
			switch ( nResult )
			{
			case CXC_E_NOT_CONNECTED:
				break;
			case CXC_E_CONNECTION_LOST :
				DoDisconnect();//Lost connection to Server
				break;
			}
			return;
		}

		if ( 0 != ( CXC_GOT_CLIENTID & nResult ) )
		{
			cxcGetClientId( &m_nClientID );
			m_bGotClientID = true;
		}
		if ( 0 != ( CXC_GOT_SESSION_MODE & nResult ) )
		{
			cxcGetSessionMode( &m_nSessionMode );
			m_bGotSessionMode = true;
		}
		if ( 0 != ( CXC_GOT_SESSION_STATE & nResult ) )
		{
			cxcGetSessionState( &m_nSessionState );
		}

		//if we reached here, it means session is not stopped.
		//get latest values before sending:
		if ( 0 != ( CXC_GOT_FORMAT & nResult ) )
		{
			int cFormats = CXCEX_MAX_ACTIONS;
			cxcGetFormats( g_aActionFormats, &cFormats, 0 );
			for ( int iAction = 0; iAction < cFormats; ++iAction )
			{
				CXC_ACTION_FORMAT & oFormat = g_aActionFormats[ iAction ];
				m_strScaleMin[iAction].Format(_T("%.2f") , g_aActionFormats[iAction].nScaleMin);
				m_strScaleMax[iAction].Format(_T("%.2f") , g_aActionFormats[iAction].nScaleMax);
				m_strThreshold1[iAction].Format(_T("%.2f") , g_aActionFormats[iAction].nThreshold1);
				m_strThreshold2[iAction].Format(_T("%.2f") , g_aActionFormats[iAction].nThreshold2);
				if ( 0 == oFormat.maskChanged ) 
					continue;// no changes to formats(Min,Max,Thresholds) 
				oFormat.maskChanged = 0;//reset mask.
			}
		}
		if ( 0 != ( CXC_GOT_NEW_STATE & nResult ) )
		{
			for ( int iAction = 0; iAction < cStates; ++iAction )
			{
				CXC_ACTION_STATE & oState = g_aActionStates[ iAction ];
				if (g_aActionStates[iAction].nLast < 9.0e+37)//we could get hole value! which we must ignore!keep prev.value
					m_strSignal[iAction].Format(_T("%.2f"), g_aActionStates[iAction].nLast);
				if ( 0 == oState.cSpan ) 
					continue;//no changes to signal value
				oState.cSpan = 0;//reset span.
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}
void CFlashPluginDlg::OnFileMenuDisconnect() 
{
	DoDisconnect();

	g_ConnectionSettings.strComputerName = "";
	g_ConnectionSettings.strTargetIP = "";
}

void CFlashPluginDlg::OnFileMenuExit() 
{
	OnOK(); 
}
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CFlashPluginDlg::OnPaint() 
{
	CRect rect;
	GetClientRect(&rect);
	GetDlgItem(IDC_SHOCKWAVEFLASH)->SetWindowPos(NULL, 0, 15, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

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
void CFlashPluginDlg::OnDestroy()
{
	DoDisconnect();
	/*
		USAGE cxcTerminate
	*/
	cxcTerminate();

	::DestroyWindow(m_StatusBar);

	CDialog::OnDestroy();
}
void CFlashPluginDlg::DoDisconnect()
{
	if(!m_bConnected)
		return;
	g_ConnectionSettings.strComputerName = "";
	g_ConnectionSettings.strTargetIP = "";
	m_bConnected = false;
	m_nSessionState = CXCEX_SESSION_STATE_UNKNOWN;
	KillTimer(m_iTimer1);
	UpdateStatusBar(m_bConnected);
	cxcDisconnect();
}

void CFlashPluginDlg::OnOK()// this is button EXIT!!
{
	CDialog::OnOK();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlashPluginDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Select Flash movie
void CFlashPluginDlg::OnFileOpen()
{
    
    // Select Flash Animation
    CString strPath;
    TCHAR tcCurrDir[1024];
    GetCurrentDirectory(1024, tcCurrDir);
	strPath.Format(_T("%s"), "C:\\ProgramData\\Thought Technology\\Infiniti\\Flash");

    CFileDialog fileDlg(TRUE, NULL, NULL, NULL, _T("Flash Movies (*.swf)|*.swf||"));
    fileDlg.m_ofn.lpstrInitialDir = strPath;
    if (fileDlg.DoModal() == IDOK)
	{
		ResetAllValues();
        strPath = fileDlg.GetPathName();
		SetMovie(strPath);
	}
}

void CFlashPluginDlg::SetMovie(CString csPath)
{
    g_FlashPlayer.put_Movie(csPath);
	//g_FlashPlayer.PutMovie(_bstr_t(csPath));

	CString strFileNameOnly;
	//extract directory from target path
	int iIndex = csPath.ReverseFind(_T('\\'));
	if(iIndex > -1)
	{
		int iPos = csPath.GetLength() - iIndex - 1;
		strFileNameOnly = csPath.Right(iPos); 
	}
	SetWindowText(strFileNameOnly);
}

void CFlashPluginDlg::OnConnectToPC()
{
   TIPAddress tIPAddr;
    CSelectComputerDialog dlg;
    INT_PTR result = dlg.DoModal();
    if (result == IDOK)
    {
		DoDisconnect();//disconnect from current one(if connected)
        if (dlg.GetIPAddress(&tIPAddr))
		{
            g_ConnectionSettings.strTargetIP = tIPAddr.strIP;
			g_ConnectionSettings.strComputerName = tIPAddr.strComputerName;
			DoConnect();
		}
	}
}

void CFlashPluginDlg::DoConnect()
{
	CWaitCursor w;
	DoDisconnect();

	m_bGotClientID = false;
	m_bGotSessionMode = false;

	// By default, we try to connect  to 127.0.0.1(loopback device).
    // It means that both the producer and consumer are running on the same computer.
	// Keep in mind, that Biograph acts as a server.
	// That means that if you close the game and then open it, connections will be restored automatically.
	// But if you close Biograph and then open it again and connection with the game will be lost and you have to restart your game(i.e recreate your ConnectionServer again).

	cxcSetPort( m_uiPort );//if we do not do it , default port used by Server is 1000. But it could be also changed for Server.
	BOOL bRetVal = cxcConnect( g_ConnectionSettings.strTargetIP );
	if(!bRetVal)
	{  //could not connect!
		DWORD dwError = GetLastError();
		if(dwError == CXC_ERROR_NOTINITIALIZED)
			AfxMessageBox(_T("\nERROR! Either Missing or Failed call to cxcInitialize() !\r\n"));
		else
			AfxMessageBox(_T("\nERROR! Could not connect to Server!\r\nProbably it is not running or destination IP is not correct!\r\nTry to start Server or enter correct IP and then try to Reconnect !\r\n"));
		m_bConnected = false;
		UpdateStatusBar(m_bConnected);
		return;
	}

	m_iTimer1 = SetTimer(TIMER_WORK, FRAME_PERIOD, NULL);
	
	m_bConnected = true;

	UpdateStatusBar(m_bConnected);
}

int GetIndexFromString(CString strAction )
{
	bool bFound = false;
	UINT i;
	for (i = 0; i < CXCEX_MAX_ACTIONS; i++)
	{
		if(g_strConnection[i].CompareNoCase(strAction) == 0)
		{
			bFound = true;
			break;
		}
	}
	if(bFound)
		return i;
	else
		return -1;
}
//this function is called by Flash object.
//and according to params, we send back the requested data(last values gotten in my timer!)!
void CFlashPluginDlg::OnFSCommandShockwaveflash(LPCTSTR command, LPCTSTR args) 
{
	//check if we are connected!
	if(!m_bConnected)//if not just reset all values and send them back to Flash.
	{
		ResetAllValues();

		CString strTemp;
		strTemp.Format(_T("%d"),m_nSessionState);
		g_FlashPlayer.SetVariable(_T("g_strState"), _bstr_t(strTemp));
		
		for (UINT i = 0; i < CXCEX_MAX_ACTIONS; i++)
		{
			CString strTemp;
			strTemp.Format(_T("g_strSignal%d"), i);
			g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strSignal[i]));
			
			strTemp.Empty();
			strTemp.Format(_T("g_strThreshold%d_1"), i);
			g_FlashPlayer.SetVariable(_bstr_t(strTemp) , _bstr_t(m_strThreshold1[i]));

			strTemp.Empty();
			strTemp.Format(_T("g_strThreshold%d_2"), i);
			g_FlashPlayer.SetVariable(_bstr_t(strTemp) , _bstr_t(m_strThreshold2[i]));

			strTemp.Empty();
			strTemp.Format(_T("g_strScaleMin%d"), i);
			g_FlashPlayer.SetVariable(_bstr_t(strTemp) , _bstr_t(m_strScaleMin[i]));

			strTemp.Empty();
			strTemp.Format(_T("g_strScaleMax%d"), i);
			g_FlashPlayer.SetVariable(_bstr_t(strTemp) , _bstr_t(m_strScaleMax[i]));
		}
		return;
	}
	 
	//if we reached here, it means we are connected!
	if(m_nSessionState == CXCEX_SESSION_STATE_STOPPED)
	{
		for (UINT i = 0; i < CXCEX_MAX_ACTIONS; i++)
			m_strSignal[i] = _T("0");//set all signals to 0.but do not touch thresholds and scale as we do not send them unless they are changed!
	}
	//you can tell your flash the current Session State:
	CString strTemp;
	strTemp.Format(_T("%d"),m_nSessionState);
	g_FlashPlayer.SetVariable(_T("g_strState"), _bstr_t(strTemp));

	// Now check what Flash wants..
	CString strCommand(_T("GetNewData"));
	CString strConnectionALL(_T("ConnectionALL"));
	// I do this to avoid sending too much unecessary data.
	// But it is still possible to get all the data at once with arg="ConnectionALL"
	if (strCommand.CompareNoCase(command) == 0)//GetNewData
	{
		if(strConnectionALL.CompareNoCase(args) == 0)//"ConnectionALL"
		{	//Flash asked for data for all actions. Send all data:
			for (UINT i = 0; i < CXCEX_MAX_ACTIONS; i++)
			{
				CString strTemp;
				strTemp.Format(_T("g_strSignal%d"), i);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strSignal[i]));

				strTemp.Empty();
				strTemp.Format(_T("g_strThreshold%d_1"), i);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strThreshold1[i]));

				strTemp.Empty();
				strTemp.Format(_T("g_strThreshold%d_2"), i);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strThreshold2[i]));

				strTemp.Empty();
				strTemp.Format(_T("g_strScaleMin%d"), i);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strScaleMin[i]));

				strTemp.Empty();
				strTemp.Format(_T("g_strScaleMax%d"), i);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strScaleMax[i]));
			}
		}
		else
		{ //only specific action!
			int index = GetIndexFromString(args);
			if(index >= 0 && index < CXCEX_MAX_ACTIONS)
			{
				CString strTemp;
				strTemp.Format(_T("g_strSignal%d"), index);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strSignal[index]));

				strTemp.Empty();
				strTemp.Format(_T("g_strThreshold%d_1"), index);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strThreshold1[index]));

				strTemp.Empty();
				strTemp.Format(_T("g_strThreshold%d_2"), index);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strThreshold2[index]));

				strTemp.Empty();
				strTemp.Format(_T("g_strScaleMin%d"), index);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strScaleMin[index]));

				strTemp.Empty();
				strTemp.Format(_T("g_strScaleMax%d"), index);
				g_FlashPlayer.SetVariable(_bstr_t(strTemp), _bstr_t(m_strScaleMax[index]));
			}
		}
	}
}
//clears all values.
void CFlashPluginDlg::ResetAllValues()
{
	for (UINT i = 0; i < CXCEX_MAX_ACTIONS; i++)
	{
		m_strSignal[i] = _T("0");
		m_strThreshold1[i] = _T("0");
		m_strThreshold2[i] = _T("0");
		m_strScaleMin[i] = _T("0");
		m_strScaleMax[i] = _T("0");
	}
}