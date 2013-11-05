/*!
  @file main.cpp
  @brief Utility functions for Emerge Desktop
  @author The Emerge Desktop Development Team

  @attention This file is part of Emerge Desktop.
  @attention Copyright (C) 2004-2012  The Emerge Desktop Development Team

  @attention Emerge Desktop is free software; you can redistribute it and/or
  modify  it under the terms of the GNU General Public License as published
  by  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  @attention Emerge Desktop is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  @attention You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL UNUSED, DWORD fdwReason, LPVOID lpvReserved UNUSED)
{
  switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
      if (mprDLL == NULL)
        mprDLL = ELLoadSystemLibrary(TEXT("mpr.dll"));
      break;
    case DLL_PROCESS_DETACH:
      if (mprDLL != NULL)
        {
          FreeLibrary(mprDLL);
          mprDLL = NULL;
        }
      break;
    }

  return TRUE;
}
