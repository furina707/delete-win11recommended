Option Explicit

Dim WshShell, FSO, objReg, strAppData, strRecentItemsPath
Dim result

Set WshShell = CreateObject("WScript.Shell")
Set FSO = CreateObject("Scripting.FileSystemObject")
Set objReg = GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\default:StdRegProv")

' --- 1. 清空最近项目文件夹 ---
strAppData = WshShell.ExpandEnvironmentStrings("%APPDATA%")
strRecentItemsPath = strAppData & "\Microsoft\Windows\Recent Items"

If FSO.FolderExists(strRecentItemsPath) Then
    On Error Resume Next
    FSO.DeleteFolder strRecentItemsPath & "\*", True
    If Err.Number <> 0 Then
        WScript.Echo "Warning: Unable to fully clear recent items folder. Some files may be in use." & vbCrLf & "Error: " & Err.Description
        Err.Clear
    Else
        WScript.Echo "Successfully cleared recent items folder: " & strRecentItemsPath
    End If
    On Error GoTo 0
Else
    WScript.Echo "Recent items folder does not exist: " & strRecentItemsPath
End If

' --- 2. 修改注册表以禁用"显示最近打开的项目" ---
Const HKEY_CURRENT_USER = &H80000001
Dim strKeyPathAdvanced, strValueNameTrackDocs, dwValueTrackDocs

strKeyPathAdvanced = "Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced"
strValueNameTrackDocs = "Start_TrackDocs"
dwValueTrackDocs = 0

result = objReg.SetDWORDValue(HKEY_CURRENT_USER, strKeyPathAdvanced, strValueNameTrackDocs, dwValueTrackDocs)
If result = 0 Then
    WScript.Echo "Successfully disabled 'Show recently opened items' (Start_TrackDocs)."
Else
    WScript.Echo "Warning: Unable to disable 'Show recently opened items' (Start_TrackDocs). Error code: " & result
End If

' --- 3. 修改注册表以禁用"显示最近添加的应用" ---
Dim strKeyPathStart, strValueNameShowRecentList, dwValueShowRecentList

strKeyPathStart = "Software\Microsoft\Windows\CurrentVersion\Start"
strValueNameShowRecentList = "ShowRecentList"
dwValueShowRecentList = 0

result = objReg.SetDWORDValue(HKEY_CURRENT_USER, strKeyPathStart, strValueNameShowRecentList, dwValueShowRecentList)
If result = 0 Then
    WScript.Echo "Successfully disabled 'Show recently added apps' (ShowRecentList)."
Else
    WScript.Echo "Warning: Unable to disable 'Show recently added apps' (ShowRecentList). Error code: " & result
End If

WScript.Echo vbCrLf & "Operation complete. You may need to restart File Explorer or sign out/in for changes to take full effect."

Set WshShell = Nothing
Set FSO = Nothing
Set objReg = Nothing