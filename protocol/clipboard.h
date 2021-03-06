//
// PROJECT:         Aspia Remote Desktop
// FILE:            protocol/clipboard.h
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#ifndef _ASPIA_PROTOCOL__CLIPBOARD_H
#define _ASPIA_PROTOCOL__CLIPBOARD_H

#include <functional>
#include <memory>
#include <string>

#include "base/message_window.h"
#include "proto/desktop_session_message.pb.h"

namespace aspia {

class Clipboard
{
public:
    Clipboard() = default;
    ~Clipboard();

    using ClipboardEventCallback =
        std::function<void(std::unique_ptr<proto::ClipboardEvent> clipboard_event)>;

    // Callback is called when there is an outgoing clipboard.
    bool Start(ClipboardEventCallback clipboard_event_callback);

    void Stop();

    // Receiving the incoming clipboard.
    void InjectClipboardEvent(std::shared_ptr<proto::ClipboardEvent> clipboard_event);

private:
    // Handles messages received by |window_|.
    bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);

    bool HaveClipboardListenerApi();
    void OnClipboardUpdate();

    typedef BOOL(WINAPI AddClipboardFormatListenerFn)(HWND);
    typedef BOOL(WINAPI RemoveClipboardFormatListenerFn)(HWND);

    AddClipboardFormatListenerFn* add_clipboard_format_listener_ = nullptr;
    RemoveClipboardFormatListenerFn* remove_clipboard_format_listener_ = nullptr;

    HWND next_viewer_window_ = nullptr;

    // Used to subscribe to WM_CLIPBOARDUPDATE messages.
    std::unique_ptr<MessageWindow> window_;

    ClipboardEventCallback clipboard_event_callback_;

    std::string last_mime_type_;
    std::string last_data_;

    DISALLOW_COPY_AND_ASSIGN(Clipboard);
};

} // namespace aspia

#endif // _ASPIA_PROTOCOL__CLIPBOARD_H
