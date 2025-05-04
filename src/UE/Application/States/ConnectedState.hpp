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
    void handleDisconnect() override;
    void selectMode(IUeGui::IListViewMode& menu);
    void sendSMS();

private:
    void changeMode(unsigned int mode);

};

}
