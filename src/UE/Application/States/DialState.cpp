#include "DialState.hpp"

namespace ue
{

DialState::DialState(Context &context)
    : ConnectedState(context)
{
    logger.logDebug("Entering DialState");
}

}