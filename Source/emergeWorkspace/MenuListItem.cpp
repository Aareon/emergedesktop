//---
//
//  This file is part of Emerge Desktop.
//  Copyright (C) 2004-2012  The Emerge Desktop Development Team
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

#include "MenuListItem.h"

MenuListItem::MenuListItem(WCHAR *name, UINT type, WCHAR *value, TiXmlElement *section, HMENU menu)
{
  this->type = type;
  this->section = section;
  this->menu = menu;

  if (name)
    wcscpy(this->name, name);
  else
    wcscpy(this->name, (WCHAR*)TEXT("\0"));

  if (value)
    wcscpy(this->value, value);
  else
    wcscpy(this->value, (WCHAR*)TEXT("\0"));
}

MenuListItem::~MenuListItem()
{
}

WCHAR *MenuListItem::GetValue()
{
  return value;
}

TiXmlElement *MenuListItem::GetSection()
{
  return section;
}

WCHAR *MenuListItem::GetName()
{
  return name;
}

void MenuListItem::SetValue(WCHAR *value)
{
  wcscpy((*this).value, value);
}

void MenuListItem::SetName(WCHAR *name)
{
  wcscpy((*this).name, name);
}

void MenuListItem::SetSection(TiXmlElement *section)
{
  this->section = section;
}

UINT MenuListItem::GetType()
{
  return type;
}

MenuItem *MenuListItem::FindMenuItem(UINT id)
{
  MenuItem *menuItem = NULL;
  std::vector< std::tr1::shared_ptr<MenuItem> >::iterator iter = menuItems.begin();

  while (iter != menuItems.end())
  {
    if ((*iter)->GetID() == id)
      break;

    iter++;
  }

  if (iter != menuItems.end())
    menuItem = iter->get();

  return menuItem;
}

MenuItem *MenuListItem::GetMenuItem(UINT index)
{
  return menuItems[index].get();
}

UINT MenuListItem::GetMenuItemCount()
{
  return (UINT)menuItems.size();
}

void MenuListItem::AddMenuItem(MenuItem *menuItem)
{
  menuItems.push_back( std::tr1::shared_ptr<MenuItem>(menuItem) );
}

void MenuListItem::DeleteMenuItem(UINT index)
{
  menuItems.erase(menuItems.begin() + index);
}
