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
    void handleCallTalk(const std::string message);
    void sendCallTalk();
    void callDropped();
    void handleCallDropped(common::PhoneNumber from);
    void handleTimeout() override;
    void handleUnknownRecipent();
    void handleCallRequest(common::PhoneNumber from) override;
};
}