#include "ViewSmsState.hpp"

namespace ue
{

ViewSmsState::ViewSmsState(Context &context)
    : BaseState(context, "ViewSmsState")
{
    context.user.showViewSms();
    logger.logDebug("Entering ViewSmsState");
}

}