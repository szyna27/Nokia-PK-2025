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

void Application::handleCallTalk(common::PhoneNumber from)
{
    context.state->handleCallTalk(from);
}

void Application::handleCallAccept(common::PhoneNumber from)
{
    context.state->handleCallAccept(from);

}

void Application::handleCallDropped(common::PhoneNumber from)
{
    context.state->handleCallDropped(from);
}

void Application::handleSMS(common::PhoneNumber from, const std::string &message)
{
    context.state->handleSMS(from, message);
}

void Application::handleCallRequest(common::PhoneNumber from)
{
    context.state->handleCallRequest(from);
}

void Application::handleUIAction(std::optional<std::size_t> selectedIndex)
{
    context.state->handleUIAction(selectedIndex);

} 

void Application::handleUIBack()
{
    context.state->handleUIBack();
}
} // namespace ue

