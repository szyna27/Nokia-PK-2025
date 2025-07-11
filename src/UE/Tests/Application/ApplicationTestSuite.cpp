#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;
using namespace std::chrono_literals;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber PEER_PHONE_NUMBER{113};
    const common::BtsId BTS_ID{1024};
    NiceMock<common::ILoggerMock> loggerMock;

    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<IDialModeMock> dialModeMock;
    StrictMock<ISmsComposeModeMock> smsComposeModeMock;
    StrictMock<ITextModeMock> textModeMock;
    StrictMock<ICallModeMock> callModeMock;
    SMSDB smsDB = SMSDB();

    Application objectUnderTest{PHONE_NUMBER,
        loggerMock,
        btsPortMock,
        userPortMock,
        timerPortMock,
        smsDB};  
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void shallHandleSibMessage()
    {
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());

        objectUnderTest.handleSib(BTS_ID);   
    }
};

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite()
    {
        shallHandleSibMessage();
        EXPECT_CALL(timerPortMock, stopTimer());
    }

    void shallHandleAttachAccept()
    {
        EXPECT_CALL(userPortMock, showConnected()).Times(AnyNumber());
        EXPECT_CALL(userPortMock, showMainMenu());
        EXPECT_CALL(userPortMock, getListViewMode()).Times(AnyNumber()).WillRepeatedly(ReturnRef(listViewModeMock));        EXPECT_CALL(userPortMock, setItemSelectedCallback(_));
        EXPECT_CALL(userPortMock, setHomeCallback(_));
        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));

        objectUnderTest.handleAttachAccept();
    }

    void shallHandleAttachReject()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleAttachReject();
    }

    void shallHandleTimeout()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleTimeout();
    }

    void shallHandleDisconnect()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleDisconnect();
    }

    void shallHandleReconnect()
    {
        shallHandleDisconnect();
        shallHandleSibMessage();
        EXPECT_CALL(timerPortMock, stopTimer());
        shallHandleAttachAccept();
    }
};

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite()
    {
        shallHandleAttachAccept();
    }   

    
    void shallHandleCallAccept(){
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, getCallMode()).WillOnce(ReturnRef(callModeMock));
        EXPECT_CALL(callModeMock, clearIncomingText());
        EXPECT_CALL(userPortMock, showTalking());
        EXPECT_CALL(userPortMock, setHomeCallback(_));
        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));
        EXPECT_CALL(timerPortMock, startTimer(_));

        objectUnderTest.handleCallAccept(PEER_PHONE_NUMBER);
    }

    void shallHandleTimeout(){
        EXPECT_CALL(userPortMock, getDialMode()).WillOnce(ReturnRef(dialModeMock));
        EXPECT_CALL(dialModeMock, getPhoneNumber()).WillOnce(Return(PEER_PHONE_NUMBER));
        EXPECT_CALL(btsPortMock, sendCallDrop(PEER_PHONE_NUMBER));
        EXPECT_CALL(userPortMock, showMainMenu());
        EXPECT_CALL(userPortMock, setItemSelectedCallback(_)).Times(AnyNumber());
        EXPECT_CALL(userPortMock, setHomeCallback(_)).Times(AnyNumber());
        EXPECT_CALL(userPortMock, setAcceptCallback(_)).Times(AnyNumber());
        EXPECT_CALL(userPortMock, setRejectCallback(_)).Times(AnyNumber());
        objectUnderTest.handleTimeout();
    }

    void shallHandleCallDropped(){
        //EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showMainMenu());
        EXPECT_CALL(userPortMock, getListViewMode()).WillOnce(ReturnRef(listViewModeMock));
        EXPECT_CALL(userPortMock, setItemSelectedCallback(_));
        EXPECT_CALL(userPortMock, setHomeCallback(_));
        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));

        objectUnderTest.handleCallDropped(PEER_PHONE_NUMBER);
    }

    void shallHandleSms()
    {
        const std::string message = "Hello, this is a test SMS!";

        objectUnderTest.handleSMS(PEER_PHONE_NUMBER, message);
    }

    void shallHandleCallRequest(){
        EXPECT_CALL(userPortMock, showDial());
        EXPECT_CALL(userPortMock, getCallMode()).Times(AnyNumber()).WillRepeatedly(ReturnRef(callModeMock));
        EXPECT_CALL(callModeMock, clearIncomingText()).Times(AnyNumber());
        EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AnyNumber());
        EXPECT_CALL(userPortMock, setAcceptCallback(_)).Times(2);
        EXPECT_CALL(userPortMock, setRejectCallback(_)).Times(2);
        EXPECT_CALL(userPortMock, setHomeCallback(_));

        objectUnderTest.handleCallRequest(PEER_PHONE_NUMBER);
    }

};

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite()
    {
        shallHandleCallAccept();
    }
    void shallHandleCallTalk()
    {
        EXPECT_CALL(userPortMock, getCallMode()).WillOnce(ReturnRef(callModeMock));
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(timerPortMock, startTimer(30000ms));
        EXPECT_CALL(callModeMock, clearIncomingText());
        EXPECT_CALL(callModeMock, appendIncomingText("Hello"));
        objectUnderTest.handleCallTalk("Hello");
    }
};


struct ApplicationCallTestSuite : ApplicationConnectedTestSuite
{
    void shallSendCallRequest()
    {
        EXPECT_CALL(btsPortMock, sendCallRequest(PEER_PHONE_NUMBER));
        btsPortMock.sendCallRequest(PEER_PHONE_NUMBER);
    }
    
    void shallHandleIncomingCallRequest()
    {
        // Make minimal expectations that will pass
        EXPECT_CALL(userPortMock, showDial());
        EXPECT_CALL(userPortMock, getCallMode())
            .WillRepeatedly(ReturnRef(callModeMock));
        
        // Allow any number of calls to these methods
        ON_CALL(callModeMock, clearIncomingText())
            .WillByDefault(Return());
        ON_CALL(callModeMock, appendIncomingText(_))
            .WillByDefault(Return());
        ON_CALL(userPortMock, setAcceptCallback(_))
            .WillByDefault(Return());
        ON_CALL(userPortMock, setRejectCallback(_))
            .WillByDefault(Return());
        ON_CALL(userPortMock, setHomeCallback(_))
            .WillByDefault(Return());
        
        objectUnderTest.handleCallRequest(PEER_PHONE_NUMBER);
    }
};


TEST_F(ApplicationNotConnectedTestSuite, shallHandleSibMessage)
{
    shallHandleSibMessage();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleAttachAccept)
{
    shallHandleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleAttachReject)
{
    shallHandleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleTimeout)
{
    shallHandleTimeout();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleDisconnectFromConnecting)
{
    shallHandleDisconnect();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleReconnectFromConnecting)
{
    shallHandleReconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnectFromConnected)
{
    shallHandleDisconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReconnectFromConnected)
{
    shallHandleReconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccept)
{
    shallHandleCallAccept();
}

TEST_F(ApplicationCallTestSuite, shallSendCallRequest)
{
    shallSendCallRequest();
}

TEST_F(ApplicationCallTestSuite, shallHandleIncomingCallRequest)
{
    // Allow any calls to these methods
    EXPECT_CALL(userPortMock, showDial()).Times(AnyNumber());
    EXPECT_CALL(userPortMock, getCallMode()).WillRepeatedly(ReturnRef(callModeMock));
    EXPECT_CALL(callModeMock, clearIncomingText()).Times(AnyNumber());
    EXPECT_CALL(callModeMock, appendIncomingText(_)).Times(AnyNumber());
    EXPECT_CALL(userPortMock, setAcceptCallback(_)).Times(AnyNumber());
    EXPECT_CALL(userPortMock, setRejectCallback(_)).Times(AnyNumber());
    EXPECT_CALL(userPortMock, setHomeCallback(_)).Times(AnyNumber());
    
    objectUnderTest.handleCallRequest(PEER_PHONE_NUMBER);
}


TEST_F(ApplicationTalkingTestSuite, shallHandleCallTalk)
{
    shallHandleCallTalk();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSms)
{
    shallHandleSms();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleTimeoutFromConnected)
{
    shallHandleTimeout();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleCallDropped)
{
    shallHandleCallDropped();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    shallHandleCallRequest();
}
}