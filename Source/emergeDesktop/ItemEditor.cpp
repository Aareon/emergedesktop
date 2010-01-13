//----  --------------------------------------------------------------------------------------------------------
//
//  This file is part of Emerge Desktop.
//  Copyright (C) 2004-2010  The Emerge Desktop Development Team
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

#include "ItemEditor.h"

BOOL CALLBACK ItemEditor::MenuDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  static ItemEditor *pItemEditor = NULL;

  switch (message)
    {
    case WM_INITDIALOG:
      pItemEditor = reinterpret_cast<ItemEditor*>(lParam);
      if (!pItemEditor)
        break;
      return pItemEditor->DoInitDialog(hwndDlg);

    case WM_COMMAND:
      return pItemEditor->DoMenuCommand(hwndDlg, wParam, lParam);
    }

  return FALSE;
}

ItemEditor::ItemEditor(HINSTANCE hInstance, HWND mainWnd)
{
  (*this).hInstance = hInstance;
  (*this).mainWnd = mainWnd;
  section = NULL;

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

  browseIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_BROWSE), IMAGE_ICON, 16, 16, 0);

  dialogVisible = false;
}

ItemEditor::~ItemEditor()
{
  if (browseIcon)
    DestroyIcon(browseIcon);

  DestroyWindow(toolWnd);
}

int ItemEditor::Show(TiXmlElement *section, WCHAR *name, WCHAR *value, UINT type, WCHAR *workingDir)
{
  (*this).section = section;
  wcscpy((*this).name, name);
  wcscpy((*this).value, value);
  (*this).type = type;
  wcscpy((*this).workingDir, workingDir);
  dialogVisible = true;
  return (int)DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ITEMEDIT), mainWnd, (DLGPROC)MenuDlgProc, (LPARAM)this);
}

BOOL ItemEditor::DoInitDialog(HWND hwndDlg)
{
  RECT rect;
  int x, y;
  TOOLINFO ti;

  ZeroMemory(&ti, sizeof(TOOLINFO));
  GetWindowRect(hwndDlg, &rect);

  HWND nameWnd = GetDlgItem(hwndDlg, IDC_ITEMNAME);
  HWND nameTextWnd = GetDlgItem(hwndDlg, IDC_NAMETEXT);
  HWND typeWnd = GetDlgItem(hwndDlg, IDC_ITEMTYPE);
  HWND typeTextWnd = GetDlgItem(hwndDlg, IDC_TYPETEXT);
  HWND valueWnd = GetDlgItem(hwndDlg, IDC_ITEMVALUE);
  HWND valueTextWnd = GetDlgItem(hwndDlg, IDC_VALUETEXT);
  HWND commandWnd = GetDlgItem(hwndDlg, IDC_ITEMCOMMAND);
  HWND saveWnd = GetDlgItem(hwndDlg, IDC_SAVEITEM);
  HWND abortWnd = GetDlgItem(hwndDlg, IDC_ABORTITEM);
  HWND browseWnd = GetDlgItem(hwndDlg, IDC_ITEMBROWSE);
  HWND dirWnd = GetDlgItem(hwndDlg, IDC_WORKINGDIR);
  HWND workingDirTextWnd = GetDlgItem(hwndDlg, IDC_WORKINGDIRTEXT);
  HWND dirBrowseWnd = GetDlgItem(hwndDlg, IDC_DIRBROWSE);
  HWND specialFoldersWnd = GetDlgItem(hwndDlg, IDC_ITEMSPECIALFOLDERS);

  x = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((rect.right - rect.left) / 2);
  y = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((rect.bottom - rect.top) / 2);
  SetWindowPos(hwndDlg, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
  ELStealFocus(hwndDlg);

  PopulateCommands(commandWnd);
  ShowWindow(commandWnd, false);

  PopulateSpecialFolders(specialFoldersWnd);
  ShowWindow(specialFoldersWnd, false);

  if (browseIcon)
    {
      SendMessage(browseWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)browseIcon);
      SendMessage(dirBrowseWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)browseIcon);
    }

  ti.cbSize = TTTOOLINFOW_V2_SIZE;
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = saveWnd;
  ti.uId = (ULONG_PTR)saveWnd;
  ti.lpszText = (WCHAR*)TEXT("Save Changes");
  GetClientRect(saveWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = abortWnd;
  ti.uId = (ULONG_PTR)abortWnd;
  ti.lpszText = (WCHAR*)TEXT("Discard Changes");
  GetClientRect(abortWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  ti.hwnd = browseWnd;
  ti.uId = (ULONG_PTR)browseWnd;
  ti.lpszText = (WCHAR*)TEXT("");
  GetClientRect(browseWnd, &ti.rect);
  SendMessage(toolWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

  SetDlgItemText(hwndDlg, IDC_ITEMNAME, name);
  SetDlgItemText(hwndDlg, IDC_WORKINGDIR, workingDir);

  EnableWindow(nameWnd, true);
  EnableWindow(nameTextWnd, true);
  EnableWindow(typeWnd, true);
  EnableWindow(typeTextWnd, true);
  EnableWindow(valueWnd, false);
  EnableWindow(valueTextWnd, false);
  EnableWindow(saveWnd, false);
  EnableWindow(abortWnd, false);
  EnableWindow(commandWnd, false);
  EnableWindow(browseWnd, false);
  EnableWindow(dirWnd, false);
  EnableWindow(workingDirTextWnd, false);
  EnableWindow(dirBrowseWnd, false);
  EnableWindow(specialFoldersWnd, false);

  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Separator"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Executable"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Internal Command"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("DateTime"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Special Folder"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Submenu"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Folder Menu"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Tasks Menu"));
  SendMessage(typeWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Settings Menu"));

  SendMessage(typeWnd, CB_SETCURSEL,
              (WPARAM)GetTypeValue(type), 0);
  switch (type)
    {
    case 2:
      SendMessage(commandWnd, CB_SETCURSEL,
                  SendMessage(commandWnd,
                              CB_FINDSTRINGEXACT,
                              (WPARAM)-1,
                              (LPARAM)value),
                  0);
      break;
    case 4:
    {
      int folder = ELIsSpecialFolder(value);
      if (ELGetSpecialFolder(folder, value))
        SendMessage(specialFoldersWnd, CB_SETCURSEL,
                    SendMessage(specialFoldersWnd,
                                CB_FINDSTRINGEXACT,
                                (WPARAM)-1,
                                (LPARAM)value),
                    0);
    }
    break;
    default:
      SetDlgItemText(hwndDlg, IDC_ITEMVALUE, value);
      break;
    }
  EnableFields(hwndDlg);

  return TRUE;
}

void ItemEditor::PopulateCommands(HWND commandWnd)
{
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("RightDeskMenu"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("MidDeskMenu"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Quit"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Run"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Shutdown"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Lock"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Hide"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Show"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_1"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_2"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_3"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_4"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_5"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_6"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_7"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_8"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWM_9"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWMUp"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWMDown"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWMLeft"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWMRight"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VWMGather"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("EmptyBin"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Logoff"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Reboot"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Halt"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Suspend"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Hibernate"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("Disconnect"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ShowDesktop"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VolumeUp"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VolumeDown"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("VolumeMute"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("DesktopSettings"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("DesktopItemEditor"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("CoreLaunchEditor"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("CoreShellChanger"));
  SendMessage(commandWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("CoreAbout"));
}

void ItemEditor::PopulateSpecialFolders(HWND specialFoldersWnd)
{
  WCHAR tmp[MAX_PATH];

  if (ELGetSpecialFolder(CSIDL_DRIVES, tmp))
    SendMessage(specialFoldersWnd, CB_ADDSTRING, 0, (LPARAM)tmp);
  if (ELGetSpecialFolder(CSIDL_NETWORK, tmp))
    SendMessage(specialFoldersWnd, CB_ADDSTRING, 0, (LPARAM)tmp);
  if (ELGetSpecialFolder(CSIDL_CONTROLS, tmp))
    SendMessage(specialFoldersWnd, CB_ADDSTRING, 0, (LPARAM)tmp);
  if (ELGetSpecialFolder(CSIDL_BITBUCKET, tmp))
    SendMessage(specialFoldersWnd, CB_ADDSTRING, 0, (LPARAM)tmp);
}

bool ItemEditor::GetVisible()
{
  return dialogVisible;
}

BOOL ItemEditor::DoMenuCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam UNUSED)
{
  switch (LOWORD(wParam))
    {
    case IDOK:
      if (!DoSaveItem(hwndDlg))
        return FALSE;
    case IDCANCEL:
      dialogVisible = false;
      EndDialog(hwndDlg, wParam);
      return TRUE;
    case IDC_ITEMTYPE:
      if (HIWORD(wParam) == CBN_SELCHANGE)
        return DoSelChange(hwndDlg, (HWND)lParam);
      break;
    case IDC_ITEMBROWSE:
      return DoBrowseItem(hwndDlg, false);
    case IDC_DIRBROWSE:
      return DoBrowseItem(hwndDlg, true);
    }

  return FALSE;
}

bool ItemEditor::DoBrowseItem(HWND hwndDlg, bool workingDir)
{
  OPENFILENAME ofn;
  BROWSEINFO bi;
  HWND typeWnd = GetDlgItem(hwndDlg, IDC_ITEMTYPE);
  WCHAR tmp[MAX_PATH], program[MAX_PATH], arguments[MAX_LINE_LENGTH];
  WCHAR initPath[MAX_PATH];
  UINT type = GetValueType((int)SendMessage(typeWnd, CB_GETCURSEL, 0, 0));
  std::wstring workingPath;

  ZeroMemory(tmp, MAX_PATH);

  if ((type == 101) || workingDir)
    {
      ZeroMemory(&bi, sizeof(BROWSEINFO));
      bi.hwndOwner = hwndDlg;
      bi.ulFlags = BIF_NEWDIALOGSTYLE;
      bi.lpszTitle = TEXT("Select a folder:");

      LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

      if (pItemIDList != NULL)
        {
          if (SHGetPathFromIDList(pItemIDList, tmp))
            {
              IMalloc* pMalloc = NULL;
              if (SUCCEEDED(SHGetMalloc(&pMalloc)))
                {
                  pMalloc->Free(pItemIDList);
                  pMalloc->Release();
                }

              ELUnExpandVars(tmp);
              if (workingDir)
                SetDlgItemText(hwndDlg, IDC_WORKINGDIR, tmp);
              else
                SetDlgItemText(hwndDlg, IDC_ITEMVALUE, tmp);

              return true;
            }
        }
    }
  else
    {
      ZeroMemory(&ofn, sizeof(ofn));

      ELGetCurrentPath(initPath);

      ofn.lpstrInitialDir = initPath;
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = hwndDlg;
      ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
      GetDlgItemText(hwndDlg, IDC_ITEMVALUE, tmp, MAX_PATH);
      workingPath = tmp;
      workingPath = ELExpandVars(workingPath);
      if (ELParseCommand(workingPath.c_str(), program, arguments))
        wcscpy(tmp, program);
      ofn.lpstrFile = tmp;
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrTitle = TEXT("Browse For File");
      ofn.lpstrDefExt = NULL;
      ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR | OFN_NODEREFERENCELINKS;

      if (GetOpenFileName(&ofn))
        {
          ELUnExpandVars(tmp);
          if (type == 4)
            swprintf(tmp, TEXT("*%s"), PathFindFileName(tmp));

          SetDlgItemText(hwndDlg, IDC_ITEMVALUE, tmp);

          return true;
        }
    }

  return false;
}

int ItemEditor::GetTypeValue(UINT type)
{
  switch (type)
    {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 3;
    case 4:
      return 4;
    case 100:
      return 5;
    case 101:
      return 6;
    case 102:
      return 7;
    case 103:
      return 8;
    }

  return -1;
}

bool ItemEditor::EnableFields(HWND hwndDlg)
{
  HWND nameWnd = GetDlgItem(hwndDlg, IDC_ITEMNAME);
  HWND nameTextWnd = GetDlgItem(hwndDlg, IDC_NAMETEXT);
  HWND typeWnd = GetDlgItem(hwndDlg, IDC_ITEMTYPE);
  HWND typeTextWnd = GetDlgItem(hwndDlg, IDC_TYPETEXT);
  HWND valueWnd = GetDlgItem(hwndDlg, IDC_ITEMVALUE);
  HWND valueTextWnd = GetDlgItem(hwndDlg, IDC_VALUETEXT);
  HWND commandWnd = GetDlgItem(hwndDlg, IDC_ITEMCOMMAND);
  HWND browseWnd = GetDlgItem(hwndDlg, IDC_ITEMBROWSE);
  HWND dirWnd = GetDlgItem(hwndDlg, IDC_WORKINGDIR);
  HWND workingDirTextWnd = GetDlgItem(hwndDlg, IDC_WORKINGDIRTEXT);
  HWND dirBrowseWnd = GetDlgItem(hwndDlg, IDC_DIRBROWSE);
  HWND specialFoldersWnd = GetDlgItem(hwndDlg, IDC_ITEMSPECIALFOLDERS);
  int index = (int)SendMessage(typeWnd, CB_GETCURSEL, 0, 0);

  EnableWindow(typeWnd, true);
  EnableWindow(typeTextWnd, true);

  switch (index)
    {
    case 0:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(nameWnd, false);
      EnableWindow(nameTextWnd, false);
      EnableWindow(valueWnd, false);
      EnableWindow(valueTextWnd, false);
      EnableWindow(browseWnd, false);
      break;
    case 2:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, false);
      ShowWindow(browseWnd, false);
      ShowWindow(specialFoldersWnd, false);
      ShowWindow(commandWnd, true);
      EnableWindow(commandWnd, true);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      EnableWindow(valueTextWnd, true);
      break;
    case 3:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(valueWnd, true);
      EnableWindow(valueTextWnd, true);
      EnableWindow(browseWnd, false);
      EnableWindow(nameWnd, false);
      EnableWindow(nameTextWnd, true);
      break;
    case 4:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, false);
      ShowWindow(browseWnd, false);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, true);
      EnableWindow(specialFoldersWnd, true);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      EnableWindow(valueTextWnd, true);
      break;
    case 5:
    case 7:
    case 8:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(valueWnd, false);
      EnableWindow(valueTextWnd, false);
      EnableWindow(browseWnd, false);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      break;
    case 1:
      EnableWindow(dirWnd, true);
      EnableWindow(workingDirTextWnd, true);
      EnableWindow(dirBrowseWnd, true);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      EnableWindow(valueWnd, true);
      EnableWindow(valueTextWnd, true);
      EnableWindow(browseWnd, true);
      break;
    case 6:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      EnableWindow(valueWnd, true);
      EnableWindow(valueTextWnd, true);
      EnableWindow(browseWnd, true);
      break;
    default:
      EnableWindow(dirWnd, false);
      EnableWindow(workingDirTextWnd, false);
      EnableWindow(dirBrowseWnd, false);
      ShowWindow(valueWnd, true);
      ShowWindow(browseWnd, true);
      ShowWindow(commandWnd, false);
      ShowWindow(specialFoldersWnd, false);
      EnableWindow(nameWnd, true);
      EnableWindow(nameTextWnd, true);
      EnableWindow(valueWnd, false);
      EnableWindow(valueTextWnd, false);
      EnableWindow(browseWnd, false);
    }

  return true;
}

bool ItemEditor::DoSelChange(HWND hwndDlg, HWND typeWnd)
{
  HWND browseWnd = GetDlgItem(hwndDlg, IDC_ITEMBROWSE);

  SetDlgItemText(hwndDlg, IDC_ITEMVALUE, TEXT("\0"));

  SetTooltip(browseWnd, GetValueType((int)SendMessage(typeWnd, CB_GETCURSEL, 0, 0)));

  return EnableFields(hwndDlg);
}

UINT ItemEditor::GetValueType(int value)
{
  switch (value)
    {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 3;
    case 4:
      return 4;
    case 5:
      return 100;
    case 6:
      return 101;
    case 7:
      return 102;
    case 8:
      return 103;
    }

  return 0;
}

void ItemEditor::SetTooltip(HWND browseWnd, UINT type)
{
  TOOLINFO ti;
  ZeroMemory(&ti, sizeof(TOOLINFO));

  ti.cbSize = TTTOOLINFOW_V2_SIZE;
  ti.hwnd = browseWnd;
  ti.uId = (ULONG_PTR)browseWnd;

  SendMessage(toolWnd, TTM_GETTOOLINFO, 0,(LPARAM) (LPTOOLINFO) &ti);

  switch (type)
    {
    case 1:
      ti.lpszText = (WCHAR*)TEXT("Browse for a file");
      break;
    case 4:
      ti.lpszText = (WCHAR*)TEXT("Browse for an ELScript");
      break;
    case 101:
      ti.lpszText = (WCHAR*)TEXT("Browse for a directory");
    }

  SendMessage(toolWnd, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
}

bool ItemEditor::DoSaveItem(HWND hwndDlg)
{
  HWND typeWnd = GetDlgItem(hwndDlg, IDC_ITEMTYPE);
  HWND nameWnd = GetDlgItem(hwndDlg, IDC_ITEMNAME);
  WCHAR name[MAX_LINE_LENGTH], value[MAX_LINE_LENGTH], workingDir[MAX_LINE_LENGTH], tmp[MAX_LINE_LENGTH];
  ZeroMemory(name, MAX_LINE_LENGTH);
  ZeroMemory(workingDir, MAX_LINE_LENGTH);
  ZeroMemory(tmp, MAX_LINE_LENGTH);
  int type;

  if (IsWindowEnabled(nameWnd))
    {
      GetDlgItemText(hwndDlg, IDC_ITEMNAME, name, MAX_LINE_LENGTH);
      if (wcslen(name) == 0)
        {
          ELMessageBox(hwndDlg, (WCHAR*)TEXT("Name cannot be empty"), (WCHAR*)TEXT("emergeDesktop"),
                       ELMB_OK|ELMB_ICONERROR|ELMB_MODAL);
          return false;
        }
    }

  type = GetValueType((int)SendMessage(typeWnd, CB_GETCURSEL, 0, 0));
  switch (type)
    {
    case 2:
      GetDlgItemText(hwndDlg, IDC_ITEMCOMMAND, value, MAX_LINE_LENGTH);
      break;
    case 4:
      GetDlgItemText(hwndDlg, IDC_ITEMSPECIALFOLDERS, tmp, MAX_LINE_LENGTH);
      if (!ELSpecialFolderValue(tmp, value))
        wcscpy(value, TEXT(""));
      break;
    default:
      GetDlgItemText(hwndDlg, IDC_ITEMVALUE, value, MAX_LINE_LENGTH);
      break;
    }
  GetDlgItemText(hwndDlg, IDC_WORKINGDIR, workingDir, MAX_LINE_LENGTH);

  ELWriteXMLStringValue(section, (WCHAR*)TEXT("Name"), name);
  ELWriteXMLIntValue(section, (WCHAR*)TEXT("Type"), type);
  if (wcslen(value) > 0)
    ELWriteXMLStringValue(section, (WCHAR*)TEXT("Value"), value);
  if (wcslen(workingDir) > 0)
    ELWriteXMLStringValue(section, (WCHAR*)TEXT("WorkingDir"), workingDir);

  TiXmlDocument *configXML = ELGetXMLConfig(section);
  if (configXML)
    return ELWriteXMLConfig(configXML);

  return false;
}