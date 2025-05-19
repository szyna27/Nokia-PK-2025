#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{
    class SMS
    {
        public:
            SMS(common::PhoneNumber phoneNumber, std::string text);
            ~SMS() = default;
            common::PhoneNumber getPhoneNumber() const;
            std::string getText() const;
            bool isRead() const ;
            void setPhoneNumber(common::PhoneNumber phoneNumber);
            void setText(std::string text);
            void setRead(bool read);

        private:
            common::PhoneNumber phoneNumber;
            std::string text;
            bool read = false;
    };
}