#pragma once
#include <vector>
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
            std::vector<SMS> getAllSMS() const;
            SMS getSMS(common::PhoneNumber phoneNumber, const std::string& text);
        
        private:
            std::vector<SMS> smsList;
    };
}