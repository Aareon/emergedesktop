; -------------------------------------------------------------------
; help >>> Modern UI defines...
; -------------------------------------------------------------------
!include LogicLib.nsh
!include MUI2.nsh

; -------------------------------------------------------------------
; help >>> General NAME and Versioning
; -------------------------------------------------------------------
Name "Emerge Desktop 4.2"
!define FIELD1 $R1
!define FIELD2 $R2
!define FIELD3 $R3
!define FIELD4 $R4
!define EMERGERUNNING $R8
!define MUI_CUSTOMFUNCTION_ABORT ".onCustomAbort"

Var Dialog
Var Label1
Var Label2
Var Label3
Var CheckBox1
Var CheckBox2
Var CheckBox3
Var CheckBox4

; -------------------------------------------------------------------
; help >>> Set compression
; -------------------------------------------------------------------
SetCompressor lzma

; -------------------------------------------------------------------
; help >>> installer pages we're gonna use
; -------------------------------------------------------------------
!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING
!define MUI_HEADERBITMAP_RIGHT
!define MUI_CUSTOMPAGECOMMANDS
!define MUI_UNCUSTOMPAGECOMMANDS

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license.txt"
Page custom nsDialogOptions nsDialogOptionsLeave
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
UninstPage custom un.nsDialogOptions un.nsDialogOptionsLeave
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

; -------------------------------------------------------------------
; help >>> when installation finishes, allow user to launch docs.
; -------------------------------------------------------------------
Icon "emerge.ico"
UninstallIcon "unemerge.ico"
OutFile "EmergeDesktop-4.2.exe"

; -------------------------------------------------------------------
; help >>> Branding the installer makes it nice an unique... ;)
; -------------------------------------------------------------------
BrandingText "�2004 - 2008 Emerge Desktop Development Team"

; -------------------------------------------------------------------
; help >>> NSIS 2.0 modern ui with XP style please...
; -------------------------------------------------------------------
XPStyle on

; -------------------------------------------------------------------
; help >>> Good setting for all installers, overwrites older files
;          which is quite cool if you're updating later!
; -------------------------------------------------------------------
SetOverwrite ifnewer

; -------------------------------------------------------------------
; help >>> Where do we want our app to go today? Aah, the install dir!
; -------------------------------------------------------------------
InstallDir "$PROGRAMFILES\Emerge Desktop"

; -------------------------------------------------------------------
; help >>> FILES TO INSTALL STUFF
; -------------------------------------------------------------------
Section "emergeCore" SecemergeCore
SectionIn RO
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeCore.exe"
SetOutPath "$INSTDIR\scripts"
IfFileExists "$INSTDIR\scripts\hide.pl" +2
File "..\Source\scripts\hide.pl"
IfFileExists "$INSTDIR\scripts\Perl_HowTo.txt" +2
File "..\Source\scripts\Perl_HowTo.txt"
IfFileExists "$INSTDIR\scripts\hide.pyw" +2
File "..\Source\scripts\hide.pyw"
IfFileExists "$INSTDIR\scripts\Python_HowTo.txt" +2
File "..\Source\scripts\Python_HowTo.txt"
IfFileExists "$INSTDIR\scripts\hide.rb" +2
File "..\Source\scripts\hide.rb"
Push $R0
${If} ${FIELD1} == ${BST_CHECKED}
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  StrCmp $0 "0" +2
  SetRegView 64
  ReadRegStr $R0 HKCU "Software\Microsoft\Windows NT\CurrentVersion\Winlogon" "Shell"
  StrCmp $R0 "$INSTDIR\emergeCore.exe" CORE_END
  IfFileExists "$INSTDIR\emergeCore.exe" 0 CORE_END
  WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\Winlogon" "Shell" "$INSTDIR\emergeCore.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\Winlogon" "Shell" "explorer.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\IniFileMapping\system.ini\boot" "Shell" "USR:Software\Microsoft\Windows NT\CurrentVersion\Winlogon"
  WriteRegDword HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer" "DesktopProcess" 1
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer" "BrowseNewProcess" "yes"
  SetRebootFlag true
${EndIf}
CORE_END:
Pop $R0
SectionEnd

SubSection "Core Applets" SecCoreApplets

Section "emergeDesktop" SecemergeDesktop
SectionIn RO
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeDesktop.exe"
SectionEnd

Section "emergeTasks" SecemergeTasks
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeTasks.exe"
SectionEnd

Section "emergeTray" SecemergeTray
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeTray.exe"
SectionEnd

SubSectionEnd

SubSection "Additional Applets" SecAdditionalApplets

Section "emergeCommand" SecemergeCommand
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeCommand.exe"
CreateDirectory "$PROFILE\Emerge Desktop\files"
IfFileExists "$PROFILE\Emerge Desktop\files\cmd.txt" +3
SetOutPath "$PROFILE\Emerge Desktop\files"
File "..\Source\files\cmd.txt"
IfFileExists "$INSTDIR\files\cmd.txt" +3
SetOutPath "$INSTDIR\files"
File "..\Source\files\cmd.txt"
SectionEnd

Section "emergeHotkeys" SecemergeHotkeys
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeHotkeys.exe"
SectionEnd

Section "emergeLauncher" SecemergeLauncher
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeLauncher.exe"
SectionEnd

Section "emergeVWM" SecemergeVWM
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeVWM.exe"
SectionEnd

Section "emergePower" SecemergePower
SetOutPath "$INSTDIR"
File "..\Source\bin\emergePower.exe"
SectionEnd

Section "emergeSysMon" SecemergeSysMon
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeSysMon.exe"
SectionEnd

SubSectionEnd

SubSection "Utilities" SecUtilities

Section "emerge" Secemerge
SetOutPath "$INSTDIR"
File "..\Source\bin\emerge.exe"
SectionEnd

SubSectionEnd

Section "-Libraries"
SetOutPath "$INSTDIR"
File "..\Source\bin\emergeLib.dll"
File "..\Source\bin\emergeGraphics.dll"
File "..\Source\bin\emergeAppletEngine.dll"
File "..\Source\bin\emergeSchemeEngine.dll"
File "..\Source\bin\emergeBaseClasses.dll"
SectionEnd

Section "-Schemes"
CreateDirectory "$PROFILE\Emerge Desktop\schemes"
SetOutPath "$PROFILE\Emerge Desktop\schemes"
IfFileExists "$PROFILE\Emerge Desktop\schemes\Default.eds" +2
File "..\Source\schemes\Default.eds"
SetOutPath "$INSTDIR\schemes"
IfFileExists "$INSTDIR\schemes\Default.eds" +2
File "..\Source\schemes\Default.eds"
SectionEnd

; -------------------------------------------------------------------
; help >>> add all files needed... here: documentation
; -------------------------------------------------------------------
Section "Documentation" SecDocumentation
SetOutPath "$INSTDIR\documentation\"
File "..\Documentation\Emerge Desktop.chm"
SectionEnd

; -------------------------------------------------------------------
; help >>> STARTMENU STUFF
; -------------------------------------------------------------------

Section "Start Menu Shortcuts" SecShortcuts
  SetShellVarContext current
  CreateDirectory "$SMPROGRAMS\Emerge Desktop"

; -------------------------------------------------------------------
; help >>> add more shortcuts as needed.
; -------------------------------------------------------------------
  CreateShortCut "$SMPROGRAMS\Emerge Desktop\Emerge Desktop Documentation.lnk" "$INSTDIR\documentation\Emerge Desktop.chm" "" "$INSTDIR\documentation\Emerge Desktop.chm" 0
  CreateShortCut "$SMPROGRAMS\Emerge Desktop\Emerge Desktop Shell Changer.lnk" "$INSTDIR\emergeCore.exe" "/shellchanger" "$INSTDIR\emergeCore.exe" 0
  CreateShortCut "$SMPROGRAMS\Emerge Desktop\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
SectionEnd

; -------------------------------------------------------------------
; help >>> UNINSTALL STUFF
; -------------------------------------------------------------------
Section Uninstall
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  StrCmp $0 "0" +2
  SetRegView 64
  DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\Winlogon" "Shell"
  WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\Winlogon" "Shell" "explorer.exe"
  SetRebootFlag true

; -------------------------------------------------------------------
; help >>> remove scripts, if desired
; -------------------------------------------------------------------
  ${If} ${FIELD2} == ${BST_UNCHECKED}
    Delete "$PROFILE\Emerge Desktop\scripts\*"
    RMDir "$PROFILE\Emerge Desktop\scripts"
  ${EndIf}

; -------------------------------------------------------------------
; help >>> remove files, if desired
; -------------------------------------------------------------------
  ${If} ${FIELD3} == ${BST_UNCHECKED}
    Delete "$PROFILE\Emerge Desktop\files\*"
    RMDir "$PROFILE\Emerge Desktop\files"
  ${EndIf}

; -------------------------------------------------------------------
; help >>> remove schemes, if desired
; -------------------------------------------------------------------
  ${If} ${FIELD4} == ${BST_UNCHECKED}
    Delete "$PROFILE\Emerge Desktop\schemes\*"
    RMDir "$PROFILE\Emerge Desktop\schemes"
  ${EndIf}

; -------------------------------------------------------------------
; help >>> remove registry entries, if desired
; -------------------------------------------------------------------
  ${If} ${FIELD1} == ${BST_UNCHECKED}
    DeleteRegKey HKCU "SOFTWARE\Emerge Desktop"
  ${EndIf}

; -------------------------------------------------------------------
; help >>> uninstall Emerge Desktop
; -------------------------------------------------------------------
  Call un.CloseCore
  SetShellVarContext current
  Delete "$SMPROGRAMS\Emerge Desktop\*.lnk"
  RMDir "$SMPROGRAMS\Emerge Desktop"
  Delete "$INSTDIR\documentation\Emerge Desktop.chm"
  RMDir "$INSTDIR\documentation"
  Delete "$INSTDIR\emerge.exe"
  Delete "$INSTDIR\emergeCommand.exe"
  Delete "$INSTDIR\emergeDesktop.exe"
  Delete "$INSTDIR\emergeHotkeys.exe"
  Delete "$INSTDIR\emergeLauncher.exe"
  Delete "$INSTDIR\emergeTasks.exe"
  Delete "$INSTDIR\emergeTray.exe"
  Delete "$INSTDIR\emergeCore.exe"
  Delete "$INSTDIR\emergeVWM.exe"
  Delete "$INSTDIR\emergePower.exe"
  Delete "$INSTDIR\emergeSysMon.exe"
  Delete "$INSTDIR\emergeLib.dll"
  Delete "$INSTDIR\emergeAppletEngine.dll"
  Delete "$INSTDIR\emergeSchemeEngine.dll"
  Delete "$INSTDIR\emergeGraphics.dll"
  Delete "$INSTDIR\emergeBaseClasses.dll"
  Delete "$INSTDIR\uninst.exe"
  RMDir "$INSTDIR"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Emerge Desktop"
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${Secemerge} "emerge is a command line utility that can be used to execute Emerge Desktop Internal Commands by other non-Emerge Desktop applications."
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeCore} "emergeCore is the core of Emerge Desktop.  It is required for setting Emerge Desktop as the default shell."
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeCommand} "Provides a clock as well as method of entering commands to be executed"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeHotkeys} "'Hotkey' application laucher as well as interfacing to Emerge Desktop itself"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeTasks} "Provides an interface for managing the tasks on the system"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeTray} "A 'System Tray' for Emerge Desktop"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeVWM} "A 'Virtual Window Manager' for Emerge Desktop"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergePower} "A battery charge monitor for Emerge Desktop"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeSysMon} "A CPU and Memory monitor for Emerge Desktop"
!insertmacro MUI_DESCRIPTION_TEXT ${SecDocumentation} "All the documentation for the Emerge Desktop applets and applications"
!insertmacro MUI_DESCRIPTION_TEXT ${SecShortcuts} "Provides a shortcut in the start menu to the Emerge Desktop documentation"
!insertmacro MUI_DESCRIPTION_TEXT ${SecCoreApplets} "Emerge Desktop Core applets"
!insertmacro MUI_DESCRIPTION_TEXT ${SecAdditionalApplets} "Additional Emerge Desktop applets"
!insertmacro MUI_DESCRIPTION_TEXT ${SecUtilities} "Utilities to assist in using Emerge Desktop"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeLauncher} "Application Launcher"
!insertmacro MUI_DESCRIPTION_TEXT ${SecemergeDesktop} "Provides the desktop menus"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; -------------------------------------------------------------------
; help >>> Functions
; -------------------------------------------------------------------

; .onInit
;
; Read the install opions and versify the user has Windows 2000 or above
Function .onInit
Call CheckWindowsVersion
StrCpy ${EMERGERUNNING} "0"
Push $R0
FindWindow $R0 "emergeCoreClass"
IntCmp $R0 0 +5
Pop $R0
StrCpy ${EMERGERUNNING} "1"
MessageBox MB_OKCANCEL|MB_SETFOREGROUND|MB_ICONQUESTION "Emerge Desktop is currently running.  Would you like the$\r$\n installer to quit Emerge Desktop so the install can continue?" IDCANCEL +1 IDOK +2
Abort
Call CloseCore
FunctionEnd

;Function un.onUninstSuccess
;IfRebootFlag +1 SKIP_REBOOT
;MessageBox MB_YESNO "The system must be rebooted for the changes to take effect.  Do you want to reboot now?" IDYES +1 IDNO SKIP_REBOOT
;Reboot
;SKIP_REBOOT:
;FunctionEnd

; .onInstSuccess
;
; Write uninstaller after successful installation
Function .onInstSuccess
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Emerge Desktop" "DisplayName" "Emerge Desktop, replacement shell (remove only)"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Emerge Desktop" "UninstallString" "$INSTDIR\uninst.exe"
WriteUnInstaller "$INSTDIR\uninst.exe"
IfRebootFlag +2
call RestartCore
FunctionEnd

Function .onCustomAbort
call RestartCore
FunctionEnd

Function .onInstFailed
call RestartCore
FunctionEnd

Function RestartCore
IfFileExists "$INSTDIR\emergeCore.exe" +4
MessageBox MB_OK "The installer could not find emergeCore.  The installer will now start Windows Explorer."
Exec "exporer.exe"
Goto +4
StrCmp ${EMERGERUNNING} "1" +1 +3
MessageBox MB_OK "The installer will now restart Emerge Desktop.  Please note that it is possible not all tray icons will reappear."
Exec "$INSTDIR\emergeCore.exe"
FunctionEnd

; CheckWindowsVersion
;
; Based on GetWindowsVersion
;
; Usage:
;   Call CheckWindowsVersion
Function CheckWindowsVersion
Push $R0
ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
StrCmp $R0 "" 0 lbl_winnt
; we are not NT.
Goto lbl_error

lbl_winnt:
StrCmp $R0 '3' lbl_error
StrCmp $R0 '4' lbl_error
Goto lbl_done

lbl_error:
MessageBox MB_OK|MB_ICONEXCLAMATION "Emerge Desktop Requires Windows 2000 or higher"
Abort
lbl_done:
Pop $R0
FunctionEnd

Function CloseCore
  Push $R0
LOOP:
  FindWindow $R0 "emergeCoreClass"
  IntCmp $R0 0 +5
    SendMessage $R0 1028 1 100
    SendMessage $R0 2028 1 100
    Sleep 100
    Goto LOOP
  Pop $R0
FunctionEnd

Function un.CloseCore
  Push $R0
LOOP:
  FindWindow $R0 "emergeCoreClass"
  IntCmp $R0 0 +6
  SetRebootFlag true
        SendMessage $R0 1028 1 100
        SendMessage $R0 2028 1 100
        Sleep 100
        Goto LOOP
  Pop $R0
FunctionEnd

Function nsDialogOptions
  nsDialogs::Create /NOUNLOAD 1018
  Pop $Dialog

  ${If} $Dialog == error
    Abort
  ${EndIf}

  ${NSD_CreateLabel} 0 10u 100% 20u "Emerge Desktop has the ability to replace the default windows desktop.  Would you like to replace the default windows desktop?"
  Pop $Label1

  ${NSD_CreateCheckBox} 0 40u 100% 12u "Set Emerge Desktop as the default shell for the current user"
  Pop $CheckBox1

  nsDialogs::Show
FunctionEnd

Function nsDialogOptionsLeave
  ${NSD_GetState} $CheckBox1 ${FIELD1}
FunctionEnd

Function un.nsDialogOptions
  nsDialogs::Create /NOUNLOAD 1018
  Pop $Dialog

  ${If} $Dialog == error
    Abort
  ${EndIf}

  ${NSD_CreateLabel} 0 5u 100% 25u "The uninstallation process will remove both the Emerge Desktop core as well as all the applets.  It will also restore the default Windows desktop."
  Pop $Label1

  ${NSD_CreateLabel} 0 35u 100% 25u "By default the uninstaller will remove all the Emerge Desktop configuration information.  If you would like to save your configuration information for a future installation, please check the box below:"
  Pop $Label2

  ${NSD_CreateCheckBox} 0 70u 100% 12u "Save Configuration"
  Pop $CheckBox1

  ${NSD_CreateLabel} 0 90u 100% 20u "The uninstaller by default also removes all scripts, files and schemes.  If you would like to save your scripts, files or schemes for later use, please check the appropriate box below:"
  Pop $Label3

  ${NSD_CreateCheckBox} 0 115u 100u 12u "Save Scripts"
  Pop $CheckBox2

  ${NSD_CreateCheckBox} 100u 115u 100u 12u "Save Files"
  Pop $CheckBox3

  ${NSD_CreateCheckBox} 200u 115u 100u 12u "Save Schemes"
  Pop $CheckBox4

  nsDialogs::Show
FunctionEnd

Function un.nsDialogOptionsLeave
  ${NSD_GetState} $CheckBox1 ${FIELD1}
  ${NSD_GetState} $CheckBox2 ${FIELD2}
  ${NSD_GetState} $CheckBox3 ${FIELD3}
  ${NSD_GetState} $CheckBox4 ${FIELD4}
FunctionEnd
