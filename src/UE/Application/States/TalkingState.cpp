#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
    : ConnectedState(context)
{
    logger.logDebug("Entering TalkingState");
}

}