//
// PROJECT:         Aspia Remote Desktop
// FILE:            ui/base/imagelist.cc
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "ui/base/imagelist.h"

namespace aspia {

static int GetICLColor()
{
    DEVMODEW mode = { 0 };
    mode.dmSize = sizeof(mode);

    if (EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &mode))
    {
        switch (mode.dmBitsPerPel)
        {
            case 32:
                return ILC_COLOR32;

            case 24:
                return ILC_COLOR24;

            case 16:
                return ILC_COLOR16;

            case 8:
                return ILC_COLOR8;

            case 4:
                return ILC_COLOR4;
        }
    }

    return ILC_COLOR32;
}

ImageList::~ImageList()
{
    Close();
}

bool ImageList::Create(int width, int height, UINT flags, int initial, int grow)
{
    Close();

    list_ = ImageList_Create(width, height, flags, initial, grow);
    if (!list_)
        return false;

    return true;
}

bool ImageList::CreateSmall()
{
    return Create(GetSystemMetrics(SM_CXSMICON),
                  GetSystemMetrics(SM_CYSMICON),
                  ILC_MASK | GetICLColor(),
                  1, 1);
}

int ImageList::AddIcon(HICON icon)
{
    return ImageList_AddIcon(list_, icon);
}

int ImageList::AddIcon(const Module& module, UINT resource_id)
{
    int width = 0;
    int height = 0;

    if (ImageList_GetIconSize(list_, &width, &height))
    {
        ScopedHICON icon(module.icon(resource_id, width, height, LR_CREATEDIBSECTION));
        return AddIcon(icon);
    }

    return -1;
}

void ImageList::Close()
{
    if (list_)
    {
        ImageList_Destroy(list_);
        list_ = nullptr;
    }
}

} // namespace aspia
