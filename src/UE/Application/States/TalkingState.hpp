#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
public:
    TalkingState(Context &context, common::PhoneNumber from);
    common::PhoneNumber peerNumber;
    void returnToConnectedState();
    void handleCallTalk(common::PhoneNumber from, const std::string& message);
    void sendCallTalk();
    void callDropped();
    void handleCallDropped(common::PhoneNumber from);
};
}