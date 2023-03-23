#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "logger.h"
__thread EventLoop* t_loopInThisThread = nullptr;

// 设置超时事件
const int kPollTimeMs = 10000;

// 创建wakeupfd, 用来唤醒subReacotr处理新来的channel
int creatEventfd()
{
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_()
    , callingPendingFunctors_(false)
    , threadId_(CurrentThread::tid())
    , poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(creatEventfd())
    , wakeupChannel_(new Channel(this, wakeupFd_))
    , currentActiveChannel_(nullptr)
{
    LOG_DEBUG("EventLoop created %p in thread %d \n", this, threadId_);
    if (t_loopInThisThread) {
        LOG_FATAL("Another EventLoop %p exists in this thread %d \n", t_loopInThisThread, threadId_);
    } else {
        t_loopInThisThread = this;
    }

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}
EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR("EventLoop::handleRead() reads %lu bytes instead of 8", n);
    }
}