#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this]() {handleDisconnect();});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleDisconnect()
{
    if (handler)
        handler->handleDisconnect();
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            std::string text = reader.readRemainingText();
            handler->handleSMS(from, text);
            break;
        }
        case common::MessageId::CallRequest:
        {
            logger.logInfo("Call request from: ", from);
            handler->handleCallRequest(from);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            logger.logInfo("Call accept from: ", from);
            handler->handleCallAccept(from);
            break;
        }
        case common::MessageId::CallDropped:
        {
            logger.logInfo("Call drop from: ", from);
            handler->handleCallDropped(from);
            break;
        }
        case common::MessageId::CallTalk:
        {
            std::string text = reader.readRemainingText();
            logger.logInfo("Call talk from: ", from, ", text: ", text);
            handler->handleCallTalk(from);
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            logger.logError("Unknown recipient: ", from, " to: ", to);
            if (to == phoneNumber)
            {
                logger.logError("Call treated as droped");
                handler->handleCallDropped(from);
            }
            break;
        }
        default:
            logger.logError("unknow message: ", msgId, ", from: ", from);
        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());


}


void BtsPort::sendSMS(common::PhoneNumber receiver, const std::string &message)
{
    logger.logDebug("sendSMS from : ", phoneNumber, " to: ", receiver, 
        ",message: ", message);
        common::OutgoingMessage msg{common::MessageId::Sms,
            phoneNumber,
            receiver};
            msg.writeText(message);
            transport.sendMessage(msg.getMessage());
}

// Calling
void BtsPort::sendCallRequest(common::PhoneNumber receiver)
{
    logger.logDebug("sendCallRequest from: ", phoneNumber, " to: ", receiver);
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                phoneNumber,
                                receiver};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallAccept(common::PhoneNumber receiver)
{
    logger.logDebug("sendCallAccept from: ", phoneNumber, " to: ", receiver);
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                phoneNumber,
                                receiver};
    transport.sendMessage(msg.getMessage());

}

void BtsPort::sendCallDrop(common::PhoneNumber receiver)
{
    logger.logDebug("sendCallDrop from: ", phoneNumber, " to: ", receiver);
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                phoneNumber,
                                receiver};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallTalk(common::PhoneNumber receiver)
{
    logger.logDebug("sendCallTalk from: ", phoneNumber, " to: ", receiver);
    common::OutgoingMessage msg{common::MessageId::CallTalk,
                                phoneNumber,
                                receiver};
    // msg.writeText(text);
    transport.sendMessage(msg.getMessage());
}

} // namespace ue
    