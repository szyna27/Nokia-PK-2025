#pragma once

#include "BaseState.hpp"
#include "IListViewMode.hpp"

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
};

}
