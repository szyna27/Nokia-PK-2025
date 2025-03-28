#pragma once

#include "BaseState.hpp"

namespace ue
{

class NotConnectedState : public BaseState
{
public:
    NotConnectedState(Context& context, bool isFirstState = true);
    void handleSib(common::BtsId btsId) override;
};

}
