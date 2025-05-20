#include "ViewSingleSmsState.hpp"
#include "Context.hpp"
#include "States/ConnectedState.hpp"
#include "ViewSMSListState.hpp"
#include "NotConnectedState.hpp"
#include "UeGui/ITextMode.hpp"

namespace ue
{

ViewSingleSmsState::ViewSingleSmsState(Context &context, std::size_t selectedIndex)
    : BaseState(context, "ViewSingleSmsState"), selectedIndex(selectedIndex)
{
    try {
        // Get a const reference to all SMS messages
        const auto& allSms = context.smsDB.getAllSMS();
        
        // Check if the selected index is valid
        if (selectedIndex >= allSms.size()) {
            throw std::out_of_range("SMS index out of range");
        }
        
        // Get the selected SMS (only as a const reference, to read its data)
        const auto& sms = allSms[selectedIndex];
        
        // Mark SMS as read using the proper phone number and text
        context.smsDB.markSmsAsRead(sms.getPhoneNumber(), sms.getText());
        
        // Format the SMS message for display
        std::string displayText = "From: " + std::to_string(sms.getPhoneNumber().value) + "\n\n" + sms.getText();
        
        // Show the message in the text view mode
        auto& textMode = context.user.getViewSmsMode();
        textMode.setText(displayText);
        
        // Set up callbacks for navigation
        context.user.setHomeCallback([this, &context]() {
            // When home button is pressed, go to main menu
            context.user.showMainMenu();
            context.setState<ConnectedState>();
        });
        
        context.user.setRejectCallback([this]() {
            // When reject/back button is pressed, go back to SMS list
            handleUIBack();
        });
        
        logger.logInfo("Viewing SMS from: ", sms.getPhoneNumber(), ", marked as read");
    }
    catch (const std::out_of_range& e) {
        logger.logError("Invalid SMS index: ", selectedIndex);
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

void ViewSingleSmsState::handleSMS(common::PhoneNumber from, const std::string& message)
{
    // Add the SMS to the database
    context.smsDB.addSMS(from, message);
    
    // Show notification
    context.user.showNewSms(true);
    
    logger.logInfo("Received SMS in ViewSingleSmsState from: ", from, " message: ", message);
}

} // namespace ue