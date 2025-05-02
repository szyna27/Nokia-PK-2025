#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
class ComposingSmsState : public BaseState
{
public:
    ComposingSmsState(Context& context);
    void handleAccept();
    void handleReject();
    void handleDisconnect() override;
    void handleSms(common::PhoneNumber from, std::string message) override;
};
} 