#pragma once
#include "BaseState.hpp"
#include <vector>
#include <string>
#include "ViewSingleSmsState.hpp"

namespace ue
{
    class ViewSMSListState : public BaseState
    {
        public:
            ViewSMSListState(Context &context);
            void handleUIAction(std::optional<std::size_t> selectedIndex) override;
            void handleUIBack() override;
            void handleDisconnect() override;
            void handleSMS(common::PhoneNumber from, const std::string& message) override;

        private:
            void showSMSList();
            std::vector<std::string> smsList;
    };
} // namespace ue