Attribute VB_Name = "modMinMaxInfo"
'********************************************************************
'
' Subclass one (1) form to handle WM_GETMINMAXINFO
'
Option Explicit

Private Const GWL_WNDPROC = -4
Private Const WM_GETMINMAXINFO = &H24

Private Type POINT
    x As Long
    y As Long
End Type

Private Type MINMAXINFO
    ptReserved As POINT
    ptMaxSize As POINT
    ptMaxPosition As POINT
    ptMinTrackSize As POINT
    ptMaxTrackSize As POINT
End Type

Private g_lpPrevWndProc As Long
Private g_ptMinSize As POINT

Private Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" _
    (ByVal hWnd As Long, ByVal nIndex As Long _
    , ByVal dwNewLong As Long _
    ) As Long

Private Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" _
    (ByVal lpPrevWndFunc As Long _
    , ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long _
    , ByRef pmmi As MINMAXINFO _
    ) As Long

Private Const HWND_TOPMOST = -1
Private Const HWND_NOTOPMOST = -2
Private Const SWP_NOSIZE = 1
Private Const SWP_NOMOVE = 2

Private Declare Function SetWindowPos Lib "user32" Alias "SetWindowPos" _
    (ByVal hwnd As Long _
    , ByVal hWndInsertAfter As Long _
    , ByVal x As Long _
    , ByVal y As Long _
    , ByVal cx As Long _
    , ByVal cy As Long _
    , ByVal wFlags As Long _
    ) As Long

Public Sub HandleMinMaxInfo(ByRef frm As Form)

    If 0 <> g_lpPrevWndProc Then Exit Sub

#If NDEBUG Then

    g_ptMinSize.x = frm.ScaleX(frm.Width, vbTwips, vbPixels)
    g_ptMinSize.y = frm.ScaleY(frm.Height, vbTwips, vbPixels)

    g_lpPrevWndProc = SetWindowLong(frm.hWnd, GWL_WNDPROC, AddressOf MinMaxInfoProc)

#End If

    SetWindowPos frm.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE

End Sub

Public Sub UnhandleMinMaxInfo(ByRef frm As Form)

    If 0 = g_lpPrevWndProc Then Exit Sub

    SetWindowLong frm.hWnd, GWL_WNDPROC, g_lpPrevWndProc

End Sub

Function MinMaxInfoProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long _
    , ByRef pmmi As MINMAXINFO _
    ) As Long

    On Error Resume Next

    If uMsg = WM_GETMINMAXINFO Then
        pmmi.ptMinTrackSize = g_ptMinSize
        MinMaxInfoProc = 0
    Else
        MinMaxInfoProc = CallWindowProc(g_lpPrevWndProc, hWnd, uMsg, wParam, pmmi)
    End If

End Function

