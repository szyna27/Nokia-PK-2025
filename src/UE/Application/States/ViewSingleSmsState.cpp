#include "ViewSingleSmsState.hpp"
#include "Context.hpp"
#include "States/ConnectedState.hpp"
#include "ViewSMSListState.hpp"
#include "NotConnectedState.hpp"
#include "UeGui/ISmsViewMode.hpp"

// TODO: Check state
namespace ue
{

ViewSingleSmsState::ViewSingleSmsState(Context &context, std::size_t selectedIndex)
    : BaseState(context, "ViewSingleSmsState"), selectedIndex(selectedIndex)
{
    // Get all SMS messages
    auto allSms = context.smsDB.getAllSMS();
    
    // Check if the selected index is valid
    if (selectedIndex < allSms.size()) {
        // Get the selected SMS
        auto& sms = allSms[selectedIndex];
        
        // Mark SMS as read
        sms.setRead(true);
        
        // Format the SMS message for display
        std::string displayText = "From: " + std::to_string(sms.getPhoneNumber().value) + "\n\n" + sms.getText();
        
        // Show the message in the text view mode
        auto& textMode = context.user.getViewSmsMode();
        textMode.setText(displayText);
        
        // Set up callbacks
        context.user.setHomeCallback([this, &context]() {
            context.user.showMainMenu();
        });
        
        context.user.setRejectCallback([this]() {
            handleUIBack();
        });
    } else {
        // Invalid selection, go back to the SMS list
        context.setState<ViewSMSListState>();
    }
}

void ViewSingleSmsState::handleUIBack()
{
    // Go back to SMS list view
    context.setState<ViewSMSListState>();
}

void ViewSingleSmsState::handleDisconnect()
{
    context.user.showNotConnected();
    context.setState<NotConnectedState>();
}

void ViewSingleSmsState::handleSMS(common::PhoneNumber from, const std::string &message)
{
    // Add the SMS to the database
    context.smsDB.addSMS(from, message);
    
    // Notify user about new message
    context.user.showNewSms(true);
    
    logger.logInfo("Received SMS in ViewSingleSmsState from: ", from, " message: ", message);
}

} // namespace ue