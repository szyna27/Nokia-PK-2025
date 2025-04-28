#pragma once
 
 #include "BaseState.hpp"
 
 namespace ue 
 {
     class CallingState : public BaseState
     {
         public:
             CallingState(Context &context);
             void handleTimeout() override;
             void handleCallDropped() override;
             void handleDisconnect() override;
             void handleUIEnding() override;
             void handleUICalling(std::optional<std::size_t> selectedIndex) override;
             void handleCallAccepted(common::PhoneNumber recipient) override;
             void handleUnknownRecipient(common::PhoneNumber recipient) override;
 
         private:
             common::PhoneNumber recipientNumber; 
 
     };
 }