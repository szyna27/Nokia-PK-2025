#include "ViewSmsState.hpp"

namespace ue
{

ViewSmsState::ViewSmsState(Context &context)
    : ConnectedState(context)
{
    logger.logDebug("Entering ViewSmsState");
}

}