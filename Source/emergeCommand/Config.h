/*!
  @file Config.h
  @brief header for emergeCommand
  @author The Emerge Desktop Development Team

  @attention This file is part of Emerge Desktop.
  @attention Copyright (C) 2004-2012  The Emerge Desktop Development Team

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

#ifndef __GUARD_452a0986_4855_406e_91fc_c96caec93825
#define __GUARD_452a0986_4855_406e_91fc_c96caec93825

#define UNICODE 1

#undef _WIN32_IE
#define _WIN32_IE 0x0600

#ifdef __GNUC__
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#else
#include <memory>
#endif

#include "../emergeStyleEngine/StyleEditor.h"
#include "ConfigPage.h"
#include "resource.h"
#include "Settings.h"
#include "TextPage.h"

class Config
{
public:
  Config(HINSTANCE hInstance, HWND mainWnd, WCHAR* instanceName, std::tr1::shared_ptr<Settings> pSettings);
  ~Config();
  int Show();
  INT_PTR DoInitDialog(HWND hwndDlg);

private:
  std::tr1::shared_ptr<StyleEditor> pStyleEditor;
  std::tr1::shared_ptr<ConfigPage> pConfigPage;
  std::tr1::shared_ptr<TextPage> pTextPage;
  std::tr1::shared_ptr<Settings> pSettings;
  HINSTANCE hInstance;
  HWND mainWnd;
  static INT_PTR CALLBACK ConfigDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif

