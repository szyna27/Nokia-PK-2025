#pragma once

#include "BaseState.hpp"
#include "UeGui/IListViewMode.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

enum Modes
{
    NONE = -1,
    COMPOSE_SMS,
    VIEW_SMS,
    DIAL,
    TALKING
};

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnect() override;
    void selectMode(IUeGui::IListViewMode& menu);
    void handleSms(common::PhoneNumber from, std::string message) override;
};

}
