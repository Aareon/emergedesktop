
/*!
  @file emergeLib.h
  @brief export header for emergeLib
  @author The Emerge Desktop Development Team

  @attention This file is part of Emerge Desktop.
  @attention Copyright (C) 2004-2010  The Emerge Desktop Development Team

  @attention Emerge Desktop is free software; you can redistribute it and/or
  modify  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  @attention Emerge Desktop is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  @attention You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef __EMERGELIB_H
#define __EMERGELIB_H

#ifdef EMERGELIB_EXPORTS
#undef DLL_EXPORT
/*!
  @def DLL_EXPORT
  @brief Define for export if EMERGELIB_EXPORTS is defined
  */
#define DLL_EXPORT  __declspec(dllexport)
#else
#ifndef DLL_EXPORT
/*!
  @def DLL_EXPORT
  @brief Define for import if EMERGELIB_EXPORTS is not defined
  */
#define DLL_EXPORT  __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

/*!
  @def UNICODE
  @brief Define to use UNICODE versions of functions
  */
#define UNICODE 1

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <reason.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include "../tinyxml/tinyxml.h"

/*!
  @def MAX_LINE_LENGTH
  @brief Define the maximum size a string can be
  */
#define MAX_LINE_LENGTH 4096

// Emerge Desktop application defines
static const UINT EMERGE_REGISTER = RegisterWindowMessage(TEXT("EmergeRegister"));
static const UINT EMERGE_UNREGISTER = RegisterWindowMessage(TEXT("EmergeUnregister"));
static const UINT EMERGE_NOTIFY = RegisterWindowMessage(TEXT("EmergeNotify"));
static const UINT EMERGE_DISPATCH = RegisterWindowMessage(TEXT("EmergeDispatch"));
static const UINT EMERGE_MESSAGE = RegisterWindowMessage(TEXT("EmergeMessage"));

// Emerge exit types
#define EMERGE_SUSPEND      100
#define EMERGE_HIBERNATE    101
#define EMERGE_DISCONNECT   102
#define EMERGE_HALT         103
#define EMERGE_REBOOT       104
#define EMERGE_LOGOFF       105

// Emerge Desktop function defines
#define EMERGE_CORE     0x01
#define EMERGE_VWM      0x02

// EMERGE_CORE Messages
#define CORE_QUIT           100
#define CORE_SHOW           101
#define CORE_HIDE           102
#define CORE_LAUNCH         103
#define CORE_SETTINGS       104
#define CORE_MENU           105
#define CORE_SHELL          106
#define CORE_RIGHTMENU      107
#define CORE_MIDMENU        108
#define CORE_RUN            109
#define CORE_SHUTDOWN       110
#define CORE_EMPTYBIN       111
#define CORE_LOGOFF         112
#define CORE_REBOOT         113
#define CORE_HALT           114
#define CORE_SUSPEND        115
#define CORE_HIBERNATE      116
#define CORE_DESKTOP        117
#define CORE_REFRESH        118
#define CORE_DISCONNECT     119
#define CORE_ABOUT          120
#define CORE_RECONFIGURE    121
#define CORE_THEMESELECT    122
#define CORE_WRITESETTINGS  123
#define CORE_LEFTMENU       124

// EMERGE_VWM Messages
#define VWM_UP          200
#define VWM_DOWN        201
#define VWM_LEFT        202
#define VWM_RIGHT       203
#define VWM_GATHER      204
#define VWM_1           211
#define VWM_2           212
#define VWM_3           213
#define VWM_4           214
#define VWM_5           215
#define VWM_6           216
#define VWM_7           217
#define VWM_8           218
#define VWM_9           219

#define RSH_UNREGISTER  0
#define RSH_REGISTER    1
#define RSH_TASKMGR     3

#define ELAV_MUTE       0
#define ELAV_VOLUMEUP   1
#define ELAV_VOLUMEDOWN 2

#define ELCB_BACK       0

#define ELMB_ICONWARNING  0x00
#define ELMB_ICONQUESTION 0x01
#define ELMB_ICONERROR    0x02

#define ELMB_OK           0x0100
#define ELMB_YESNO        0x0200

#define ELMB_MODAL        0x010000

/*!
  @def COMMAND_RUN
  @brief Internal Command define for Run
  */
#define COMMAND_RUN             1
/*!
  @def COMMAND_QUIT
  @brief Internal Command define for Quit
  */
#define COMMAND_QUIT            2
/*!
  @def COMMAND_LOGOFF
  @brief Internal Command define for Logoff
  */
#define COMMAND_LOGOFF          3
/*!
  @def COMMAND_SHUTDOWN
  @brief Internal Command define for Shutdown
  */
#define COMMAND_SHUTDOWN        4
/*!
  @def COMMAND_LOCK
  @brief Internal Command define for Lock
  */
#define COMMAND_LOCK            5

/*!
  @def ELVI_2K
  @brief ELVersionInfo define for Windows 2000
  */
#define ELVI_2K     1
/*!
  @def ELVI_XP
  @brief ELVersionInfo define for Windows XP
  */
#define ELVI_XP     2
/*!
  @def ELVI_VISTA
  @brief ELVersionInfo define for Windows Vista
  */
#define ELVI_VISTA  3
/*!
  @def ELVI_WIN7
  @brief ELVersionInfo define for Windows 7
  */
#define ELVI_WIN7   4

#ifndef SND_SYSTEM
#define SND_SYSTEM      0x200000
#endif

#define SI_PATH       0x01
#define SI_WORKINGDIR 0x02
#define SI_ARGUMENTS  0x04
#define SI_SHOW       0x08
#define SI_ICONPATH   0x10
#define SI_ICONINDEX  0x20
#define SI_ALL        SI_PATH|SI_WORKINGDIR|SI_ARGUMENTS|SI_SHOW|SI_ICONPATH|SI_ICONINDEX

#define CTP_FULL      0
#define CTP_RELATIVE  1

/*!
  @struct _SHORTCUTINFO
  @brief structure that contains shortcut information
  */
typedef struct _SHORTCUTINFO
{
  DWORD flags;
  WCHAR Path[MAX_PATH]; /*!< Path to the executable defined by the shortcut */
  WCHAR WorkingDirectory[MAX_PATH]; /*!< Working directory of the shortcut */
  WCHAR Arguments[MAX_PATH]; /*!< Arguments of the shortcut */
  int ShowCmd; /*!< Initial visibility state of the shortcut */
  WCHAR IconPath[MAX_PATH]; /*!< Path to the icon for the shortcut */
  int IconIndex; /*!< Index of the icon to use for the shortcut */
}
SHORTCUTINFO, *LPSHORTCUTINFO;

/*!
  @struct _VERSIONINFO
  @brief structure that contains applet version information
  */
typedef struct _VERSIONINFO
{
  WCHAR Version[MAX_PATH];
  WCHAR Author[MAX_PATH];
  WCHAR Description[MAX_PATH];
}
VERSIONINFO, *LPVERSIONINFO;

/*!
  @struct _SNAPMOVEINFO
  @brief structure that contains information about a window to be 'snapped' when
  moving
  */
typedef struct _SNAPMOVEINFO
{
  HWND AppletWindow;
  POINT origin;
  RECT *AppletRect;
}
SNAPMOVEINFO, *LPSNAPMOVEINFO;

/*!
  @struct _SNAPSIZEINFO
  @brief structure that contains information about a window to be 'snapped' when
  being sized
  */
typedef struct _SNAPSIZEINFO
{
  HWND AppletWindow;
  UINT AppletEdge;
  RECT *AppletRect;
}
SNAPSIZEINFO, *LPSNAPSIZEINFO;

typedef struct _THEMEINFO
{
  WCHAR theme[MAX_PATH];
  WCHAR path[MAX_PATH];
  WCHAR themePath[MAX_PATH];
  WCHAR userPath[MAX_PATH];
}
THEMEINFO, *LPTHEMEINFO;

// Declaration of functions to import
DLL_EXPORT bool ELReadRegDWord(HKEY key, LPCTSTR value, DWORD *target, DWORD notFound);
DLL_EXPORT bool ELWriteRegDWord(HKEY key, LPTSTR value, DWORD *source);
DLL_EXPORT bool ELReadRegString(HKEY key, LPCTSTR value, LPTSTR target, LPCTSTR notFound);
DLL_EXPORT bool ELReadRegRect(HKEY key, LPCTSTR value, RECT *target, RECT *notFound);
DLL_EXPORT bool ELWriteRegString(HKEY key, LPTSTR value, LPTSTR source);
DLL_EXPORT bool ELWriteRegRect(HKEY key, LPTSTR value, RECT *source);
DLL_EXPORT bool ELOpenRegKey(LPCTSTR subkey, HKEY *key, bool createKey);
DLL_EXPORT bool ELDeleteRegKey(LPTSTR subkey);
DLL_EXPORT bool ELCloseRegKey(HKEY key);
DLL_EXPORT void ELThreadExecute(void *argument);
DLL_EXPORT bool ELExecute(LPTSTR application, LPTSTR workingDir = (WCHAR*)TEXT("\0"), int nShow = SW_SHOW);
DLL_EXPORT bool ELExecuteInternal(LPTSTR application);
DLL_EXPORT bool ELExecuteAlias(LPTSTR alias);
DLL_EXPORT bool ELExecuteSpecialFolder(LPTSTR folder);
DLL_EXPORT bool ELExecuteAll(WCHAR *command, WCHAR *workingDir);
DLL_EXPORT bool ELGetCurrentPath(LPTSTR path);
DLL_EXPORT std::wstring ELGetUserDataPath();
DLL_EXPORT HWND ELGetCoreWindow();
DLL_EXPORT bool ELRun();
DLL_EXPORT bool ELRegisterShellHook(HWND hwnd, DWORD method);
DLL_EXPORT bool ELQuit(bool prompt);
DLL_EXPORT HMONITOR ELGetDesktopRect(RECT *appletRect, RECT *rect);
DLL_EXPORT bool ELShutdown(HWND wnd);
DLL_EXPORT int ELMessageBox(HWND hwnd, const WCHAR* messageText, const WCHAR* messageTitle, DWORD msgFlags);
DLL_EXPORT bool ELSnapMove(LPSNAPMOVEINFO snapMoveInfo);
DLL_EXPORT bool ELSnapMoveToDesk(RECT *AppletRect);
DLL_EXPORT bool ELSnapSize(LPSNAPSIZEINFO snapSizeInfo);
DLL_EXPORT POINT ELGetAnchorPoint(HWND hwnd);
DLL_EXPORT bool ELStealFocus(HWND wnd);
DLL_EXPORT bool ELSwitchToThisWindow(HWND wnd);
DLL_EXPORT bool ELSetForeground(HWND wnd);
DLL_EXPORT bool ELParseShortcut(LPCTSTR shortcut, LPSHORTCUTINFO shortcutInfo);
DLL_EXPORT bool ELExit(UINT uFlag , bool prompt);
DLL_EXPORT bool ELShowDesktop();
DLL_EXPORT bool ELCheckWindow(HWND hwnd);
DLL_EXPORT HICON ELConvertIcon(HICON sourceIcon, HINSTANCE appInstance, COLORREF bgColour, BYTE bgAlpha);
DLL_EXPORT bool ELReadFileInt(const WCHAR *fileName, WCHAR *item, int *target, int notFound);
DLL_EXPORT bool ELReadFileByte(const WCHAR *fileName, WCHAR *item, BYTE *target, BYTE notFound);
DLL_EXPORT bool ELReadFileString(const WCHAR *fileName, WCHAR *item, WCHAR *target, WCHAR *notFound);
DLL_EXPORT bool ELReadFileBool(const WCHAR *fileName, WCHAR *item, bool *target, bool notFound);
DLL_EXPORT bool ELReadFileColor(const WCHAR *fileName, WCHAR *item, COLORREF *target, COLORREF notFound);
DLL_EXPORT bool ELWriteFileInt(const WCHAR *fileName, WCHAR *keyword, int value);
DLL_EXPORT bool ELWriteFileString(const WCHAR *fileName, WCHAR *keyword, WCHAR *value);
DLL_EXPORT bool ELWriteFileBool(const WCHAR *fileName, WCHAR *keyword, bool value);
DLL_EXPORT bool ELWriteFileColor(const WCHAR *fileName, WCHAR *keyword, COLORREF value);
DLL_EXPORT UINT ELVersionInfo();
DLL_EXPORT bool ELAppletVersionInfo(HWND appWnd, LPVERSIONINFO versionInfo);
DLL_EXPORT bool ELAppletFileVersion(WCHAR *applet, LPVERSIONINFO versionInfo);
DLL_EXPORT bool ELGetWindowApp(HWND hWnd, WCHAR *processName, bool fullName);
DLL_EXPORT bool ELIsFullScreen(HWND appletWnd, HWND appWnd);
DLL_EXPORT bool ELParseCommand(const WCHAR *application, WCHAR *program, WCHAR *arguments);
DLL_EXPORT int ELIsSpecialFolder(WCHAR *folder);
DLL_EXPORT bool ELSpecialFolderValue(WCHAR *folder, WCHAR *value);
DLL_EXPORT UINT ELIsInternalCommand(WCHAR *command);
DLL_EXPORT bool ELGetSpecialFolder(int folder, WCHAR *folderPath);
DLL_EXPORT void ELWriteDebug(std::wstring debugText);
DLL_EXPORT void ELDisplayChange(HWND hwnd);
DLL_EXPORT int ELMid(int left, int right);
DLL_EXPORT bool ELGetTempFileName(WCHAR *fileName);
DLL_EXPORT bool ELForceForeground(HWND hwnd);
DLL_EXPORT HWND ELGetDesktopWindow();
DLL_EXPORT bool ELAdjustVolume(UINT command);
//DLL_EXPORT bool ELControlBrowser(UINT command);
DLL_EXPORT UINT ELStringReplace(WCHAR *original, WCHAR *pattern, WCHAR *replacement, bool ignoreCase);
DLL_EXPORT BOOL ELIsWow64();
DLL_EXPORT std::wstring ELExpandVars(std::wstring value);
DLL_EXPORT bool ELUnExpandVars(LPTSTR value);
DLL_EXPORT bool ELIsModal(HWND window);
DLL_EXPORT std::tr1::shared_ptr<TiXmlDocument> ELOpenXMLConfig(std::wstring filename, bool create);
DLL_EXPORT bool ELWriteXMLConfig(TiXmlDocument *configXML);
DLL_EXPORT bool ELReadXMLColorValue(TiXmlElement *section, const WCHAR *item, COLORREF *value, COLORREF defaultValue);
DLL_EXPORT bool ELWriteXMLColorValue(TiXmlElement *section, const WCHAR *item, COLORREF value);
DLL_EXPORT bool ELReadXMLRectValue(TiXmlElement *section, const WCHAR *item, RECT *value, RECT defaultValue);
DLL_EXPORT bool ELWriteXMLRectValue(TiXmlElement *section, const WCHAR *item, RECT value);
DLL_EXPORT bool ELReadXMLBoolValue(TiXmlElement *section, const WCHAR *item, bool *value, bool defaultValue);
DLL_EXPORT bool ELWriteXMLBoolValue(TiXmlElement *section, const WCHAR *item, bool value);
DLL_EXPORT bool ELReadXMLIntValue(TiXmlElement *section, const WCHAR *item, int *value, int defaultValue);
DLL_EXPORT bool ELWriteXMLIntValue(TiXmlElement *section, const WCHAR *item, int value);
DLL_EXPORT bool ELReadXMLFloatValue(TiXmlElement *section, const WCHAR *item, float *value, float defaultValue);
DLL_EXPORT bool ELWriteXMLFloatValue(TiXmlElement *section, const WCHAR *item, float value);
DLL_EXPORT bool ELReadXMLStringValue(TiXmlElement *section, const WCHAR *item, WCHAR *value, const WCHAR *defaultValue);
DLL_EXPORT bool ELWriteXMLStringValue(TiXmlElement *section, const WCHAR *item, const WCHAR *value);
DLL_EXPORT TiXmlElement *ELGetXMLSection(TiXmlDocument *configXML, WCHAR *section, bool createSection);
DLL_EXPORT TiXmlElement *ELGetFirstXMLElement(TiXmlElement *xmlSection);
DLL_EXPORT TiXmlElement *ELGetFirstXMLElementByName(TiXmlElement *xmlSection, WCHAR *elementName);
DLL_EXPORT TiXmlElement *ELSetFirstXMLElement(TiXmlElement *xmlSection, const WCHAR *elementName);
DLL_EXPORT TiXmlElement *ELGetSiblingXMLElement(TiXmlElement *xmlElement);
DLL_EXPORT TiXmlElement *ELSetSibilingXMLElement(TiXmlElement *xmlElement, const WCHAR *elementName);
DLL_EXPORT bool ELGetXMLElementText(TiXmlElement *xmlElement, WCHAR* xmlString);
DLL_EXPORT bool ELGetXMLElementLabel(TiXmlElement *xmlElement, WCHAR* xmlString);
DLL_EXPORT bool ELRemoveXMLElement(TiXmlElement *xmlElement);
DLL_EXPORT bool ELPlaySound(const WCHAR *sound);
DLL_EXPORT bool ELGetAppPath(const WCHAR *program, WCHAR *path);
DLL_EXPORT bool ELIsExecutable(WCHAR *extension);
DLL_EXPORT bool ELFileTypeCommand(WCHAR *document, WCHAR *docArgs, WCHAR *commandLine);
DLL_EXPORT bool ELCreateDirectory(std::wstring directory);
DLL_EXPORT void ELGetThemeInfo(LPTHEMEINFO themeInfo);
DLL_EXPORT TiXmlDocument *ELGetXMLConfig(TiXmlElement *element);
DLL_EXPORT bool ELConvertThemePath(WCHAR *schemeFile, DWORD flags);
DLL_EXPORT bool ELConvertUserPath(WCHAR *schemeFile, DWORD flags);
DLL_EXPORT bool ELConvertAppletPath(WCHAR *schemeFile, DWORD flags);
DLL_EXPORT int ELGetAppletMonitor(HWND hwnd);
DLL_EXPORT RECT ELGetMonitorRect(int monitor);
//DLL_EXPORT bool ELIsKeyDown(UINT virtualKey);
inline bool ELIsKeyDown(UINT virtualKey)
{
  return ((GetAsyncKeyState(virtualKey) & 0x8000) == 0x8000);
}
inline bool ELIsModifiedTheme(std::wstring theme)
{
  return (theme.find(TEXT("(Modified)")) != std::wstring::npos);
}
DLL_EXPORT bool ELPathFileExists(const WCHAR *file);
DLL_EXPORT bool ELSetModifiedTheme(std::wstring theme);
DLL_EXPORT void ELStripModified(WCHAR *theme);
DLL_EXPORT std::wstring ELwcsftime(const WCHAR *format, const struct tm *timeptr);
DLL_EXPORT BOOL ELPathIsRelative(LPCTSTR lpszPath);
template <class T>
inline std::wstring towstring(const T& t)
{
  std::wostringstream ostr;
  ostr << t;
  return ostr.str();
}
template <class T>
inline std::wstring towstring(const T& t, int precision)
{
  std::wostringstream ostr;
  ostr << std::fixed << std::setprecision(precision) << t;
  return ostr.str();
}
template <class T>
inline T fromwstring(const std::wstring& s)
{
  T t;
  std::wistringstream istr(s);
  istr >> t;
  return t;
}
DLL_EXPORT bool ELSetEmergeVars();
DLL_EXPORT void ELClearEmergeVars();
DLL_EXPORT std::string ELwstringTostring(std::wstring wideString, UINT codePage = CP_UTF8);
DLL_EXPORT std::wstring ELstringTowstring(std::string narrowString, UINT codePage = CP_UTF8);
DLL_EXPORT std::wstring ELGetThemeName();
DLL_EXPORT bool ELSetTheme(std::wstring theme);
DLL_EXPORT std::wstring ELToLower(std::wstring workingString);
DLL_EXPORT bool ELSetAppletsTheme(std::wstring theme);
DLL_EXPORT bool ELFileOp(HWND appletWnd, UINT function, std::wstring source, std::wstring destination = TEXT(""));
DLL_EXPORT void ELILFree(LPITEMIDLIST pidl);
DLL_EXPORT LPITEMIDLIST ELILClone(LPITEMIDLIST pidl);
DLL_EXPORT LPITEMIDLIST ELILFindLastID(LPITEMIDLIST pidl);
DLL_EXPORT BOOL ELILRemoveLastID(LPITEMIDLIST pidl);

#endif