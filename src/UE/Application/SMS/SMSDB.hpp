#pragma once
#include <vector>
#include <optional>
#include "SMS.hpp"

namespace ue
{
    class SMSDB
    {
        public:
            SMSDB() = default;
            ~SMSDB() = default;
            void addSMS(common::PhoneNumber phoneNumber, const std::string& text);
            void removeSMS(const SMS& sms);
            const std::vector<SMS>& getAllSMS() const;
            SMS getSMS(common::PhoneNumber phoneNumber, const std::string& text);
            SMS& getSmsAt(std::size_t index);
            void markSmsAsRead(common::PhoneNumber phoneNumber, const std::string& text);
            
            // ID-based methods
            std::optional<SMS> getSmsById(uint64_t id) const;
            void markSmsAsReadById(uint64_t id);
            bool toggleReadStatusById(uint64_t id);
        
        private:
            std::vector<SMS> smsList;
    };
}