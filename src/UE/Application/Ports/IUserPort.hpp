#pragma once

#include "IUeGui.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    virtual void setAcceptCallback(IUeGui::Callback) = 0;
    virtual void setRejectCallback(IUeGui::Callback) = 0;
    virtual void setHomeCallback(IUeGui::Callback) = 0;
    virtual void setItemSelectedCallback(IUeGui::Callback) = 0;
    virtual int getCurrentMode() = 0;
};

}
