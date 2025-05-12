#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    TalkingState(Context &context, common::PhoneNumber from);
    void returnToConnectedState();
    void handleCallTalk(common::PhoneNumber from, const std::string& message);
    void sendCallTalk();
};
}