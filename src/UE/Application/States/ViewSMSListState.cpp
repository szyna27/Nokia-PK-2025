#include "ViewSMSListState.hpp"
#include "Context.hpp"
#include "ViewSingleSmsState.hpp"
#include "NotConnectedState.hpp"
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
    
    auto allSms = context.smsDB.getAllSMS();
    
    if (allSms.empty()) {
        context.user.getListViewMode().addSelectionListItem("No messages", "");
        return;
    }
    
    for (const auto& sms : allSms) {
        std::string sender = "From: " + std::to_string(sms.getPhoneNumber().value);
        std::string preview = sms.getText().substr(0, std::min(sms.getText().length(), size_t(20)));
        if (sms.getText().length() > 20) {
            preview += "...";
        }
        
        context.user.getListViewMode().addSelectionListItem(sender, preview);
    }
    
    context.user.setHomeCallback([this]() { handleUIBack(); });
    context.user.setItemSelectedCallback([this]() { handleUIAction(std::nullopt); });
    
    context.user.showNewSms(false);
}

} // namespace ue