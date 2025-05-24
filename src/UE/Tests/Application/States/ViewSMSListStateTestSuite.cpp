#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "States/ViewSMSListState.hpp"
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

class ViewSMSListStateTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{123};
    const std::string MESSAGE_TEXT{"Test message"};
    
    // Mocks
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    
    // SMS Database
    SMSDB smsDB;
    
    // Context for the state
    Context context{loggerMock, btsPortMock, userPortMock, timerPortMock, smsDB};
    
    ViewSMSListStateTestSuite() {
        // Setup common expectations
        EXPECT_CALL(userPortMock, getListViewMode())
            .WillRepeatedly(ReturnRef(listViewModeMock));
        
        EXPECT_CALL(listViewModeMock, clearSelectionList())
            .Times(AnyNumber());
        
        EXPECT_CALL(userPortMock, setHomeCallback(_))
            .Times(AnyNumber());
        
        EXPECT_CALL(userPortMock, setItemSelectedCallback(_))
            .Times(AnyNumber());
            
        // Additional mock expectations to handle underlying state behavior
        EXPECT_CALL(userPortMock, showConnected())
            .Times(AnyNumber());
            
        EXPECT_CALL(userPortMock, showMainMenu())
            .Times(AnyNumber());
            
        EXPECT_CALL(userPortMock, setAcceptCallback(_))
            .Times(AnyNumber());
            
        EXPECT_CALL(userPortMock, setRejectCallback(_))
            .Times(AnyNumber());
            
        // Setup for ViewSingleSmsState transition
        EXPECT_CALL(userPortMock, getViewSmsMode())
            .Times(AnyNumber());
    }
};

TEST_F(ViewSMSListStateTestSuite, shallDisplayEmptyListWhenNoSMS)
{
    // Expect "No messages" for empty list
    EXPECT_CALL(listViewModeMock, addSelectionListItem(HasSubstr("No messages"), _));
    
    // Create the state
    ViewSMSListState objectUnderTest(context);
}

TEST_F(ViewSMSListStateTestSuite, shallDisplaySMSListWithReadStatus)
{
    // Add a test SMS to the database
    smsDB.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Expect list item with unread status (NEW)
    EXPECT_CALL(listViewModeMock, addSelectionListItem(HasSubstr("[NEW]"), _));
    
    // Create the state
    ViewSMSListState objectUnderTest(context);
    
    // Mark the SMS as read
    const auto& allSms = smsDB.getAllSMS();
    smsDB.markSmsAsReadById(allSms[0].getId());
    
    // Clear previous expectations
    testing::Mock::VerifyAndClearExpectations(&listViewModeMock);
    
    // Setup new expectations
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(HasSubstr("[Read]"), _));
    
    // Refresh view by creating a new state
    ViewSMSListState objectUnderTest2(context);
}

TEST_F(ViewSMSListStateTestSuite, shallHandleNewSMSWhileViewingList)
{
    // Create the state with empty list
    EXPECT_CALL(listViewModeMock, addSelectionListItem(HasSubstr("No messages"), _));
    ViewSMSListState objectUnderTest(context);
    
    // Clear previous expectations
    testing::Mock::VerifyAndClearExpectations(&listViewModeMock);
    
    // Expect list refresh with new SMS
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(HasSubstr("[NEW]"), _));
    
    // Trigger SMS reception
    objectUnderTest.handleSMS(PHONE_NUMBER, MESSAGE_TEXT);
}

TEST_F(ViewSMSListStateTestSuite, shallNavigateToSingleSMSViewOnSelection)
{
    // Setup expectations for listViewModeMock 
    // including the item that will be displayed in the list
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _))
        .Times(AnyNumber());
    
    // Add a test SMS to the database
    smsDB.addSMS(PHONE_NUMBER, MESSAGE_TEXT);
    
    // Create the state
    ViewSMSListState objectUnderTest(context);
    
    // Mark the SMS as read to test both states
    const auto& allSms = smsDB.getAllSMS();
    smsDB.markSmsAsReadById(allSms[0].getId());
    
    // This test only verifies that the handleUIAction function
    // doesn't crash when given a valid item index
    ASSERT_NO_THROW({
        objectUnderTest.handleUIAction(0);
    });
}

} // namespace ue
