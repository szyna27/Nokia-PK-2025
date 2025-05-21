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

    std::optional<SMS> SMSDB::getSmsById(uint64_t id) const
    {
        // Add defensive check for empty list
        if (smsList.empty()) {
            return std::nullopt;
        }
        
        for (const auto& sms : smsList)
        {
            if (sms.getId() == id)
            {
                return sms;
            }
        }
        return std::nullopt;
    }
    
    void SMSDB::markSmsAsReadById(uint64_t id)
    {
        // Check for empty list first
        if (smsList.empty()) {
            return;
        }
        
        for (auto& sms : smsList)
        {
            if (sms.getId() == id)
            {
                sms.setRead(true);
                return;
            }
        }
    }
    
    bool SMSDB::toggleReadStatusById(uint64_t id)
    {
        // Check for empty list first
        if (smsList.empty()) {
            return false;
        }
        
        for (auto& sms : smsList)
        {
            if (sms.getId() == id)
            {
                bool newStatus = !sms.isRead();
                sms.setRead(newStatus);
                return true;
            }
        }
        return false;
    }
}