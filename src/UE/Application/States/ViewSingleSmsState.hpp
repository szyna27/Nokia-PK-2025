#pragma once
#include "BaseState.hpp"

namespace ue
{
    class ViewSingleSmsState : public BaseState
    {
    public:
        ViewSingleSmsState(Context &context, std::size_t selectedIndex);
        void handleUIBack() override;
        void handleDisconnect() override;
        void handleSMSRecieved(common::PhoneNumber from, const std::string& message) override;

    private:
        std::size_t selectedIndex;
    };
} // namespace ue