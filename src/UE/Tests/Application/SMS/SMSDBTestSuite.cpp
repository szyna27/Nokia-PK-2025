#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMS/SMSDB.hpp"
#include "SMS/SMS.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using namespace ::testing;

class SMSDBTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{123};
    const std::string MESSAGE_TEXT{"Test message"};
    SMSDB objectUnderTest;
};

TEST_F(SMSDBTestSuite, shallAddAndRetrieveSMS)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Get all SMS and check if there is one
    const auto& allSms = objectUnderTest.getAllSMS();
    ASSERT_EQ(allSms.size(), 1);
    
    // Verify SMS content
    EXPECT_EQ(allSms[0].getPhoneNumber(), PHONE_NUMBER);
    EXPECT_EQ(allSms[0].getText(), MESSAGE_TEXT);
    EXPECT_FALSE(allSms[0].isRead());
}

TEST_F(SMSDBTestSuite, shallRetrieveSMSByIndex)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Retrieve by index
    auto& sms = objectUnderTest.getSmsAt(0);
    
    // Verify SMS content
    EXPECT_EQ(sms.getPhoneNumber(), PHONE_NUMBER);
    EXPECT_EQ(sms.getText(), MESSAGE_TEXT);
}

TEST_F(SMSDBTestSuite, shallRetrieveSMSById)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Get the ID of the added SMS
    const auto& allSms = objectUnderTest.getAllSMS();
    uint64_t smsId = allSms[0].getId();
    
    // Retrieve by ID
    auto smsOpt = objectUnderTest.getSmsById(smsId);
    
    // Verify SMS exists and content is correct
    ASSERT_TRUE(smsOpt.has_value());
    EXPECT_EQ(smsOpt.value().getPhoneNumber(), PHONE_NUMBER);
    EXPECT_EQ(smsOpt.value().getText(), MESSAGE_TEXT);
}

TEST_F(SMSDBTestSuite, shallMarkSMSAsReadByPhoneNumberAndText)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Mark as read
    objectUnderTest.markSmsAsRead(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Verify it's marked as read
    const auto& allSms = objectUnderTest.getAllSMS();
    EXPECT_TRUE(allSms[0].isRead());
}

TEST_F(SMSDBTestSuite, shallMarkSMSAsReadById)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Get the ID of the added SMS
    const auto& allSms = objectUnderTest.getAllSMS();
    uint64_t smsId = allSms[0].getId();
    
    // Mark as read by ID
    objectUnderTest.markSmsAsReadById(smsId);
    
    // Verify it's marked as read
    auto smsOpt = objectUnderTest.getSmsById(smsId);
    ASSERT_TRUE(smsOpt.has_value());
    EXPECT_TRUE(smsOpt.value().isRead());
}

TEST_F(SMSDBTestSuite, shallToggleReadStatusById)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Get the ID of the added SMS
    const auto& allSms = objectUnderTest.getAllSMS();
    uint64_t smsId = allSms[0].getId();
    
    // Toggle read status (initially unread -> read)
    bool toggleResult1 = objectUnderTest.toggleReadStatusById(smsId);
    EXPECT_TRUE(toggleResult1);
    
    auto smsOpt1 = objectUnderTest.getSmsById(smsId);
    ASSERT_TRUE(smsOpt1.has_value());
    EXPECT_TRUE(smsOpt1.value().isRead());
    
    // Toggle again (read -> unread)
    bool toggleResult2 = objectUnderTest.toggleReadStatusById(smsId);
    EXPECT_TRUE(toggleResult2);
    
    auto smsOpt2 = objectUnderTest.getSmsById(smsId);
    ASSERT_TRUE(smsOpt2.has_value());
    EXPECT_FALSE(smsOpt2.value().isRead());
}

TEST_F(SMSDBTestSuite, shallHandleNonExistentSMSGracefully)
{
    // Try to get non-existent SMS by ID
    auto smsOpt = objectUnderTest.getSmsById(999);
    EXPECT_FALSE(smsOpt.has_value());
    
    // Try to toggle read status of non-existent SMS
    bool toggleResult = objectUnderTest.toggleReadStatusById(999);
    EXPECT_FALSE(toggleResult);
    
    // Mark as read by ID should not crash
    objectUnderTest.markSmsAsReadById(999);
}

TEST_F(SMSDBTestSuite, shallRemoveSMS)
{
    // Add SMS
    objectUnderTest.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Get SMS
    SMS sms = objectUnderTest.getSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Remove it
    objectUnderTest.removeSMS(sms);
    
    // Verify it's removed
    const auto& allSms = objectUnderTest.getAllSMS();
    EXPECT_EQ(allSms.size(), 0);
}

} // namespace ue
