#include "Application.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         SMSDB &smsDB)
    : context{iLogger, bts, user, timer, smsDB},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>(true);
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleDisconnect()
{
    context.state->handleDisconnect(); 
}

void Application::handleCallMessage(common::MessageId msgId)
{
    context.state->handleCallMessage(msgId);
}

void Application::handleCallAccept(common::MessageId msgId)
{
    context.state->handleCallAccept(msgId);

}

void Application::handleCallDropped(common::MessageId msgId)
{
    context.state->handleCallDropped(msgId);
}

void Application::handleSMS(common::PhoneNumber from, const std::string &message)
{
    context.state->handleSMS(from, message);
}

} // namespace ue


