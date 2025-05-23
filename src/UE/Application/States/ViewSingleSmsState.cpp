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
        auto smsOpt = context.smsDB.getSmsById(smsId);
        
        if (!smsOpt.has_value()) {
            throw std::runtime_error("SMS with ID " + std::to_string(smsId) + " not found");
        }

        auto sms = smsOpt.value();
        context.smsDB.markSmsAsReadById(smsId);
        
        context.user.setHomeCallback([this]() {
            this->context.user.showMainMenu();
            this->context.setState<ConnectedState>();
        });
        
        context.user.setRejectCallback([this]() {
            handleUIBack();
        });
        
        // Set up accept callback for toggling read status
        context.user.setAcceptCallback([this]() {
            try {
                if (this->context.smsDB.toggleReadStatusById(this->smsId)) {
                    this->refreshDisplay();
                }
            } catch (const std::exception& e) {
                this->logger.logError("Error toggling SMS read status: ", e.what());
                this->handleUIBack();
            }
        });
        
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
    context.setState<ViewSMSListState>();
}

void ViewSingleSmsState::handleDisconnect()
{
    context.user.showNotConnected();
    context.setState<NotConnectedState>();
}

void ViewSingleSmsState::handleSMS(common::PhoneNumber from, const std::string& message)
{
    context.smsDB.addSMS(from, message);
    context.user.showNewSms(true);   
    logger.logInfo("Received SMS in ViewSingleSmsState from: ", from, " message: ", message);
}

void ViewSingleSmsState::refreshDisplay()
{
    auto smsOpt = context.smsDB.getSmsById(smsId);
    if (!smsOpt.has_value()) {
        logger.logError("Failed to find SMS with ID: ", smsId);
        // If SMS not found, go back to SMS list
        handleUIBack();
        return;
    }
    
    const auto& sms = smsOpt.value();
    
    // Format the SMS message for display with read status
    std::string readStatus = sms.isRead() ? "[Read]" : "[NEW]";
    std::string displayText = readStatus + "\nFrom: " + std::to_string(sms.getPhoneNumber().value) 
                            + "\n\n" + sms.getText();
    
    auto& textMode = context.user.getViewSmsMode();
    textMode.setText(displayText);
}

} // namespace ue