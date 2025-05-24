#pragma once

#include "Ports/ITimerPort.hpp"
#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"


namespace ue
{

class IEventsHandler : public IBtsEventsHandler,
                       public IUserEventsHandler,
                       public ITimerEventsHandler
{
public:
    virtual ~IEventsHandler() = default;

    virtual void handleUIAction(const std::optional<std::size_t> selectedIndex) = 0;
    virtual void handleUIBack() = 0;
};

}
