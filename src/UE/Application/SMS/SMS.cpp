#include "SMS.hpp"

namespace ue
{
    SMS::SMS(common::PhoneNumber phoneNumber, std::string text)
        : phoneNumber(phoneNumber), text(text)
    {
    }

    common::PhoneNumber SMS::getPhoneNumber() const
    {
        return phoneNumber;
    }

    std::string SMS::getText() const
    {
        return text;
    }

    bool SMS::isRead() const 
    {
        return read;
    }

    void SMS::setPhoneNumber(common::PhoneNumber phoneNumber)
    {
        this->phoneNumber = phoneNumber;
    }

    void SMS::setText(std::string text)
    {
        this->text = text;
    }

    void SMS::setRead(bool read)
    {
        this->read = read;
    }
}