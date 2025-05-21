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
        auto uiIndex = selectedIndex.value();
        
        // Check if we have this UI index mapped to an SMS ID
        if (indexToSmsIdMap.find(uiIndex) != indexToSmsIdMap.end()) {
            // Get the SMS ID corresponding to the UI index
            uint64_t smsId = indexToSmsIdMap[uiIndex];
            context.setState<ViewSingleSmsState>(smsId);
        } else {
            logger.logError("Invalid UI index: ", uiIndex);
        }
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
    
    const auto& allSms = context.smsDB.getAllSMS();
    
    // Clear the previous mapping
    indexToSmsIdMap.clear();
    
    if (allSms.empty()) {
        listViewMode.addSelectionListItem("No messages", "");
    } else {
        size_t uiIndex = 0;
        for (const auto& sms : allSms) {
            std::string readStatus = sms.isRead() ? "[Read] " : "[NEW] ";
            std::string sender = readStatus + "From: " + std::to_string(sms.getPhoneNumber().value);
            std::string preview = sms.getText().substr(0, std::min(sms.getText().length(), size_t(20)));
            if (sms.getText().length() > 20) {
                preview += "...";
            }
            
            // Store mapping between UI index and SMS ID
            indexToSmsIdMap[uiIndex] = sms.getId();
            
            listViewMode.addSelectionListItem(sender, preview);
            uiIndex++;
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