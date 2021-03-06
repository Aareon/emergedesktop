/*!
  @file MenuBuilder.h
  @brief header for emergeWorkspace
  @author The Emerge Desktop Development Team

  @attention This file is part of Emerge Desktop.
  @attention Copyright (C) 2004-2013  The Emerge Desktop Development Team

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

#ifndef __GUARD_620C9D0B_74A0_48E8_B6E2_4386C0DA40A0
#define __GUARD_620C9D0B_74A0_48E8_B6E2_4386C0DA40A0

#define UNICODE 1

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1 //suppress warnings about old versions of wcscpy, wcscat, etc.
#define _CRT_NON_CONFORMING_SWPRINTFS 1 //suppress warnings about old swprintf format
#endif

// Define required for the Window Transparency
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501

// Define required for menu animation
#undef WINVER
#define WINVER 0x0501

#include <process.h>
#include <time.h>
#include <map>
#include "../emergeAppletEngine/emergeAppletEngine.h"
#include "../emergeGraphics/emergeGraphics.h"
#include "../emergeLib/emergeFileRegistryLib.h"
#include "Config.h"
#include "ItemEditor.h"
#include "MenuItem.h"
#include "MenuListItem.h"

// Define Menu Types
#define MENU_RIGHT           0x01
#define MENU_MID             0x02

// Define DisplayRegMenu Types
#define DRM_DELETE          0x100
#define DRM_EDIT            0x200
#define DRM_ADD             0x300
#define DRM_RUNAS           0x400

#ifndef MND_CONTINUE
#define MND_CONTINUE 0
#endif

#ifndef MND_ENDMENU
#define MND_ENDMENU 1
#endif

#ifndef MN_SELECTITEM
#define MN_SELECTITEM 0x01E5
#endif

#ifndef MN_BUTTONDOWN
#define MN_BUTTONDOWN 0x01ED
#endif

#ifndef MN_BUTTONUP
#define MN_BUTTONUP 0x01EF
#endif

#ifndef MN_GETMENU
#define MN_GETMENU 0x01E1
#endif

#define MENU_DOWN   0xFFFFFFFD
#define MENU_UP     0xFFFFFFFC

// Define BuildSettingsMenu options
typedef enum _BUILDSETTINGSMENU {
  BSM_SEPARATOR = 0,
  BSM_CONFIGURE,
  BSM_CORE,
  BSM_SHELL,
  BSM_SELECTTHEME,
  BSM_SAVETHEME,
  BSM_QUIT
} BUILDSETTINGSMENU;

// Define BuildHelpMenu options
typedef enum _BUILDHELPMENU {
  BHM_SEPARATOR = 0,
  BHM_OFFLINE,
  BHM_TUTORIAL,
  BHM_WEBSITE,
  BHM_ABOUT,
  BHM_WELCOME
} BUILDHELPMENU;

typedef std::map< HMENU,std::tr1::shared_ptr<MenuListItem> > MenuMap;
typedef std::pair< HMENU,std::tr1::shared_ptr<MenuListItem> > MenuPair;
typedef std::map< UINT_PTR,std::tr1::shared_ptr<MenuItem> > MenuItemMap;
typedef std::pair< UINT_PTR,std::tr1::shared_ptr<MenuItem> > MenuItemPair;

class MenuBuilder
{
private:
  std::tr1::shared_ptr<ItemEditor> pItemEditor;
  std::tr1::shared_ptr<Settings> pSettings;
  HWND menuWnd;
  HINSTANCE mainInst;
  HMENU rootMenu, taskMenu;
  bool MButtonDown;
  UINT ShellMessage;
  void BuildMenu(MenuMap::iterator iter);
  void BuildXMLMenu(MenuMap::iterator iter);
  void BuildFileMenu(MenuMap::iterator iter);
  void BuildFileMenuFromString(MenuMap::iterator iter, WCHAR *parsedValue);
  void BuildDefaultMenu(MenuMap::iterator iter);
  void BuildSettingsMenu(MenuMap::iterator iter);
  void BuildHelpMenu(MenuMap::iterator iter);
  std::wstring NoPrefixString(std::wstring source);
  MenuMap menuMap;
  HHOOK menuHook;
  RECT explorerWorkArea;
  void ExecuteXMLMenuItem(UINT type, WCHAR *value, WCHAR *workingDir);
  void ExecuteFileMenuItem(const WCHAR *value);
  void ExecuteTaskMenuItem(HWND task);
  void ExecuteSettingsMenuItem(UINT index);
  void ExecuteHelpMenuItem(UINT index);
  void ExpandEmergeVar(LPTSTR value, LPTSTR var);
  void ClearMenu(MenuMap::iterator iter);
  void AddSpecialItem(MenuMap::iterator iter, UINT type, WCHAR* text, UINT id);
  bool GetPos(MenuMap::iterator iter, WCHAR *input, bool directory, UINT *pos, UINT *itemID, ULONG_PTR *itemData);
  static LRESULT CALLBACK HookCallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
  static BOOL CALLBACK BuildTasksMenu(HWND hwnd, LPARAM lParam);
  static LRESULT CALLBACK MenuProcedure (HWND, UINT, WPARAM, LPARAM);
  static BOOL CALLBACK SetMonitorArea(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
  int DisplayRegContext(POINT pt, int type);
  bool registered;
  bool AddMenuItem(std::tr1::shared_ptr<MenuItem> menuItem);
  bool EditMenuItem(std::tr1::shared_ptr<MenuItem> menuItem);
  void ElevatedExecute(std::tr1::shared_ptr<MenuItem> menuItem);
  HMENU activeMenu;
  HANDLE DragItemDataToHandle(DRAGITEMDATA *menuItemData);
  HDROP FileToHandle(WCHAR *file);
  MenuItemMap menuItemMap;
  bool topGap, bottomGap;

public:
  MenuBuilder(HINSTANCE desktopInst);
  ~MenuBuilder();
  bool Initialize();
  LRESULT DoButtonDown(UINT button);
  LRESULT DoMenuDrag(HWND hwnd, UINT pos, HMENU menu);
  LRESULT DoMenuGetObject(HWND hwnd, MENUGETOBJECTINFO *mgoInfo);
  LRESULT DoInitMenu(HMENU menu);
  LRESULT ExecuteMenuItem(UINT_PTR itemID);
  LRESULT DoContextMenu();
  LRESULT DoDefault(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
  LRESULT DoCopyData(COPYDATASTRUCT *cds);
  void AddTaskItem(HWND hwnd);
  BOOL DoDrawItem(LPDRAWITEMSTRUCT lpDrawItem);
  BOOL DoMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItem);
  LRESULT DoMenuSelect(HMENU menu, WORD itemType, WORD item);
  void ClearAllMenus();
  HWND GetWnd();
  void SetWorkArea();
  void RenameConfigFile();
  BYTE GetMenuAlpha();
  void SetActiveMenu(HMENU menu);
  bool DropMenuItem(MENUITEMDATA *menuItemData, MENUITEMDATA *dropItemData, HMENU menu, POINT pt);
};

#endif
