; Language       : multilanguage
; Author         : Michael Meyer (michaelm_007) et al.
; e-Mail         : email.address@gmx.de
; License        : http://creativecommons.org/licenses/by-nc-sa/3.0/
; Version        : 6.4.9.0
; Download       : http://www.vbox.me
; Support        : http://www.win-lite.de/wbb/index.php?page=Board&boardID=153

#AutoIt3Wrapper_Res_Fileversion=6.4.9.0
#AutoIt3Wrapper_Res_ProductVersion=6.4.9.0
#AutoIt3Wrapper_Icon=VirtualBox.ico
#AutoIt3Wrapper_Compression=4
#AutoIt3Wrapper_Compile_both=Y

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

#NoTrayIcon
#RequireAdmin

Opt ("GUIOnEventMode", 1)
Opt ("TrayAutoPause", 0)
Opt ("TrayMenuMode", 11)
Opt ("TrayOnEventMode", 1)

TraySetClick (16)
TraySetState ()
TraySetToolTip ("AFi-Controller")

Global $version = "6.4.9.0"
Global $var1 = @ScriptDir&"\data\settings\settings.ini"
Global $var2 = @ScriptDir&"\data\language\"
Global $lng = IniRead ($var1, "language", "key", "NotFound")
Global $pwd = @ScriptDir
Global $updateUrl = IniRead (@ScriptDir&"\data\settings\vboxinstall.ini", "download", "update", "NotFound")

Global $new1 = 0, $new2 = 0

;Controls for process bar
Global $procBarForm, $procBarLabel,$procBarButton, $afcHostIp, $DefaultVm

If FileExists (@ScriptDir&"\data\settings\AFC-VM.jpg") Then
	SplashImageOn ("AFi-Controller", @ScriptDir&"\data\settings\AFC-VM.jpg", 480, 360, -1, -1, 1)
Else
	SplashTextOn ("AFi-Controller", IniRead ($var2 & $lng &".ini", "messages", "06", "NotFound"), 220, 40, -1, -1, 1, "arial", 12)
EndIf
sleep(5000)
SplashOff ()
sleep(1000)

InitTray()

InitProcessBar()

Func InitTray()
	TrayCreateItem ("显示主界面")
	TrayItemSetOnEvent(-1, "ShowMainFrame")
	TrayCreateItem ("隐藏主界面")
	TrayItemSetOnEvent(-1, "HideMainFrame")
	TrayCreateItem ("")
	TrayCreateItem ("退出")
	TrayItemSetOnEvent (-1, "ExitAFCLoad")
	TraySetToolTip ("右键查看菜单")
	TrayTip("", "鼠标右键查看菜单", 5)
EndFunc

Func ShowMainFrame()
	Opt ("WinTitleMatchMode", 3)
	WinSetState ("启动AFC", "", BitAND (@SW_SHOW, @SW_RESTORE))
EndFunc

Func HideMainFrame()
	Opt ("WinTitleMatchMode", 3)
	WinSetState ("启动AFC", "",  @SW_HIDE))
EndFunc

Func ExitAFCLoad()
	Exit
EndFunc

; Initialize process bar window
Func InitProcessBar()
	$procBarForm = GUICreate("启动AFC", 720, 260, -1, -1, BitOR($WS_DLGFRAME,$WS_BORDER), $WS_EX_APPWINDOW)
	GUISetIcon(@ScriptDir&"\data\settings\AFC-VM-min.ico", -1)
	GUISetCursor (2)
	GUISetFont(16, 400, 0, "华文细黑")
	;GUISetBkColor(0x00B0F0)
	GUISetBkColor(0x0072D8)
	GUISetState(@SW_SHOW)
	
	;$proBarPic = GUICtrlCreatePic(@ScriptDir&"\data\settings\AFC-Bar.jpg",-1, -1, 720,260)
	;GUISetState(@SW_SHOW)

	$procBarLabel = GUICtrlCreateLabel("欢迎使用AFi Ctroller", 20, 60, 220, 36)
	GUICtrlSetBkColor($procBarLabel,0x0072D8)
	GUICtrlSetStyle($procBarLabel, 0x01, 0x00000020); 0x01 - $SS_CENTER, 0x00000020 - $WS_EX_TRANSPARENT
	GUICtrlSetColor($procBarLabel, 0xFFFFFF)
	GUISetState(@SW_SHOW)
	
	$procBarButton = GUICtrlCreateButton("启动AFC开发版", 385, 140, 185, 41)
	GUICtrlSetBkColor($procBarButton, 0x0070C0)
	GUICtrlSetColor($procBarButton, 0xFFFFFF)
	GUICtrlSetOnEvent ($procBarButton, "StartAFCWebRnd")
	GUICtrlSetState($procBarButton, BitOR($GUI_SHOW, $GUI_ENABLE, $GUI_DEFBUTTON))
	GUISetState(@SW_SHOW)
	
	$procBarButton = GUICtrlCreateButton("启动AFC发行版", 150, 140, 185, 41)
	GUICtrlSetBkColor($procBarButton, 0x0070C0)
	GUICtrlSetColor($procBarButton, 0xFFFFFF)
	GUICtrlSetOnEvent ($procBarButton, "StartAFCWeb")
	GUICtrlSetState($procBarButton, BitOR($GUI_SHOW, $GUI_ENABLE, $GUI_DEFBUTTON))
	GUISetState(@SW_SHOW)
	ProcessWaitClose ("VirtualBox.exe")
    ProcessWaitClose ("VBoxManage.exe")
	ProcessWaitClose ("VBoxHeadless.exe")
	ProcessWaitClose ("AFC-load.exe")
EndFunc

; Get VM guest ip
Func VMGetGuestip()
	Local $GuestIpFile = "guestip.txt"
	Local $arch = "app32"

	Run("cmd /c .\" & $arch & "\VBoxManage.exe guestproperty get " & $DefaultVm & " guestip > " & $GuestIpFile, @ScriptDir, @SW_HIDE)
	sleep(500)
	Local $FileHdl = FileOpen(".\" & $GuestIpFile, $FO_READ)

	If $FileHdl <> -1 then
		Local $FileCtent = FileRead($FileHdl)
		
		If StringInStr($FileCtent, "Value: ") <> 0 then
			Local $nParams = StringSplit($FileCtent, ": ", $STR_ENTIRESPLIT)
			FileClose($FileHdl)
			FileDelete($GuestIpFile)
			$afcHostIp = $nParams[2]
		Endif
	Endif 
EndFunc

; Load AFC web
Func StartAFCWeb()
	$DefaultVm = "AFC"
	VMGetGuestip()

	Local $g_oIE = _IECreate("http://" & $afcHostIp, 0, 1, 1, 1)
	_IELoadWait($g_oIE)
EndFunc

Func StartAFCWebRnd()
	$DefaultVm = "AFC-rnd"
	VMGetGuestip()

	Local $g_oIE = _IECreate("http://" & $afcHostIp, 0, 1, 1, 1)
	_IELoadWait($g_oIE)
EndFunc