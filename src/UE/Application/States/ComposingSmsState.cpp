#pragma once
#include "ComposingSmsState.hpp"
#include "Context.hpp"

ue::ComposingSmsState::ComposingSmsState(Context &context)
{
    context.user.showComposeSMS();
}

void ue::ComposingSmsState::handleAccept()
{
    printf("ComposingSmsState::handleAccept");
}

void ue::ComposingSmsState::handleReject() {
    // Handle reject action
}

void ue::ComposingSmsState::handleDisconnect() {
    // Handle disconnect action
}

void ue::ComposingSmsState::handleSms(common::PhoneNumber from, std::string message) {
    // Handle incoming SMS
}