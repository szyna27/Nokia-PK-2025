#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState")
{
    context.user.showTalking();
    logger.logDebug("Entering TalkingState");
}

}