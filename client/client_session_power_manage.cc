//
// PROJECT:         Aspia Remote Desktop
// FILE:            client/client_session_power_manage.cc
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "client/client_session_power_manage.h"
#include "protocol/message_serialization.h"
#include "ui/power_manage_dialog.h"

namespace aspia {

ClientSessionPowerManage::ClientSessionPowerManage(const ClientConfig& config,
                                                   ClientSession::Delegate* delegate) :
    ClientSession(config, delegate)
{
    ui_thread_.Start(MessageLoop::Type::TYPE_UI, this);
}

ClientSessionPowerManage::~ClientSessionPowerManage()
{
    ui_thread_.Stop();
}

void ClientSessionPowerManage::Send(const IOBuffer& buffer)
{
    // The power management session does not receive any messages.
}

void ClientSessionPowerManage::OnBeforeThreadRunning()
{
    ui_thread_.StopSoon();
}

void ClientSessionPowerManage::OnAfterThreadRunning()
{
    PowerManageDialog dialog;

    proto::PowerEvent::Action action =
        static_cast<proto::PowerEvent::Action>(dialog.DoModal(nullptr));

    if (action != proto::PowerEvent::UNKNOWN)
    {
        proto::power::ClientToHost message;
        message.mutable_power_event()->set_action(action);

        IOBuffer buffer(SerializeMessage<IOBuffer>(message));

        delegate_->OnSessionMessage(std::move(buffer));
    }

    delegate_->OnSessionTerminate();
}

} // namespace aspia
