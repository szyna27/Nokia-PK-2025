#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

    Application::Application(common::PhoneNumber phoneNumber,
                             common::ILogger &iLogger,
                             IBtsPort &bts,
                             IUserPort &user,
                             ITimerPort &timer)
        : context{iLogger, bts, user, timer},
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

    void Application::handleUIEnding()
    {
        context.state->handleUIEnding();
    }

    void Application::handleUICalling(std::optional<std::size_t> selectedIndex)
    {
        context.state->handleUICalling(selectedIndex);
    }

    void Application::handleCallAccepted(common::PhoneNumber recipient)
    {
        context.state->handleCallAccepted(recipient);
    }

    void Application::handleUnknownRecipient(common::PhoneNumber recipient)
    {
        context.state->handleUnknownRecipient(recipient);
    }

    void Application::handleCallDropped()
    {
        context.state->handleCallDropped();
    }

}
