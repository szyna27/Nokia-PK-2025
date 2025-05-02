#include "DialState.hpp"

namespace ue
{

DialState::DialState(Context &context)
    : BaseState(context, "DialState")
{
    context.user.showDial();
    logger.logDebug("Entering DialState");
}

}