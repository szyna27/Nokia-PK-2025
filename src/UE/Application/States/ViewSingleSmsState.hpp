#pragma once
#include "ConnectedState.hpp"
#include <cstdint>

namespace ue
{
    class ViewSingleSmsState : public ConnectedState
    {
    public:
        ViewSingleSmsState(Context &context, uint64_t smsId);
        void handleUIBack() override;
        void handleDisconnect() override;
        void handleSMS(common::PhoneNumber from, const std::string& message) override;

    private:
        uint64_t smsId;
        void refreshDisplay();
    };
} // namespace ue