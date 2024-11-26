#pragma once

#include "ILogger.hpp"
#include <concepts>
#include <functional>

namespace common
{

namespace detail
{

class Prefix
{
public:
    using FunctionT = std::function<void(std::ostream&)>;

    template<std::convertible_to<FunctionT> Func>
    Prefix(Func f)
        : prefixAdder(FunctionT{f})
    {}

    auto operator()(std::ostream& os) const { return prefixAdder(os); }

private:
    FunctionT prefixAdder;
};

} // namespace detail

class PrefixedLogger : public ILogger
{
public:
    using Prefix = detail::Prefix;
    PrefixedLogger(ILogger& adaptee, Prefix prefix);
    PrefixedLogger(ILogger& adaptee, const std::string& prefix);

    void log(Level level, const std::string& message) override;

private:
    ILogger& adaptee;
    Prefix prefix;
};

} // namespace common
