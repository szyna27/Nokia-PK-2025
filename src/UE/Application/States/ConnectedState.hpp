#pragma once

#include "BaseState.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

enum Modes
{
    COMPOSE_SMS,
    VIEW_SMS,
    DIAL,
    TALKING,
    MAIN_MENU
};

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    bool isCalling = false;

    void selectMode(IUeGui::IListViewMode& menu);

    // handle events
    virtual void handleDisconnect() override;
    virtual void handleSMS(common::PhoneNumber from, const std::string& message) override;
    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallAccept(common::PhoneNumber from);
    void handleCallDropped(common::PhoneNumber from);
    void handleTimeout() override;

    // send events
    void sendSMS();
    void sendCallRequest();
    void sendCallAccept(common::PhoneNumber from);
    void sendCallDrop(common::PhoneNumber from);
    
    virtual void handleUIAction(std::optional<std::size_t> selectedIndex) override;
    virtual void handleUIBack() override;

    void dropCall();

private:
    void changeMode(unsigned int mode);

};

}
