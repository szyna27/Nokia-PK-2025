#include "ViewSMSListState.hpp"
#include "Context.hpp"
#include "ViewSingleSmsState.hpp"
#include "NotConnectedState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/IListViewMode.hpp"
#include <optional>

namespace ue
{

ViewSMSListState::ViewSMSListState(Context &context)
    : BaseState(context, "ViewSMSListState")
{
    showSMSList();
}

void ViewSMSListState::handleUIAction(std::optional<std::size_t> selectedIndex)
{
    if (selectedIndex.has_value()) {
        auto selected = selectedIndex.value();
        context.setState<ViewSingleSmsState>(selected);
    }
}

void ViewSMSListState::handleUIBack()
{
    context.user.showMainMenu();
    context.setState<ConnectedState>();
}

void ViewSMSListState::handleDisconnect()
{
    context.user.showNotConnected();
    context.setState<NotConnectedState>();
}

void ViewSMSListState::handleSMS(common::PhoneNumber from, const std::string &message)
{
    context.smsDB.addSMS(from, message);
    showSMSList();
    logger.logInfo("Received SMS in ViewSMSListState from: ", from, " message: ", message);
}

void ViewSMSListState::showSMSList()
{
    auto& listViewMode = context.user.getListViewMode();
    listViewMode.clearSelectionList();
    
    auto allSms = context.smsDB.getAllSMS();
    
    if (allSms.empty()) {
        listViewMode.addSelectionListItem("No messages", "");
    } else {
        for (const auto& sms : allSms) {
            std::string readStatus = sms.isRead() ? "✓ " : "● ";
            std::string sender = readStatus + "From: " + std::to_string(sms.getPhoneNumber().value);
            std::string preview = sms.getText().substr(0, std::min(sms.getText().length(), size_t(20)));
            if (sms.getText().length() > 20) {
                preview += "...";
            }
            
            listViewMode.addSelectionListItem(sender, preview);
        }
    }
    
    // Set callbacks for navigation
    context.user.setHomeCallback([this]() { 
        context.user.showMainMenu();
        context.setState<ConnectedState>();
    });
    
    context.user.setItemSelectedCallback([this]() { 
        auto indexPair = context.user.getListViewMode().getCurrentItemIndex();
        if (indexPair.first) {
            // If a valid selection exists (first == true)
            handleUIAction(indexPair.second);
        }
    });
}

} // namespace ue