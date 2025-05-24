#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMS/SMS.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using namespace ::testing;

class SMSTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{123};
    const std::string MESSAGE_TEXT{"Test message"};
};

TEST_F(SMSTestSuite, shallCreateSMSWithCorrectData)
{
    // Create an SMS object
    SMS sms(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Verify that the SMS has the expected properties
    EXPECT_EQ(sms.getPhoneNumber(), PHONE_NUMBER);
    EXPECT_EQ(sms.getText(), MESSAGE_TEXT);
    EXPECT_FALSE(sms.isRead());
}

TEST_F(SMSTestSuite, shallMarkAsReadCorrectly)
{
    // Create an SMS object
    SMS sms(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Initially should be unread
    EXPECT_FALSE(sms.isRead());
    
    // Mark as read
    sms.setRead(true);
    EXPECT_TRUE(sms.isRead());
    
    // Mark as unread
    sms.setRead(false);
    EXPECT_FALSE(sms.isRead());
}

TEST_F(SMSTestSuite, shallGenerateUniqueIds)
{
    // Create multiple SMS objects
    SMS sms1(PHONE_NUMBER, MESSAGE_TEXT);
    SMS sms2(PHONE_NUMBER, MESSAGE_TEXT);
    SMS sms3(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Each SMS should have a unique ID
    EXPECT_NE(sms1.getId(), sms2.getId());
    EXPECT_NE(sms2.getId(), sms3.getId());
    EXPECT_NE(sms1.getId(), sms3.getId());
}

TEST_F(SMSTestSuite, shallUpdateTextAndPhoneNumber)
{
    // Create an SMS object
    SMS sms(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Update text and phone number
    const common::PhoneNumber NEW_PHONE_NUMBER{111};
    const std::string NEW_MESSAGE_TEXT{"Updated message"};
    
    sms.setPhoneNumber(NEW_PHONE_NUMBER);
    sms.setText(NEW_MESSAGE_TEXT);
    
    // Verify updates
    EXPECT_EQ(sms.getPhoneNumber(), NEW_PHONE_NUMBER);
    EXPECT_EQ(sms.getText(), NEW_MESSAGE_TEXT);
}

} // namespace ue
