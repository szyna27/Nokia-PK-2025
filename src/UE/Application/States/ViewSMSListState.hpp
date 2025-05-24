#pragma once
#include "ConnectedState.hpp"
#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include "ViewSingleSmsState.hpp"

namespace ue
{
    class ViewSMSListState : public ConnectedState
    {
        public:
            ViewSMSListState(Context &context);
            void handleUIAction(std::optional<std::size_t> selectedIndex) override;
            void handleUIBack() override;
            void handleDisconnect() override;
            void handleSMS(common::PhoneNumber from, const std::string& message) override;

        private:
            void showSMSList();
            
            // Map to associate UI index with SMS ID
            std::map<std::size_t, uint64_t> indexToSmsIdMap;
    };
} // namespace ue