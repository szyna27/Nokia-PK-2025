#include "ViewSingleSmsState.hpp"
#include "Context.hpp"
#include "States/ConnectedState.hpp"
#include "ViewSMSListState.hpp"
#include "NotConnectedState.hpp"
#include "UeGui/ITextMode.hpp"

namespace ue
{

ViewSingleSmsState::ViewSingleSmsState(Context &context, uint64_t smsId)
    : BaseState(context, "ViewSingleSmsState"), smsId(smsId)
{
    try {
        // Get the SMS by ID
        auto smsOpt = context.smsDB.getSmsById(smsId);
        
        if (!smsOpt.has_value()) {
            throw std::runtime_error("SMS with ID " + std::to_string(smsId) + " not found");
        }
        
        // Store a copy of the SMS, not just a reference that might be invalidated
        auto sms = smsOpt.value();
        
        // Mark the SMS as read - do this before refreshing the display
        context.smsDB.markSmsAsReadById(smsId);
        
        // Setup callbacks first to avoid segfaults if refreshDisplay fails
        // Set up callbacks for navigation
        context.user.setHomeCallback([this]() {
            // When home button is pressed, go to main menu
            this->context.user.showMainMenu();
            this->context.setState<ConnectedState>();
        });
        
        context.user.setRejectCallback([this]() {
            // When reject/back button is pressed, go back to SMS list
            handleUIBack();
        });
        
        // Set up accept callback for toggling read status
        context.user.setAcceptCallback([this]() {
            try {
                // When accept is pressed, toggle read status
                if (this->context.smsDB.toggleReadStatusById(this->smsId)) {
                    // Refresh the display to show updated status
                    this->refreshDisplay();
                }
            } catch (const std::exception& e) {
                this->logger.logError("Error toggling SMS read status: ", e.what());
                this->handleUIBack();
            }
        });
        
        // Display the SMS after all callbacks are set up
        refreshDisplay();
        
        logger.logInfo("Viewing SMS from: ", sms.getPhoneNumber(), ", marked as read");
    }
    catch (const std::exception& e) {
        logger.logError("Error displaying SMS: ", e.what());
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

void ViewSingleSmsState::refreshDisplay()
{
    auto smsOpt = context.smsDB.getSmsById(smsId);
    if (!smsOpt.has_value()) {
        logger.logError("Failed to find SMS with ID: ", smsId);
        // If SMS not found, go back to SMS list to avoid segfault
        handleUIBack();
        return;
    }
    
    const auto& sms = smsOpt.value();
    
    // Format the SMS message for display with read status
    std::string readStatus = sms.isRead() ? "[Read]" : "[NEW]";
    std::string displayText = readStatus + "\nFrom: " + std::to_string(sms.getPhoneNumber().value) 
                            + "\n\n" + sms.getText() 
                            + "\n\n[Accept] to toggle read/unread status";
    
    // Get a reference to the text mode and set the text
    // Store it in a local variable to ensure it's alive during the operation
    auto& textMode = context.user.getViewSmsMode();
    textMode.setText(displayText);
}

} // namespace ue