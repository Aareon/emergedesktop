//---
//
//  This file is part of Emerge Desktop.
//  Copyright (C) 2004-2007  The Emerge Desktop Development Team
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
//---

#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "../emergeLib/emergeLib.h"
#include "../emergeBaseClasses/BaseSettings.h"
#include "Item.h"
#include <stdio.h>

class Settings: public BaseSettings
{
public:
  Settings();
  ~Settings();
  UINT GetItemListSize();
  Item *GetItem(UINT index);
  void PopulateItems();
  void DeleteItems(bool clearXML);
  void WriteItem(WCHAR *command, WCHAR *iconPath, WCHAR *tip, WCHAR *workingDir);

protected:
  virtual void ResetDefaults();

private:
  std::vector< std::tr1::shared_ptr<Item> > itemList;
  std::wstring xmlFile;
  WCHAR keyString[MAX_LINE_LENGTH];
};

#endif
