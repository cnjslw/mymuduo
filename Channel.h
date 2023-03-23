#pragma once

#include <functional>
#include <memory>

#include "Timestamp.h"
#include "noncopyable.h"

class EventLoop;

class Channel : noncopyable {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;
    Channel(EventLoop* loop, int fd);
    ~Channel();
    // fd得到poller通知后处理事件
    void handleEvent(Timestamp receiveTime);
    // 设置回调函数
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    // 防止channel被手动remove时，channel还在执行回调操作
    void tie(const std::shared_ptr<void>&);

    int fd() const { return fd_; }
    int events() const { return events_; } // 注册了哪些事件
    void set_revents(int revt) { revents_ = revt; }
    void enableReading()
    {
        events_ |= kReadEvent; // 关注了读事件
        update(); // 注册到EventLoop -》Poller
    }
    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        events_ &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    // one loop per thread
    EventLoop* ownerLoop() { return loop_; }
    void remove(); // 在disableAll()后，移除

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    EventLoop* loop_; // 所属EventLoop
    const int fd_; // Poller监听的对象
    int events_; // 注册fd感兴趣的事件
    int revents_; // poller返回的具体发生的事件
    int index_; // used by Poller, 表示Poller的事件数组中的序号

    std::weak_ptr<void> tie_; // 获取当前对象的智能指针
    bool tied_;

    // 不同事件的回调函数
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};