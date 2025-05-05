#include "ViewSMSListState.hpp"

namespace ue
{

ViewSMSListState::ViewSMSListState(Context &context)
    : BaseState(context)
{
    // Constructor implementation
}

void ViewSMSListState::handleUIAction()
{
    // Implementation for handling UI action
}

void ViewSMSListState::handleUIBack()
{
    // Implementation for handling UI back action
}

void ViewSMSListState::handleDisconnect()
{
    // Implementation for handling disconnect
}

void ViewSMSListState::handleSMSRecieved(common::PhoneNumber from, const std::string &message)
{
    // Implementation for handling received SMS
}

void ViewSMSListState::showSMSList()
{
    // Implementation for showing SMS list
}

} // namespace ue