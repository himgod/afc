#include <ColorConstants.au3>
#include <Constants.au3>
#include <GUIConstantsEx.au3>
#include <DirConstants.au3>
#include <FileConstants.au3>
#include <IE.au3>
#include <ProcessConstants.au3>
#include <String.au3>
#include <WinAPIError.au3>
#include <MsgBoxConstants.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>
#include <GuiButton.au3>

Global $var1 = @ScriptDir&"\data\settings\settings.ini"
Global $var2 = @ScriptDir&"\data\language\"
Global $lng = IniRead ($var1, "language", "key", "NotFound")
Global $DefaultVm = IniRead($var1, "afc", "vm", "AFC")
Global $debugMode = IniRead($var1, "afc", "debug", 0)

;Controls for process bar
Global $procBarForm, $procBarLabel, $procBarProgress
Global $initPercent, $progressCounter, $debugLabel
Global $DRV = 1, $NET = 1, $MON = 1, $USB = 1, $ADP = 1

InitProcessBar()

$initPercent = 100
$progressCounter = 0
AdlibRegister("RefreshProgress", 1000)

;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","ProcessWaitClose VirtualBox",0)
ProcessWaitClose ("VirtualBox.exe")
If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:1-1/4 ProcessWaitClose VirtualBox.exe")
	sleep(2000)
EndIf
$initPercent = 95
;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","ProcessWaitClose VirtualManage",0)
ProcessWaitClose ("VBoxManage.exe")
If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:1-2/4 ProcessWaitClose VBoxManage.exe")
	sleep(2000)
EndIf
$initPercent = 90
ProcessWaitClose ("VBoxHeadless.exe")
If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:1-3/4 ProcessWaitClose VBoxHeadless.exe")
	sleep(2000)
EndIf
$initPercent = 85

;SplashTextOn ("AFi-Controller", IniRead ($var2 & $lng &".ini", "messages", "07", "NotFound"), 220, 40, -1, -1, 1, "arial", 12)

ProcessWaitClose ("VBoxSVC.exe")
If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:1-4/4 ProcessWaitClose VBoxSVC.exe")
	sleep(2000)
EndIf
$initPercent = 80

EnvSet ("VBOX_USER_HOME")
Local $timer=0

Local $PID = ProcessExists ("VBoxSVC.exe")
If $PID Then ProcessClose ($PID)

While $timer < 10000 AND $PID
$PID = ProcessExists ("VBoxSVC.exe")
If $PID Then ProcessClose ($PID)
  Sleep(1000)
  $timer += 1000
Wend
If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:2-1/1 ProcessClose VBoxSVC.exe")
	sleep(2000)
EndIf
$initPercent = 75

    If @OSArch = "x86" Then
      Global $arch = "app32"
    EndIf
    If @OSArch = "x64" Then
      Global $arch = "app64"
    EndIf
	
	If @OSArch = "x86" Then
        If NOT FileExists (@SystemDir&"\msvcp71.dll") OR NOT FileExists (@SystemDir&"\msvcr71.dll") OR NOT FileExists (@SystemDir&"\msvcrt.dll") Then
          Local $msv = 1
        Else
          Local $msv = 0
        EndIf
      EndIf

  If @OSArch = "x64" Then
	If NOT FileExists (@SystemDir&"\msvcp80.dll") OR NOT FileExists (@SystemDir&"\msvcr80.dll") Then
	  Local $msv = 2
	Else
	  Local $msv = 0
	EndIf
  EndIf
  
  RunWait("cmd /c set VBOX_USER_HOME=%CD%\data\.VirtualBox & .\"& $arch &"\VBoxManage.exe unregistervm " & $DefaultVm, @ScriptDir, @SW_HIDE)
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-1/8 cmd /c set VBOX_USER_HOME=%CD%\data\.VirtualBox & .\"& $arch &"\VirtualBox.exe unregistervm " & $DefaultVm)
	sleep(2000)
  EndIf 
  $initPercent = 70
  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","完成ProcessClose VBoxSVC.exe",0)
  RunWait ($arch&"\VBoxSVC.exe /unregserver", @ScriptDir, @SW_HIDE)
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-2/8 " & $arch&"\VBoxSVC.exe /unregserver")
	sleep(2000)
  EndIf  
  $initPercent = 65
  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","完成VBoxSVC.exe反注册",0)
  RunWait (@SystemDir&"\regsvr32.exe /S /U "& $arch &"\VBoxC.dll", @ScriptDir, @SW_HIDE)
  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","完成VBoxC.dll反注册",0)
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-3/8 " & @SystemDir&"\regsvr32.exe /S /U "& $arch &"\VBoxC.dll")
	sleep(2000)
  EndIf  
  $initPercent = 60
  If $DRV = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","停止VBoxDRV服务",0)
	RunWait ("sc stop VBoxDRV", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-4/8 sc stop VBoxDRV")
	sleep(2000)
  EndIf  
 $initPercent = 55

  If $USB = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","停止VBoxUSB服务",0)
	RunWait ("sc stop VBoxUSB", @ScriptDir, @SW_HIDE)
	If @OSArch = "x86" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x86 devcon_x86删除USB服务",0)
	  RunWait (@ScriptDir &"\data\tools\devcon_x86.exe remove ""USB\VID_80EE&PID_CAFE""", @ScriptDir, @SW_HIDE)
	EndIf
	If @OSArch = "x64" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x64 devcon_x64删除USB服务",0)
	  RunWait (@ScriptDir &"\data\tools\devcon_x64.exe remove ""USB\VID_80EE&PID_CAFE""", @ScriptDir, @SW_HIDE)
	EndIf
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxUSB.sys",0)
	FileDelete (@SystemDir&"\drivers\VBoxUSB.sys")
  EndIf
  $initPercent = 50
  If $MON = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","停止VBoxUSBMon服务",0)
	RunWait ("sc stop VBoxUSBMon", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-5/8 sc stop VBoxUSBMon")
	sleep(2000)
  EndIf 
  $initPercent = 45
  If $ADP = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","停止VBoxNetAdp服务",0)
	RunWait ("sc stop VBoxNetAdp", @ScriptDir, @SW_HIDE)
	If @OSArch = "x86" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x86 devcon_x86删除NetAdp服务",0)
	  RunWait (@ScriptDir &"\data\tools\devcon_x86.exe remove ""sun_VBoxNetAdp""", @ScriptDir, @SW_HIDE)
	EndIf
	If @OSArch = "x64" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x64 devcon_x64删除NetAdp服务",0)
	  RunWait (@ScriptDir &"\data\tools\devcon_x64.exe remove ""sun_VBoxNetAdp""", @ScriptDir, @SW_HIDE)
	EndIf
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxNetAdp.sys",0)
	FileDelete (@SystemDir&"\drivers\VBoxNetAdp.sys")
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-6/8 remove sun_VBoxNetAdp")
	sleep(2000)
  EndIf 
  $initPercent = 40
  If $NET = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","停止VBoxNetFlt服务",0)
	RunWait ("sc stop VBoxNetFlt", @ScriptDir, @SW_HIDE)
	If @OSArch = "x86" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x86 snetcfg_x86删除NetFlt服务",0)
	  RunWait (@ScriptDir&"\data\tools\snetcfg_x86.exe -v -u sun_VBoxNetFlt", @ScriptDir, @SW_HIDE)
	EndIf
	If @OSArch = "x64" Then
	  ;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x64 snetcfg_x64删除NetFlt服务",0)
	  RunWait (@ScriptDir&"\data\tools\snetcfg_x64.exe -v -u sun_VBoxNetFlt", @ScriptDir, @SW_HIDE)
	EndIf
	RunWait (@SystemDir&"\regsvr32.exe /S /U "&@SystemDir&"\VBoxNetFltNobj.dll", @ScriptDir, @SW_HIDE)
	RunWait ("sc delete VBoxNetFlt", @ScriptDir, @SW_HIDE)
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxNetFltNobj.dll",0)
	FileDelete (@SystemDir&"\VBoxNetFltNobj.dll")
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxNetFlt.sys",0)
	FileDelete (@SystemDir&"\drivers\VBoxNetFlt.sys")
  EndIf

  If FileExists (@ScriptDir&"\"& $arch &"\") AND FileExists (@ScriptDir&"\vboxadditions\") Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除vboxadditions",0)
	DirMove (@ScriptDir&"\"& $arch &"\doc", @ScriptDir&"\vboxadditions\", 1)
	DirMove (@ScriptDir&"\"& $arch &"\ExtensionPacks", @ScriptDir&"\vboxadditions\", 1)
	DirMove (@ScriptDir&"\"& $arch &"\nls", @ScriptDir&"\vboxadditions\", 1)
	FileMove (@ScriptDir&"\"& $arch &"\*.iso", @ScriptDir&"\vboxadditions\guestadditions\", 9)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-7/8 remove sun_VBoxNetFlt")
	sleep(2000)
  EndIf 
  $initPercent = 35
  If $msv = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x86删除msvc*.dll",0)
	FileDelete (@SystemDir&"\msvcp71.dll")
	FileDelete (@SystemDir&"\msvcr71.dll")
	FileDelete (@SystemDir&"\msvcrt.dll")
  EndIf
  
  If $msv = 2 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","x64删除msvc*.dll",0)
	FileDelete (@SystemDir&"\msvcp80.dll")
	FileDelete (@SystemDir&"\msvcr80.dll")
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:3-8/8 remove msvc*.dll")
	sleep(2000)
  EndIf 
  $initPercent = 30
  If $DRV = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxDRV服务",0)
	RunWait ("sc delete VBoxDRV", @ScriptDir, @SW_HIDE)
  EndIf
   If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:4-1/5 sc delete VBoxDRV")
	sleep(2000)
  EndIf 
  $initPercent = 25
  If $USB = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxUSB服务",0)
	RunWait ("sc delete VBoxUSB", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:4-2/5 sc delete VBoxUSB")
	sleep(2000)
  EndIf  
  $initPercent = 20
  If $MON = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxDUSBMon服务",0)
	RunWait ("sc delete VBoxUSBMon", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:4-3/5 sc delete VBoxUSBMon")
	sleep(2000)
  EndIf 
  $initPercent = 15
  If $ADP = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxNetAdp服务",0)
	RunWait ("sc delete VBoxNetAdp", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:4-4/5 sc delete VBoxNetAdp")
	sleep(2000)
  EndIf 
  $initPercent = 10
  If $NET = 1 Then
	;MsgBox($MB_SYSTEMMODAL,"DEBUG提示","删除VBoxNetFlt服务",0)
	RunWait ("sc delete VBoxNetFlt", @ScriptDir, @SW_HIDE)
  EndIf
  If $debugMode = 1 Then
	GUICtrlSetData($debugLabel, "进度提示:4-5/5 sc delete VBoxNetFlt")
	sleep(2000)
  EndIf 
  $initPercent = 5
  ; set create vm flag to 0
  IniWrite($var1, "afc", "createvm", 0)
  ; set vm adapter to nil
  IniWrite($var1, "afc", "adapter", "")
  ;SplashOff ()
  $initPercent = 0
Break (1)
Exit

; Initialize process bar window
Func InitProcessBar()
	$procBarForm = GUICreate("清除AFC", 720, 260, -1, -1, BitOR($WS_DLGFRAME,$WS_BORDER), $WS_EX_APPWINDOW)
	GUISetIcon(@ScriptDir&"\data\settings\AFC-VM-min.ico", -1)
	GUISetCursor (2)
	GUISetFont(16, 400, 0, "华文细黑")
	;GUISetBkColor(0x00B0F0)
	GUISetBkColor(0x0072D8)

	;$proBarPic = GUICtrlCreatePic(@ScriptDir&"\data\settings\AFC-Bar.jpg",-1, -1, 720,260)
	;GUISetState(@SW_SHOW)

	$procBarLabel = GUICtrlCreateLabel("正在清除AFC组件... ...", 50, 29, 220, 36)
	GUICtrlSetBkColor($procBarLabel,0x0072D8)
	GUICtrlSetStyle($procBarLabel, 0x01, 0x00000020); 0x01 - $SS_CENTER, 0x00000020 - $WS_EX_TRANSPARENT
	GUICtrlSetColor($procBarLabel, 0xFFFFFF)
	
	If $debugMode = 1 Then
		$debugLabel = GUICtrlCreateLabel("进度提示", 55, 60, 601, 20)
		GUICtrlSetBkColor($debugLabel,0x0072D8)
		GUICtrlSetStyle($debugLabel, 0, 0x00000020); 0 - $SS_LEFT, 0x00000020 - $WS_EX_TRANSPARENT
		GUICtrlSetColor($debugLabel, 0xFFCF35)
		GUICtrlSetFont(-1, 10, 400, 4, "Comic Sans Ms")
	EndIf
	$procBarProgress = GUICtrlCreateProgress(50, 88, 601, 41, 0x01)
	GUICtrlSetBkColor($procBarProgress, 0xFFFFFF)
	GUICtrlSetColor(-1, 0xFFCF35)
	GUICtrlSetData(-1, 100)
	GUISetState(@SW_SHOW)	
EndFunc
Func RefreshProgress()
	GUICtrlSetData($procBarProgress, $initPercent)
	
	If $progressCounter >= 100 Then
		AdlibUnRegister("RefreshProgress")
	EndIf
EndFunc