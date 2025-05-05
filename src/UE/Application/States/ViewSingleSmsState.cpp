#include "ViewSingleSmsState.hpp"

namespace ue
{

ViewSingleSmsState::ViewSingleSmsState(Context &context, std::size_t selectedIndex)
    : BaseState(context), selectedIndex(selectedIndex)
{
    // Constructor implementation
}

void ViewSingleSmsState::handleUIBack()
{
    // Implementation for handling UI back action
}

void ViewSingleSmsState::handleDisconnect()
{
    // Implementation for handling disconnect
}

void ViewSingleSmsState::handleSMSRecieved(common::PhoneNumber from, const std::string &message)
{
    // Implementation for handling received SMS
}

} // namespace ue