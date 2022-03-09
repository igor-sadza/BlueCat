VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "VbTestClient"
   ClientHeight    =   5688
   ClientLeft      =   48
   ClientTop       =   432
   ClientWidth     =   5004
   ClipControls    =   0   'False
   Icon            =   "VbTestClient.frx":0000
   LinkTopic       =   "frmMain"
   ScaleHeight     =   5688
   ScaleWidth      =   5004
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrFrameRate 
      Enabled         =   0   'False
      Interval        =   33
      Left            =   3840
      Top             =   120
   End
   Begin VB.TextBox txtIpAddress 
      Height          =   288
      Left            =   120
      MaxLength       =   15
      TabIndex        =   1
      Top             =   360
      Width           =   2052
   End
   Begin VB.CheckBox chkConnected 
      Caption         =   "&Connected to BioGraph"
      CausesValidation=   0   'False
      Height          =   252
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   4092
   End
   Begin VB.TextBox txtMessages 
      CausesValidation=   0   'False
      Height          =   1572
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Top             =   1080
      Width           =   4092
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   0
      Left            =   120
      TabIndex        =   4
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   1
      Left            =   720
      TabIndex        =   5
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   2
      Left            =   1320
      TabIndex        =   6
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   3
      Left            =   1920
      TabIndex        =   7
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   4
      Left            =   2520
      TabIndex        =   8
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   5
      Left            =   3120
      TabIndex        =   9
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   6
      Left            =   3720
      TabIndex        =   10
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VbTestClient.SimpleBar sbcAction 
      CausesValidation=   0   'False
      Height          =   2688
      Index           =   7
      Left            =   4320
      TabIndex        =   11
      Top             =   2880
      Width           =   432
      _ExtentX        =   0
      _ExtentY        =   0
   End
   Begin VB.Label lbl1 
      AutoSize        =   -1  'True
      Caption         =   "&BioGraph location:"
      Height          =   192
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1320
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'********************************************************************
'
'   COPYRIGHT (c) 2008 Thought Technology Ltd.
'
'   Connection Client API Sample Application
'
'   About:
'
'       Simple application to demonstrate expected normal use of the
'       Connection Client API in "immediate mode".
'
'   Written for:
'
'       Thought Technology Ltd.
'       2180 Belgrave Ave.
'       Montreal, Quebec H4A,2L8
'       800-361-3651 (Voice)
'       514-489-8255 (Fax)
'
'   Written by:
'
'       Mark Richards
'
'********************************************************************
Option Explicit

Const strDefaultServer As String = "127.0.0.1"
Private strServer As String
Private bConnected As Boolean
Private aActionFormats(0 To TtlCxc.CXC_MAX_ACTIONS - 1) As TtlCxc.CXC_ACTION_FORMAT
Private aActionStates(0 To TtlCxc.CXC_MAX_ACTIONS - 1) As TtlCxc.CXC_ACTION_STATE
Private bPausedOrBlocked As Boolean

Private twipMargin As Long
Private twipSpaces As Long

Private Sub Form_Load()

    '
    '   USAGE cxcInitialize
    '
    If 0 = TtlCxc.cxcInitialize(TtlCxc.CXC_VERSION) Then
        MsgBox "Fatal error initializing connection client.", vbCritical
        Unload Me
        Exit Sub
    End If

    twipMargin = sbcAction(0).Left
    twipSpaces = twipMargin

    DoConnect

    If bConnected Then
        chkConnected = vbChecked
    Else
        Message "" _
            & vbCrLf & "1) Make sure BioGraph is running." _
            & vbCrLf & "2) Enter BioGraph's IP." _
            & vbCrLf & "3) Click on the check box." _
            & vbCrLf
    End If

    HandleMinMaxInfo Me

End Sub

Private Sub Form_Unload(Cancel As Integer)

    UnhandleMinMaxInfo Me

    TtlCxc.cxcDisconnect

    '
    '   USAGE cxcTerminate
    '
    TtlCxc.cxcTerminate

End Sub

Private Sub chkConnected_Click()

    If bConnected Eqv (vbChecked = chkConnected) Then
        Exit Sub ' Nothing to do, chkConnected changed by code
    End If

    If bConnected Then
        DoDisconnect
    Else
        DoConnect
    End If

    If bConnected Xor (vbChecked = chkConnected) Then
        chkConnected = IIf(bConnected, vbChecked, vbUnchecked)
    End If

End Sub

Private Sub DoConnect()

    strServer = txtIpAddress
    If 0 = Len(strServer) Then strServer = strDefaultServer

    '
    '   USAGE cxcConnect
    '
    MousePointer = vbHourglass
    Dim nRetVal As Long: nRetVal = TtlCxc.cxcConnect(strServer)
    MousePointer = vbDefault

    If 0 = nRetVal Then
        Message vbCrLf & "Could not connect to """ & strServer & """." & vbCrLf
        Exit Sub
    End If

    Message vbCrLf & "Connected to BioGraph at """ & strServer & """." & vbCrLf

    txtIpAddress.Enabled = False
    txtIpAddress.BackColor = Me.BackColor

    bConnected = True
    tmrFrameRate.Enabled = True

End Sub

Private Sub DoDisconnect()

    bConnected = False
    tmrFrameRate.Enabled = False

    '
    '   USAGE cxcDisconnect
    '
    TtlCxc.cxcDisconnect

    Message vbCrLf & "Disconnected from """ & strServer & """." & vbCrLf

    txtIpAddress.Enabled = True
    txtIpAddress.BackColor = vbWindowBackground

    ' clear state and update display:

    bPausedOrBlocked = False

    Dim oEmptyFormat As TtlCxc.CXC_ACTION_FORMAT
    Dim oEmptyState As TtlCxc.CXC_ACTION_STATE

    Dim iAction As Integer
    For iAction = 0 To TtlCxc.CXC_MAX_ACTIONS - 1
        aActionFormats(iAction) = oEmptyFormat
        aActionStates(iAction) = oEmptyState
        InvalidateFormat iAction, aActionFormats(iAction)
    Next iAction

End Sub

Private Sub tmrFrameRate_Timer()

    Dim iAction As Integer

    '
    '   USAGE cxcGetStates
    '
    Dim cStates As Long: cStates = TtlCxc.CXC_MAX_ACTIONS
    Dim nResult As Long: nResult = TtlCxc.cxcGetStates(aActionStates(0), cStates)

    ' Checking for lost connections and errors:

    If nResult < 0 Then
        Select Case nResult
        Case TtlCxc.CXC_E_CONNECTION_LOST
            Message "*** Lost connection to """ & strServer & """." & vbCrLf
            DoDisconnect
            chkConnected = vbUnchecked
        Case TtlCxc.CXC_E_SESS_IN_PROGRESS
            If Not bPausedOrBlocked Then
                Message "*** Session already in progress." & vbCrLf _
                    & "Waiting for proper session start." & vbCrLf
                bPausedOrBlocked = True ' Don't show message again
            End If
        Case TtlCxc.CXC_E_SESS_PAUSED
            If Not bPausedOrBlocked Then
                Message "Stream PAUSED." & vbCrLf
                bPausedOrBlocked = True ' Don't show message again
            End If
        End Select
        Exit Sub
    End If

    ' Checking for updates:

    If nResult And TtlCxc.CXC_GOT_START Then
        bPausedOrBlocked = False
        Message "Stream STARTED." & vbCrLf
    End If

    If nResult And TtlCxc.CXC_GOT_RESUME Then
        bPausedOrBlocked = False
        Message "Stream RESUMED." & vbCrLf
    End If

    If nResult And TtlCxc.CXC_GOT_CLIENTID Then

        '
        '   USAGE cxcGetClientId
        '
        Dim nClientId As Long
        TtlCxc.cxcGetClientId nClientId

        Message "Client ID is " & nClientId & "." & vbCrLf

    End If

    If nResult And TtlCxc.CXC_GOT_FORMAT Then

        '
        '   USAGE cxcGetFormats
        '
        Dim cFormats As Long: cFormats = TtlCxc.CXC_MAX_ACTIONS
        TtlCxc.cxcGetFormats aActionFormats(0), cFormats

        For iAction = 0 To cFormats - 1
            If 0 <> aActionFormats(iAction).maskChanged Then
                Message "Action " & iAction & "'s format updated." & vbCrLf
                InvalidateFormat iAction, aActionFormats(iAction)
                aActionFormats(iAction).maskChanged = 0
            End If
        Next iAction

    End If

    If nResult And TtlCxc.CXC_GOT_NEW_STATE Then

        For iAction = 0 To cStates - 1
            If 0 < aActionStates(iAction).cSpan Then
                InvalidateState iAction, aActionStates(iAction)
                aActionStates(iAction).cSpan = 0
            End If
        Next iAction

    End If

End Sub


'////////////////////////////////////////////////////////////////////
'// Drawing handlers

Private Sub Form_Resize()

    Dim y As Long: y = sbcAction(0).Top
    Dim cyBar As Long: cyBar = ScaleHeight - y - twipMargin
    If 0 > cyBar Then Exit Sub

    Dim x As Long: x = twipMargin
    Dim dx As Long: dx = (ScaleWidth + twipSpaces - 2 * twipMargin) / TtlCxc.CXC_MAX_ACTIONS
    Dim cxBar As Long: cxBar = dx - twipSpaces
    If 0 > cxBar Then Exit Sub

    Dim iAction As Integer
    For iAction = 0 To TtlCxc.CXC_MAX_ACTIONS - 1
        sbcAction(iAction).Move x, y, cxBar, cyBar
        x = x + dx
    Next iAction

End Sub

Sub InvalidateFormat(ByVal iAction As Integer, ByRef oFormat As TtlCxc.CXC_ACTION_FORMAT)

    sbcAction(iAction).SetFormat oFormat.eType, oFormat.nScaleMin, oFormat.nScaleMax

End Sub

Sub InvalidateState(ByVal iAction As Integer, ByRef oState As TtlCxc.CXC_ACTION_STATE)

    sbcAction(iAction).SetState oState.nLast

End Sub


'/////////////////////////////////////////////////////////////////////
'// Helper functions

Private Sub Message(strText As String)

    With txtMessages

        Dim cchEdit As Long: cchEdit = Len(.Text)
        Dim cchText As Long: cchText = Len(strText)

        If 0 = .MaxLength Then .MaxLength = 10 * 1024
        If .MaxLength < cchEdit + cchText Then
            Dim iChar As Long
            iChar = InStrRev(.Text, vbLf, cchEdit / 2)
            .SelStart = 0
            .SelLength = iChar
            .SelText = ""
            cchEdit = cchEdit - iChar
        End If

        .SelStart = cchEdit
        .SelText = strText

    End With

End Sub
