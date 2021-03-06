//----  --------------------------------------------------------------------------------------------------------
//
//  This file is part of Emerge Desktop.
//  Copyright (C) 2004-2013  The Emerge Desktop Development Team
//
//  Emerge Desktop is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  Emerge Desktop is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//----  --------------------------------------------------------------------------------------------------------

#include "Applet.h"

WCHAR myName[ ] = TEXT("emergeCommand");
WCHAR szClassName[ ] = TEXT("Shell_CommandWnd");

//----  --------------------------------------------------------------------------------------------------------
// Function:	TimerProc
// Required:	HWND hwnd - Unused
//		UINT uMsg - Unused
//		UINT idEvent - The event type
//		DWORD dwTime - Unused
// Returns:	None
// Purpose:	Tells us our timer tick has occurred - so update text with new time
//----  --------------------------------------------------------------------------------------------------------
LRESULT Applet::DoTimer(UINT idEvent)
{
  std::wstring tmp;

  if (idEvent == MOUSE_TIMER)
  {
    return BaseApplet::DoTimer(idEvent);
  }

  // Only draw the time if this is a timer event and the edit box isn't focused
  if (idEvent == ID_CLOCKTIMER)
  {
    time_t tVal;
    struct tm* stVal;

    _tzset(); /**< Grab the current timezone information for localtime to do it's job correctly */
    time(&tVal); /**< Grab the raw time */
    stVal = localtime(&tVal); /**< Format the rawtime to localtime */

    // Format the time
    tmp = ELwcsftime(pSettings->GetDisplayTimeFormat(), stVal);
    SetCommandText(tmp);

    tmp = ELwcsftime(pSettings->GetDisplayTipFormat(), stVal);
    UpdateTip(tmp);

    if (IsWindowVisible(mainWnd))
    {
      DrawAlphaBlend();
    }

    return 0;
  }

  return 1;
}

//----  --------------------------------------------------------------------------------------------------------
// Function:	WindowProcedure
// Required:	HWND hwnd - window handle that message was sent to
// 		UINT message - action to handle
// 		WPARAM wParam - dependant on message
// 		LPARAM lParam - dependant on message
// Returns:	LRESULT
// Purpose:	Handles messages sent from DispatchMessage
//----  --------------------------------------------------------------------------------------------------------
LRESULT CALLBACK Applet::WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  CREATESTRUCT* cs;
  static Applet* pApplet = NULL;

  if (message == WM_CREATE)
  {
    // Register to recieve the specified Emerge Desktop messages
    PostMessage(ELGetCoreWindow(), EMERGE_REGISTER, (WPARAM)hwnd, (LPARAM)EMERGE_CORE);

    cs = (CREATESTRUCT*)lParam;
    pApplet = reinterpret_cast<Applet*>(cs->lpCreateParams);
    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  if (pApplet == NULL)
  {
    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  switch (message)
  {
  case WM_COPYDATA:
    return pApplet->DoCopyData((COPYDATASTRUCT*)lParam);

    // Needed to handle changing the system colors.  It forces
    // a repaint of the window as well as the frame.
  case WM_SYSCOLORCHANGE:
    return pApplet->DoSysColorChange();

    // Allow for window dragging via Ctrl - Left - Click dragging
  case WM_NCLBUTTONDOWN:
    pApplet->DoNCLButtonDown();
    return DefWindowProc(hwnd, message, wParam, lParam);

    // Display the mainMenu via Ctrl - Right - Click
  case WM_NCRBUTTONUP:
    return pApplet->DoNCRButtonUp();

    // Forward the appropriate clicks to the icons
  case WM_LBUTTONDOWN:
    return pApplet->ShowCommand();

    // Reset the cursor back to the standard arrow after dragging
  case WM_NCLBUTTONUP:
    pApplet->DoNCLButtonUp();
    return DefWindowProc(hwnd, message, wParam, lParam);

  case WM_SETCURSOR:
    pApplet->DoSetCursor();
    return DefWindowProc(hwnd, message, wParam, lParam);

  case WM_DISPLAYCHANGE:
    return pApplet->DoDisplayChange(hwnd);

    // Handles the resizing of the window
  case WM_NCHITTEST:
    return pApplet->DoHitTest(lParam);

    // Repaint the icons as the window size is changing
  case WM_WINDOWPOSCHANGING:
    return pApplet->DoWindowPosChanging((WINDOWPOS*)lParam);

  case WM_MOVING:
    return pApplet->DoMoving(hwnd, (RECT*)lParam);

  case WM_ENTERSIZEMOVE:
    return pApplet->DoEnterSizeMove(hwnd);

  case WM_EXITSIZEMOVE:
    return pApplet->DoExitSizeMove(hwnd);

  case WM_SIZING:
    return pApplet->DoSizing(hwnd, (UINT)wParam, (LPRECT)lParam);

  case WM_SYSCOMMAND:
    return pApplet->DoSysCommand(hwnd, message, wParam, lParam);

  case WM_SHOWWINDOW:
    if (wParam)
    {
      pApplet->DrawAlphaBlend();
    }
    break;

  case WM_TIMER:
    return pApplet->DoTimer((UINT)wParam);

  case WM_DESTROY:
  case WM_NCDESTROY:
    // Unregister the specified Emerge Desktop messages
    PostMessage(ELGetCoreWindow(), EMERGE_UNREGISTER, (WPARAM)hwnd, (LPARAM)EMERGE_CORE);

    KillTimer(hwnd, ID_CLOCKTIMER);
    PostQuitMessage(0);
    break;

    // If not handled just forward the message on
  default:
    return pApplet->DoDefault(hwnd, message, wParam, lParam);
  }

  return 0;
}

Applet::Applet(HINSTANCE hInstance)
  : BaseApplet(hInstance, myName, false, true)
{
  setlocale(LC_TIME, "");
  mainInst = hInstance;
  commandText = TEXT("");
  mainFont = NULL;
}

Applet::~Applet()
{
  if (mainFont)
  {
    DeleteObject(mainFont);
  }
}

UINT Applet::Initialize()
{
  pSettings = std::tr1::shared_ptr<Settings>(new Settings());
  UINT ret = BaseApplet::Initialize(WindowProcedure, this, pSettings);
  if (ret == 0)
  {
    return ret;
  }

  // Create the Command Window
  pCommand = std::tr1::shared_ptr<Command>(new Command(mainWnd, mainInst, pSettings));
  if (!pCommand->Init())
  {
    UnregisterClass(szClassName, mainInst);
    return 0;
  }

  // Set the window transparency
  UpdateGUI();

  SetTimer(mainWnd, ID_CLOCKTIMER, 1000, NULL);

  return 1;
}

void Applet::Activate()
{
  ShowCommand();
}

LRESULT Applet::PaintContent(HDC hdc, RECT clientrt)
{
  CLIENTINFO clientInfo;
  FORMATINFO formatInfo;

  if (ELToLower(pSettings->GetClockTextAlign()) == TEXT("center"))
  {
    formatInfo.horizontalAlignment = EGDAT_HCENTER;
  }
  else if (ELToLower(pSettings->GetClockTextAlign()) == TEXT("right"))
  {
    formatInfo.horizontalAlignment = EGDAT_RIGHT;
  }
  else
  {
    formatInfo.horizontalAlignment = EGDAT_LEFT;
  }
  if (ELToLower(pSettings->GetClockVerticalAlign()) == TEXT("center"))
  {
    formatInfo.verticalAlignment = EGDAT_VCENTER;
  }
  else if (ELToLower(pSettings->GetClockVerticalAlign()) == TEXT("bottom"))
  {
    formatInfo.verticalAlignment = EGDAT_BOTTOM;
  }
  else
  {
    formatInfo.verticalAlignment = EGDAT_TOP;
  }
  formatInfo.font = mainFont;
  formatInfo.color = guiInfo.colorFont;
  formatInfo.flags = 0;

  clientInfo.hdc = hdc;
  CopyRect(&clientInfo.rt, &clientrt);
  clientInfo.bgAlpha = guiInfo.alphaBackground;

  EGDrawAlphaText(guiInfo.alphaText, clientInfo, formatInfo, commandText);

  return 0;
}

void Applet::ShowConfig()
{
  Config config(mainInst, mainWnd, appletName, pSettings);
  if (config.Show() == IDOK)
  {
    UpdateGUI();
  }
}

void Applet::UpdateTip(std::wstring tip)
{
  TOOLINFO ti;
  ZeroMemory(&ti, sizeof(TOOLINFO));
  RECT rect;
  bool exists;
  WCHAR tipBuffer[MAX_LINE_LENGTH];

  wcscpy(tipBuffer, tip.c_str());

  GetClientRect(mainWnd, &rect);

  // fill in the TOOLINFO structure
  ti.cbSize = TTTOOLINFOW_V2_SIZE;
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = mainWnd;
  ti.uId = (ULONG_PTR)toolWnd;

  // Check to see if the tooltip exists
  exists = SendMessage(toolWnd, TTM_GETTOOLINFO, 0, (LPARAM) (LPTOOLINFO) &ti) ? true : false;

  //  complete the rest of the TOOLINFO structure
  ti.hinst = mainInst;
  ti.lpszText = tipBuffer;
  ti.rect = rect;

  // If it exists, modify the tooltip, if not add it
  if (exists)
  {
    SendMessage(toolWnd, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
  }
  else
  {
    SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
  }
}

void Applet::AppletUpdate()
{

  if (mainFont != NULL)
  {
    DeleteObject(mainFont);
  }
  mainFont = CreateFontIndirect(pSettings->GetFont());
}

LRESULT Applet::ShowCommand()
{
  if (pCommand)
  {
    ShowWindow(mainWnd, SW_HIDE);
    pCommand->UpdateGUI(guiInfo);
    pCommand->SetHidden(appletHidden);
    ShowWindow(pCommand->GetCommandWnd(), SW_SHOW);
    return 0;
  }

  return 1;
}

void Applet::Show()
{
  if (!appletHidden)
  {
    ShowWindow(mainWnd, SW_SHOWNOACTIVATE);
  }
}

void Applet::SetCommandText(std::wstring commandText)
{
  this->commandText = commandText;
}

GUIINFO Applet::GetGUIInfo()
{
  return guiInfo;
}

