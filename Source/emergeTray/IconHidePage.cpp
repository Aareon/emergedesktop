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

#include "IconHidePage.h"

INT_PTR CALLBACK IconHidePage::IconHidePageDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  static IconHidePage *pIconHidePage = NULL;
  PROPSHEETPAGE *psp;

  switch (message)
    {
    case WM_INITDIALOG:
      psp = (PROPSHEETPAGE*)lParam;
      pIconHidePage = reinterpret_cast<IconHidePage*>(psp->lParam);
      if (!pIconHidePage)
        break;
      return pIconHidePage->DoInitPage(hwndDlg);

    case WM_COMMAND:
      return pIconHidePage->DoCommand(hwndDlg, wParam, lParam);

    case WM_NOTIFY:
      return pIconHidePage->DoNotify(hwndDlg, lParam);
    }

  return FALSE;
}

IconHidePage::IconHidePage(HINSTANCE hInstance, std::tr1::shared_ptr<Settings> pSettings)
{
  this->pSettings = pSettings;
  this->hInstance = hInstance;
  edit = false;

  InitCommonControls();

  toolWnd = CreateWindowEx(
              0,
              TOOLTIPS_CLASS,
              NULL,
              TTS_ALWAYSTIP|WS_POPUP|TTS_NOPREFIX,
              CW_USEDEFAULT, CW_USEDEFAULT,
              CW_USEDEFAULT, CW_USEDEFAULT,
              NULL,
              NULL,
              hInstance,
              NULL);

  if (toolWnd)
    {
      SendMessage(toolWnd, TTM_SETMAXTIPWIDTH, 0, 300);
      SetWindowPos(toolWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE |
                   SWP_NOACTIVATE);
    }

  addIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ADD), IMAGE_ICON, 16, 16, 0);
  editIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_EDIT), IMAGE_ICON, 16, 16, 0);
  delIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_DEL), IMAGE_ICON, 16, 16, 0);
  saveIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_SAVE), IMAGE_ICON, 16, 16, 0);
  abortIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ABORT), IMAGE_ICON, 16, 16, 0);
}

IconHidePage::~IconHidePage()
{
  if (addIcon)
    DestroyIcon(addIcon);
  if (editIcon)
    DestroyIcon(editIcon);
  if (delIcon)
    DestroyIcon(delIcon);
  if (saveIcon)
    DestroyIcon(saveIcon);
  if (abortIcon)
    DestroyIcon(abortIcon);

  DestroyWindow(toolWnd);
}

BOOL IconHidePage::DoInitPage(HWND hwndDlg)
{
  RECT rect;
  int iRet;
  HWND listWnd = GetDlgItem(hwndDlg, IDC_HIDELIST);
  HWND addWnd = GetDlgItem(hwndDlg, IDC_ADDTIP);
  HWND editWnd = GetDlgItem(hwndDlg, IDC_EDITTIP);
  HWND delWnd = GetDlgItem(hwndDlg, IDC_DELTIP);
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVETIP);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTTIP);
  HWND appWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);
  LVCOLUMN lvCol;
  TOOLINFO ti;
  DWORD dwRet;

  ZeroMemory(&ti, sizeof(TOOLINFO));
  GetWindowRect(hwndDlg, &rect);

  saveCount = 0;
  deleteCount = 0;

  if (pSettings->GetUnhideIcons())
    SendDlgItemMessage(hwndDlg, IDC_UNHIDE, BM_SETCHECK, BST_CHECKED, 0);

  if (addIcon)
    SendMessage(addWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)addIcon);
  if (editIcon)
    SendMessage(editWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)editIcon);
  if (delIcon)
    SendMessage(delWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)delIcon);
  if (saveIcon)
    SendMessage(saveWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)saveIcon);
  if (abortIcon)
    SendMessage(abortWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)abortIcon);

  EnableWindow(editWnd, false);
  EnableWindow(delWnd, false);
  EnableWindow(appWnd, false);
  EnableWindow(saveWnd, false);
  EnableWindow(abortWnd, false);

  lvCol.mask = LVCF_TEXT | LVCF_WIDTH;
  lvCol.pszText = (WCHAR*)TEXT("Icon Text");
  lvCol.cx = 300;

  iRet = ListView_InsertColumn(listWnd, 0, &lvCol);
  dwRet = ListView_SetExtendedListViewStyle(listWnd,  LVS_EX_FULLROWSELECT);

  ti.cbSize = TTTOOLINFOW_V2_SIZE;
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = addWnd;
  ti.uId = (ULONG_PTR)addWnd;
  ti.hinst = hInstance;
  ti.lpszText = (WCHAR*)TEXT("Add hidden icon");
  GetClientRect(addWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = editWnd;
  ti.uId = (ULONG_PTR)editWnd;
  ti.lpszText = (WCHAR*)TEXT("Edit hidden icon");
  GetClientRect(editWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = delWnd;
  ti.uId = (ULONG_PTR)delWnd;
  ti.lpszText = (WCHAR*)TEXT("Remove hidden icon");
  GetClientRect(delWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = saveWnd;
  ti.uId = (ULONG_PTR)saveWnd;
  ti.lpszText = (WCHAR*)TEXT("Save Item");
  GetClientRect(saveWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = abortWnd;
  ti.uId = (ULONG_PTR)abortWnd;
  ti.lpszText = (WCHAR*)TEXT("Discard Item");
  GetClientRect(abortWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  PopulateList(listWnd);

  return TRUE;
}

bool IconHidePage::CheckSaveCount(HWND hwndDlg)
{
  if ((saveCount != 0) || (deleteCount != 0))
    {
      if (ELMessageBox(hwndDlg,
                       (WCHAR*)TEXT("All currently modifications will be lost.  To save and exit press OK.\n\nDo you wish to continue?"),
                       (WCHAR*)TEXT("emergeLauncher"),
                       ELMB_YESNO|ELMB_ICONQUESTION|ELMB_MODAL) == IDYES)
        return true;
      else
        return false;
    }

  return true;
}

bool IconHidePage::PopulateList(HWND listWnd)
{
  bool ret = false;
  LVITEM lvItem;
  int iRet;

  lvItem.mask = LVIF_TEXT;

  for (UINT i = 0; i < pSettings->GetHideListSize(); i++)
    {
      ret = true;

      lvItem.iItem = i;
      lvItem.iSubItem = 0;
      lvItem.pszText = pSettings->GetHideListItem(i);
      lvItem.cchTextMax = (int)wcslen(pSettings->GetHideListItem(i));

      iRet = ListView_InsertItem(listWnd, &lvItem);
    }

  return ret;
}

BOOL IconHidePage::DoNotify(HWND hwndDlg, LPARAM lParam)
{
  HWND delWnd = GetDlgItem(hwndDlg, IDC_DELTIP);
  HWND editWnd = GetDlgItem(hwndDlg, IDC_EDITTIP);
  HWND listWnd = GetDlgItem(hwndDlg, IDC_HIDELIST);
  WCHAR tipText[MAX_LINE_LENGTH];
  int itemIndex;

  switch (((LPNMITEMACTIVATE)lParam)->hdr.code)
    {
    case LVN_ITEMCHANGED:
      EnableWindow(editWnd, true);
      EnableWindow(delWnd, true);
      itemIndex = ((LPNMLISTVIEW)lParam)->iItem;
      if (ListView_GetItemState(listWnd, itemIndex, LVIS_SELECTED))
        {
          ListView_GetItemText(listWnd, itemIndex, 0, tipText, MAX_LINE_LENGTH);
          SetDlgItemText(hwndDlg, IDC_ICONTEXT, tipText);
        }
      return TRUE;

    case PSN_APPLY:
      if (CheckFields(hwndDlg) && UpdateSettings(hwndDlg))
        SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, PSNRET_NOERROR);
      else
        SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, PSNRET_INVALID);
      return TRUE;

    case PSN_SETACTIVE:
      SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, 0);
      return TRUE;

    case PSN_KILLACTIVE:
      SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, FALSE);
      return TRUE;

    case PSN_RESET:
      pSettings->BuildHideList();
      return TRUE;

    case PSN_QUERYCANCEL:
      if (CheckFields(hwndDlg) && CheckSaveCount(hwndDlg))
        SetWindowLong(hwndDlg,DWLP_MSGRESULT,FALSE);
      else
        SetWindowLong(hwndDlg,DWLP_MSGRESULT,TRUE);
      return TRUE;
    }

  return FALSE;
}

BOOL IconHidePage::DoCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam UNUSED)
{
  switch (LOWORD(wParam))
    {
    case IDC_EDITTIP:
      return DoEdit(hwndDlg);
    case IDC_DELTIP:
      return DoDelete(hwndDlg);
    case IDC_ADDTIP:
      return DoAdd(hwndDlg);
    case IDC_SAVETIP:
      return DoSave(hwndDlg);
    case IDC_ABORTTIP:
      return DoAbort(hwndDlg);
    }

  return FALSE;
}

bool IconHidePage::UpdateSettings(HWND hwndDlg)
{
  if (SendDlgItemMessage(hwndDlg, IDC_UNHIDE, BM_GETCHECK, 0, 0) == BST_CHECKED)
    pSettings->SetUnhideIcons(true);
  else if (SendDlgItemMessage(hwndDlg, IDC_UNHIDE, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
    pSettings->SetUnhideIcons(false);

  pSettings->WriteSettings();
  pSettings->WriteHideList();

  return true;
}

bool IconHidePage::DoEdit(HWND hwndDlg UNUSED)
{
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVETIP);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTTIP);
  HWND appWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);

  EnableWindow(appWnd, true);
  EnableWindow(saveWnd, true);
  EnableWindow(abortWnd, true);
  edit = true;

  return true;
}

bool IconHidePage::DoAbort(HWND hwndDlg)
{
  HWND listWnd = GetDlgItem(hwndDlg, IDC_HIDELIST);
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVETIP);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTTIP);
  HWND appWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);
  WCHAR tipText[MAX_LINE_LENGTH];
  int i = 0;

  while (i < ListView_GetItemCount(listWnd))
    {
      if (ListView_GetItemState(listWnd, i, LVIS_SELECTED))
        break;

      i++;
    }
  ListView_GetItemText(listWnd, i, 0, tipText, MAX_LINE_LENGTH);
  SetDlgItemText(hwndDlg, IDC_ICONTEXT, tipText);
  edit = false;

  EnableWindow(saveWnd, false);
  EnableWindow(abortWnd, false);
  EnableWindow(appWnd, false);

  return true;
}

bool IconHidePage::DoSave(HWND hwndDlg)
{
  HWND listWnd = GetDlgItem(hwndDlg, IDC_HIDELIST);
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVETIP);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTTIP);
  HWND appWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);
  bool ret = false;
  LVFINDINFO lvFI;
  LVITEM lvItem;
  WCHAR tmp[MAX_LINE_LENGTH], error[MAX_LINE_LENGTH];

  ZeroMemory(tmp, MAX_LINE_LENGTH);

  lvFI.flags = LVFI_STRING;
  lvItem.mask = LVIF_TEXT;

  GetDlgItemText(hwndDlg, IDC_ICONTEXT, tmp, MAX_LINE_LENGTH);
  if (wcslen(tmp) > 0)
    {
      lvItem.iSubItem = 0;
      lvItem.pszText = tmp;
      lvItem.cchTextMax = (int)wcslen(tmp);
      lvFI.psz = tmp;
      if (ListView_FindItem(listWnd, 0, &lvFI) != -1)
        {
          swprintf(error, TEXT("%s is already in the hidden icon list"), tmp);
          ELMessageBox(GetDesktopWindow(), error, (WCHAR*)TEXT("emergeTray"),
                       ELMB_OK|ELMB_ICONERROR|ELMB_MODAL);
        }
      else
        {
          int i = 0;
          if (edit)
            {
              while (i < ListView_GetItemCount(listWnd))
                {
                  if (ListView_GetItemState(listWnd, i, LVIS_SELECTED))
                    break;

                  i++;
                }
              if (ListView_DeleteItem(listWnd, i))
                {
                  pSettings->ModifyHideListItem(i, tmp);
                  lvItem.iItem = i;
                }
            }
          else
            {
              pSettings->AddHideListItem(tmp);
              lvItem.iItem = ListView_GetItemCount(listWnd);
            }

          if (ListView_InsertItem(listWnd, &lvItem) != -1)
            {
              i = 0;
              while (i < ListView_GetItemCount(listWnd))
                {
                  if (i == lvItem.iItem)
                    {
                      ListView_SetItemState(listWnd, i, LVIS_SELECTED, LVIS_SELECTED);
                    }
                  else
                    {
                      ListView_SetItemState(listWnd, i, 0, LVIS_SELECTED);
                    }

                  i++;
                }
              saveCount++;
              ret = true;
            }
        }
    }

  edit = false;
  EnableWindow(saveWnd, false);
  EnableWindow(abortWnd, false);
  EnableWindow(appWnd, false);

  return ret;
}

bool IconHidePage::DoDelete(HWND hwndDlg)
{
  HWND listWnd = GetDlgItem(hwndDlg, IDC_HIDELIST);
  HWND delWnd = GetDlgItem(hwndDlg, IDC_DELTIP);
  HWND editWnd = GetDlgItem(hwndDlg, IDC_EDITTIP);
  bool ret = false;
  UINT i = 0;
  int prevItem = 0;
  BOOL bRet;

  if (ListView_GetSelectedCount(listWnd) > 1)
    {
      ELMessageBox(hwndDlg, (WCHAR*)TEXT("You can only delete one item at a time."),
                   (WCHAR*)TEXT("emergeTray"), ELMB_OK|ELMB_ICONERROR|ELMB_MODAL);

      return ret;
    }

  while (i < pSettings->GetHideListSize())
    {
      if (ListView_GetItemState(listWnd, i, LVIS_SELECTED))
        {
          deleteCount++;
          ret = true;
          prevItem = ListView_GetNextItem(listWnd, i, LVNI_ABOVE);
          bRet = ListView_DeleteItem(listWnd, i);
          pSettings->DeleteHideListItem(i);

          ListView_SetItemState(listWnd, i, LVIS_SELECTED,
                                LVIS_SELECTED);
          if (!ListView_EnsureVisible(listWnd, i, FALSE))
            {
              if (prevItem != -1)
                {
                  ListView_SetItemState(listWnd, prevItem, LVIS_SELECTED,
                                        LVIS_SELECTED);
                  bRet = ListView_EnsureVisible(listWnd, prevItem, FALSE);
                }
            }

          break;
        }
      else
        i++;
    }

  if (ListView_GetItemCount(listWnd) == 0)
    {
      EnableWindow(editWnd, false);
      EnableWindow(delWnd, false);
    }

  return ret;
}

bool IconHidePage::DoAdd(HWND hwndDlg)
{
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVETIP);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTTIP);
  HWND appWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);

  SetDlgItemText(hwndDlg, IDC_ICONTEXT, TEXT(""));

  EnableWindow(saveWnd, true);
  EnableWindow(abortWnd, true);
  EnableWindow(appWnd, true);

  return true;
}

bool IconHidePage::CheckFields(HWND hwndDlg)
{
  WCHAR tmp[MAX_LINE_LENGTH];
  HWND textWnd = GetDlgItem(hwndDlg, IDC_ICONTEXT);

  if (!IsWindowEnabled(textWnd))
    return true;

  if (GetWindowText(textWnd, tmp, MAX_LINE_LENGTH) != 0)
    {
      if (ELMessageBox(hwndDlg,
                       (WCHAR*)TEXT("The current icon will be lost.\n\nDo you wish to continue?"),
                       (WCHAR*)TEXT("emergeTray"),
                       ELMB_YESNO|ELMB_ICONQUESTION|ELMB_MODAL) == IDYES)
        return true;
      else
        return false;
    }

  return true;
}