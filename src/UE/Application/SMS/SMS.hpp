#pragma once

#include "Messages/PhoneNumber.hpp"
#include <cstdint>

namespace ue
{
    class SMS
    {
        public:
            SMS(common::PhoneNumber phoneNumber, std::string text);
            ~SMS() = default;
            common::PhoneNumber getPhoneNumber() const;
            std::string getText() const;
            bool isRead() const;
            void setPhoneNumber(common::PhoneNumber phoneNumber);
            void setText(std::string text);
            void setRead(bool read);
            uint64_t getId() const;
            
        private:
            common::PhoneNumber phoneNumber;
            std::string text;
            bool read = false;
            uint64_t id;
            static uint64_t nextId;
    };
}