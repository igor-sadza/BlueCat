VERSION 5.00
Begin VB.UserControl SimpleBar 
   ClientHeight    =   2880
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   600
   ClipBehavior    =   0  'None
   ClipControls    =   0   'False
   FillColor       =   &H8000000F&
   FillStyle       =   0  'Solid
   HitBehavior     =   0  'None
   ScaleHeight     =   2880
   ScaleWidth      =   600
   Begin VB.Label lblTop 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "+0000"
      Height          =   240
      Left            =   0
      TabIndex        =   0
      Top             =   0
      UseMnemonic     =   0   'False
      Width           =   600
   End
   Begin VB.Label lblBot 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "-0000"
      Height          =   240
      Left            =   0
      TabIndex        =   1
      Top             =   2640
      UseMnemonic     =   0   'False
      Width           =   600
   End
End
Attribute VB_Name = "SimpleBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private dyLabels As Long

Private bEnabled As Boolean
Private nYTop As Single ' scale max
Private nYBot As Single ' scale min
Private nYCur As Single ' last value

Private Sub UserControl_Initialize()

    ScaleMode = vbPixels

    dyLabels = TextHeight("|")

    lblTop.Top = 0
    lblTop.Left = 0
    lblTop.Height = dyLabels

    lblBot.Left = 0
    lblBot.Height = dyLabels

    nYTop = 1
    nYBot = 0
    SetFormat TtlCxc.CXC_FORMAT_NONE, 0, 0

End Sub

Private Sub UserControl_Resize()

    lblTop.Width = ScaleWidth

    If ScaleHeight >= dyLabels Then lblBot.Move 0, ScaleHeight - dyLabels, ScaleWidth

    UserControl_Paint

End Sub

Private Sub UserControl_Paint()

    Dim cx As Long: cx = ScaleWidth - 1
    Dim cy As Long: cy = ScaleHeight

    If cy >= 2 * dyLabels + 2 Then

        cy = cy - 2 * dyLabels

        If bEnabled Then

            Dim y As Long
            If nYTop <= nYCur Then
                y = cy
            ElseIf nYCur <= nYBot Then
                y = 0
            Else
                y = cy * (nYCur - nYBot) / (nYTop - nYBot)
            End If

            If y < cy Then
                Line (0, dyLabels)-Step(cx, cy - y), vbHighlightText, BF
            End If
            If 0 < y Then
                Line (0, dyLabels + cy - y)-Step(cx, y), vbHighlight, BF
            End If

        Else

            Line (0, dyLabels)-Step(cx, cy), vbButtonShadow, B

        End If

    End If

End Sub

Public Sub SetFormat(ByVal eType As Long, ByVal nScaleMin As Single, ByVal nScaleMax As Single)

    bEnabled = True

    Dim strUnits As String: strUnits = ""

    Select Case eType
    Case TtlCxc.CXC_FORMAT_FULL
        nYTop = nScaleMax
        nYBot = nScaleMin
    Case TtlCxc.CXC_FORMAT_PERCENTAGE
        nYTop = 100
        nYBot = 0
        strUnits = "%"
    Case TtlCxc.CXC_FORMAT_BOOLEAN
        nYTop = 1
        nYBot = 0
    Case Else
        bEnabled = False
        lblTop.Caption = "-"
        lblBot.Caption = "-"
    End Select

    If bEnabled Then
        lblTop.Caption = nYTop & strUnits
        lblBot.Caption = nYBot & strUnits
    End If

    UserControl_Paint

End Sub

Public Sub SetState(ByVal nLast As Single)

    nYCur = nLast

    UserControl_Paint

End Sub
