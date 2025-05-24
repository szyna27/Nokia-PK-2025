#include "TimerPort.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
}

void TimerPort::startTimer(Duration duration)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_all();

    {
        std::lock_guard<std::mutex> lock(mtx);
        running = true;
        currentDuration = duration;
    }

    if (timerThread.joinable())
        timerThread.join();

    timerThread = std::thread([this] {
        std::unique_lock<std::mutex> lock(mtx);
        auto duration = currentDuration;
        if (cv.wait_for(lock, duration, [this] { return !running; })) {
            logger.logDebug("Timer stopped before timeout");
            return;
        }
        running = false;
        if (handler) {
            logger.logDebug("Timer expired, calling handleTimeout");
            handler->handleTimeout();
        }
    });
    logger.logDebug("Start timer: ", duration.count(), "ms");
}

void TimerPort::stopTimer()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_all();
    if (timerThread.joinable())
        timerThread.join();
    logger.logDebug("Stop timer");
}

}
