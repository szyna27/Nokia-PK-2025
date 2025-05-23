#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMS/SMSDB.hpp"
#include "SMS/SMS.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Context.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "States/ViewSingleSmsState.hpp"

namespace ue
{
using namespace ::testing;

class SMSRegressionTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{123};
    const std::string MESSAGE_TEXT{"Test message"};
    uint64_t smsId;
    
    // Mocks
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITextModeMock> textModeMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    
    // SMS Database with a test message
    SMSDB smsDB;
    
    // Context for the state
    Context context{loggerMock, btsPortMock, userPortMock, timerPortMock, smsDB};
    
    SMSRegressionTestSuite() {
        // Add a test SMS to the database
        smsDB.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
        
        // Get the ID of the added SMS
        const auto& allSms = smsDB.getAllSMS();
        smsId = allSms[0].getId();
    }
};

TEST_F(SMSRegressionTestSuite, shallHandleRemovedSMSGracefully)
{
    // Setup mocks
    EXPECT_CALL(userPortMock, getViewSmsMode())
        .WillRepeatedly(ReturnRef(textModeMock));
    
    EXPECT_CALL(userPortMock, getListViewMode())
        .WillRepeatedly(ReturnRef(listViewModeMock));
    
    EXPECT_CALL(listViewModeMock, clearSelectionList())
        .Times(AnyNumber());
    
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setHomeCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setRejectCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setAcceptCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setItemSelectedCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(textModeMock, setText(_))
        .Times(AnyNumber());
    
    // Create state with a valid SMS ID
    ViewSingleSmsState objectUnderTest(context, smsId);
    
    // Now remove the SMS
    smsDB.removeSMS(smsDB.getSmsById(smsId).value());
    
    // Create a function pointer to simulate pressing the accept button
    // This would have caused a segmentation fault before our fix
    std::function<void()> acceptCallback;
    
    // Get the accept callback
    EXPECT_CALL(userPortMock, setAcceptCallback(_))
        .WillOnce(SaveArg<0>(&acceptCallback));
    
    // Create a new state with the same ID (which no longer exists)
    ViewSingleSmsState objectUnderTest2(context, smsId);
    
    // Call the accept callback - this should handle the missing SMS gracefully
    if (acceptCallback) {
        acceptCallback();
    }
}

TEST_F(SMSRegressionTestSuite, shallHandleConcurrentSMSModification)
{
    // Setup mocks
    EXPECT_CALL(userPortMock, getViewSmsMode())
        .WillRepeatedly(ReturnRef(textModeMock));
    
    EXPECT_CALL(textModeMock, setText(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setHomeCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setRejectCallback(_))
        .Times(AnyNumber());
    
    EXPECT_CALL(userPortMock, setAcceptCallback(_))
        .Times(AnyNumber());
    
    // Create state with a valid SMS ID
    ViewSingleSmsState objectUnderTest(context, smsId);
    
    // Simulate receiving a new SMS while viewing an existing one
    // This should not cause any issues
    objectUnderTest.handleSMS(common::PhoneNumber{111}, "New message while viewing");
    
    // Verify a new SMS was added
    EXPECT_EQ(smsDB.getAllSMS().size(), 2);
}

} // namespace ue
