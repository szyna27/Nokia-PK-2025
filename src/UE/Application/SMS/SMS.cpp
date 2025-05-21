#include "SMS.hpp"

namespace ue
{
    // Initialize static counter
    uint64_t SMS::nextId = 0;
    
    SMS::SMS(common::PhoneNumber phoneNumber, std::string text)
        : phoneNumber(phoneNumber), text(text), id(nextId++)
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

    uint64_t SMS::getId() const
    {
        return id;
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