#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include <functional>

namespace ue
{

    UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
        : logger(logger, "[USER-PORT]"),
          gui(gui),
          phoneNumber(phoneNumber)
    {
    }

    void UserPort::start(IUserEventsHandler &handler)
    {
        this->handler = &handler;
        gui.setTitle("Nokia " + to_string(phoneNumber));
    }

    void UserPort::stop()
    {
        handler = nullptr;
    }

    void UserPort::showNotConnected()
    {
        gui.showNotConnected();
    }

    void UserPort::showConnecting()
    {
        gui.showConnecting();
    }

    void UserPort::showConnected()
    {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        menu.addSelectionListItem("Compose SMS", "");
        menu.addSelectionListItem("View SMS", "");
        menu.addSelectionListItem("Dial", "");
    }

    void UserPort::showAlert(const std::string &header, const std::string &msg)
    {
        logger.logInfo("Alert: ", header);
        logger.logInfo("Message: ", msg);
        // TODO: Handle GUI
        gui.setComposeSmsModeCallback([this]
                                      { gui.setSmsComposeMode(); });
        gui.setViewSmsModeCallback([this]
                                   { gui.setViewTextMode(); });
    }

}