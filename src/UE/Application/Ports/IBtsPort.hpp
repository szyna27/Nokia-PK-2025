#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Messages/MessageId.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnect() = 0;

    virtual void handleSMS(common::PhoneNumber from, const std::string& message) = 0;
    
    // Call events
    virtual void handleCallRequest(common::PhoneNumber) = 0;
    virtual void handleCallAccept(common::PhoneNumber) = 0;
    virtual void handleCallDropped(common::PhoneNumber) = 0;
    virtual void handleCallTalk(const std::string message) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendSMS(common::PhoneNumber to, const std::string& message) = 0;
    
    // Calling
    virtual void sendCallRequest(common::PhoneNumber) = 0;
    virtual void sendCallAccept(common::PhoneNumber) = 0;
    virtual void sendCallDrop(common::PhoneNumber) = 0;
    virtual void sendCallTalk(common::PhoneNumber, std::string text) = 0;
};

}
