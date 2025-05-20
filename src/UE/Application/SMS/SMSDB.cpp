#include "SMSDB.hpp"
#include <stdexcept>

namespace ue
{
   void SMSDB::addSMS(common::PhoneNumber phoneNumber, const std::string& text)
   {
        SMS sms = SMS(phoneNumber, text);
        smsList.push_back(sms);
   } 
    void SMSDB::removeSMS(const SMS& sms)
    {
        for (auto it = smsList.begin(); it != smsList.end(); ++it)
        {
            if (it->getPhoneNumber() == sms.getPhoneNumber() && it->getText() == sms.getText())
            {
                smsList.erase(it);
                break;
            }
        } 
    }
    const std::vector<SMS>& SMSDB::getAllSMS() const
    {
        return smsList;
    }
    SMS SMSDB::getSMS(common::PhoneNumber phoneNumber, const std::string &text)
    {
        for (const auto& sms : smsList)
        {
            if (sms.getPhoneNumber() == phoneNumber && sms.getText() == text)
            {
                return sms;
            }
        }
        throw std::runtime_error("SMS not found");
    }

    SMS& SMSDB::getSmsAt(std::size_t index)
    {
        if (index >= smsList.size())
        {
            throw std::out_of_range("SMS index out of range");
        }
        return smsList[index];
    }

    void SMSDB::markSmsAsRead(common::PhoneNumber phoneNumber, const std::string &text)
    {
        for (auto& sms : smsList)
        {
            if (sms.getPhoneNumber() == phoneNumber && sms.getText() == text)
            {
                sms.setRead(true);
                return;
            }
        }
        throw std::runtime_error("SMS not found");
    }
}