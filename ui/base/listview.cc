//
// PROJECT:         Aspia Remote Desktop
// FILE:            ui/base/listview.cc
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "ui/base/listview.h"
#include "base/version_helpers.h"

#include <uxtheme.h>

namespace aspia {

ListView::ListView(HWND hwnd)
{
    Attach(hwnd);

    if (IsWindowsVistaOrGreater())
        SetWindowTheme(hwnd, L"explorer", nullptr);
}

int ListView::GetColumnCount()
{
    HWND header_window = ListView_GetHeader(hwnd());

    if (!header_window)
        return 0;

    return Header_GetItemCount(header_window);
}

int ListView::GetItemCount()
{
    return ListView_GetItemCount(hwnd());
}

void ListView::AddOnlyOneColumn(const std::wstring& title)
{
    RECT list_rect = { 0 };
    GetClientRect(hwnd(), &list_rect);

    LVCOLUMNW column = { 0 };

    column.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    column.cx      = (list_rect.right - list_rect.left) - GetSystemMetrics(SM_CXVSCROLL);
    column.fmt     = LVCFMT_LEFT;
    column.pszText = const_cast<LPWSTR>(title.c_str());

    ListView_InsertColumn(hwnd(), 0, &column);
}

void ListView::AddColumn(const std::wstring& title, int width)
{
    int column_count = GetColumnCount();

    LVCOLUMNW column = { 0 };

    column.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    column.cx      = width;
    column.fmt     = LVCFMT_LEFT;
    column.pszText = const_cast<LPWSTR>(title.c_str());

    ListView_InsertColumn(hwnd(), column_count, &column);
}

void ListView::DeleteAllItems()
{
    ListView_DeleteAllItems(hwnd());
}

void ListView::SetCheckState(int item_index, bool checked)
{
    ListView_SetCheckState(hwnd(), item_index, checked);
}

bool ListView::GetCheckState(int item_index)
{
    return !!ListView_GetCheckState(hwnd(), item_index);
}

bool ListView::ModifyExtendedListViewStyle(DWORD remove, DWORD add)
{
    DWORD style = ListView_GetExtendedListViewStyle(hwnd());

    DWORD new_style = (style & ~remove) | add;

    if (style == new_style)
        return false;

    ListView_SetExtendedListViewStyle(hwnd(), new_style);

    return true;
}

void ListView::SetImageList(HIMAGELIST imagelist, int type)
{
    ListView_SetImageList(hwnd(), imagelist, type);
}

int ListView::GetFirstSelectedItem()
{
    return ListView_GetNextItem(hwnd(), -1, LVNI_SELECTED);
}

} // namespace aspia
