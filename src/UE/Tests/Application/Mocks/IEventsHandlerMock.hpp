#pragma once
#include "Application/IEventsHandler.hpp"
#include <optional>
#include <gmock/gmock.h>

namespace ue{

class IEventsHandlerMock : public IEventsHandler 
{

    public:
        MOCK_METHOD(void, handleUIAction, (const std::optional<std::size_t> selectedIndex), (override));
        MOCK_METHOD(void, handleUIBack, (), (override));

};

}