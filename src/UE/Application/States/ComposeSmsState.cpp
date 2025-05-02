#include "ComposeSmsState.hpp"

namespace ue
{

ComposeSmsState::ComposeSmsState(Context &context)
    : ConnectedState(context)
{
    logger.logDebug("Entering ComposeSmsState");
}

}