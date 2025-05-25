#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include "SMS/SMSDB.hpp"
#include <memory>

namespace ue
{

struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    SMSDB& smsDB;
    std::unique_ptr<IEventsHandler> state{};

    template <typename State, typename ...Arg>
    void setState(Arg&& ...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }

    template <typename State>
    bool isState() const
    {
        return dynamic_cast<State*>(state.get()) != nullptr;
    }
};

}
