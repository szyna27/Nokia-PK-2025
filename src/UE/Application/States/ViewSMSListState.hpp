#pragma once
#include "BaseState.hpp"

namespace ue
{
    class ViewSMSListState : public BaseState
    {
        public:
            ViewSMSListState(Context &context);
            void handleUIAction() override;
            void handleUIBack() override;
            void handleDisconnect() override;
            void handleSMSRecieved(common::PhoneNumber from, const std::string& message) override;

        private:
            void showSMSList();
            std:vector<std::string> smsList;
    }
}; // namespace ue