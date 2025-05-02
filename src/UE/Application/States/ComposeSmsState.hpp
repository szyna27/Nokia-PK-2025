#pragma once

#include "BaseState.hpp"

namespace ue
{

class ComposeSmsState : public BaseState
{
public:
    ComposeSmsState(Context& context);
    void returnToConnectedState();
    void sendSMS();

};

}