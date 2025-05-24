#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "States/ViewSingleSmsState.hpp"
#include "States/BaseState.hpp"
#include "Context.hpp"
#include "SMS/SMSDB.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"

namespace ue
{
using namespace ::testing;

class ViewSingleSmsStateTestSuite : public Test
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
    
    ViewSingleSmsStateTestSuite() {
        // Add a test SMS to the database
        smsDB.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
        
        // Get the ID of the added SMS
        const auto& allSms = smsDB.getAllSMS();
        smsId = allSms[0].getId();
        
        // Setup common expectations
        EXPECT_CALL(userPortMock, getViewSmsMode())
            .WillRepeatedly(ReturnRef(textModeMock));
    }
};

TEST_F(ViewSingleSmsStateTestSuite, shallMarkSMSAsReadWhenViewed)
{
    // Setup expectations for callback functions
    EXPECT_CALL(userPortMock, setHomeCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    
    // Setup expectations for text display
    EXPECT_CALL(textModeMock, setText(HasSubstr(MESSAGE_TEXT)));
    
    // Create the state - this should mark the SMS as read
    ViewSingleSmsState objectUnderTest(context, smsId);
    
    // Verify that the SMS has been marked as read
    auto smsOpt = smsDB.getSmsById(smsId);
    ASSERT_TRUE(smsOpt.has_value());
    EXPECT_TRUE(smsOpt.value().isRead());
}

TEST_F(ViewSingleSmsStateTestSuite, shallToggleReadStatusWhenAcceptPressed)
{
    // Capture the accept callback to test it
    std::function<void()> acceptCallback;
    
    // Setup expectations
    EXPECT_CALL(userPortMock, setHomeCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    EXPECT_CALL(userPortMock, setAcceptCallback(_))
        .WillOnce(SaveArg<0>(&acceptCallback));
    EXPECT_CALL(textModeMock, setText(_))
        .Times(AtLeast(1));
    
    // Create the state - this should mark the SMS as read
    ViewSingleSmsState objectUnderTest(context, smsId);
    
    // Initially the SMS should be read
    auto smsOpt1 = smsDB.getSmsById(smsId);
    ASSERT_TRUE(smsOpt1.has_value());
    EXPECT_TRUE(smsOpt1.value().isRead());
    
    // When accept is pressed, it should toggle the read status
    // Setup expectation for the second setText call
    EXPECT_CALL(textModeMock, setText(HasSubstr("[NEW]")));
    
    // Simulate pressing accept
    acceptCallback();
    
    // Now the SMS should be unread
    auto smsOpt2 = smsDB.getSmsById(smsId);
    ASSERT_TRUE(smsOpt2.has_value());
    EXPECT_FALSE(smsOpt2.value().isRead());
}

TEST_F(ViewSingleSmsStateTestSuite, shallHandleInvalidSMSIdGracefully)
{
    // When an invalid SMS ID is provided, the ViewSingleSmsState 
    // should transition back to ViewSMSListState
    
    // Setup expectations for the transition to ViewSMSListState
    // We need to handle the list view mode being requested
    EXPECT_CALL(userPortMock, getListViewMode())
        .WillOnce(ReturnRef(listViewModeMock));
        
    EXPECT_CALL(listViewModeMock, clearSelectionList())
        .Times(AnyNumber());
        
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _))
        .Times(AnyNumber());
        
    EXPECT_CALL(userPortMock, setHomeCallback(_))
        .Times(AnyNumber());
        
    EXPECT_CALL(userPortMock, setItemSelectedCallback(_))
        .Times(AnyNumber());
    
    // Create the state with an invalid SMS ID
    // This should internally transition to ViewSMSListState
    ViewSingleSmsState objectUnderTest(context, 999);
}

} // namespace ue
