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
        EXPECT_CALL(userPortMock, showConnected());
        EXPECT_CALL(userPortMock, showMainMenu());
        EXPECT_CALL(userPortMock, getListViewMode()).WillOnce(ReturnRef(listViewModeMock));
        EXPECT_CALL(userPortMock, setItemSelectedCallback(_));
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
        EXPECT_CALL(userPortMock, showTalking());
        EXPECT_CALL(userPortMock, setHomeCallback(_));
        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));
        EXPECT_CALL(timerPortMock, startTimer(30000ms));

        objectUnderTest.handleCallAccept(PEER_PHONE_NUMBER);
    }

    void shallHandleTimeout(){
        EXPECT_CALL(userPortMock, getDialMode()).WillOnce(ReturnRef(dialModeMock));
        EXPECT_CALL(dialModeMock, getPhoneNumber()).WillOnce(Return(PEER_PHONE_NUMBER));
        EXPECT_CALL(btsPortMock, sendCallDrop(PEER_PHONE_NUMBER));
        EXPECT_CALL(userPortMock, showMainMenu());

        objectUnderTest.handleTimeout();
    }

    void shallHandleCallDropped(){
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showMainMenu());
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

TEST_F(ApplicationTalkingTestSuite, shallHandleCallTalk)
{
    shallHandleCallTalk();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSms)
{
    shallHandleSms();
}

}