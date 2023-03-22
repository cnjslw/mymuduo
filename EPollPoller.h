#pragma once
#include "Poller.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class Channel;
class EPollPoller : public Poller {
public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

    Timestamp poll(int timeoutMS, ChannelList* activeChannels) override;

private:
    static const int kInitEventListSize = 16;
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);
    using EventList = std::vector<epoll_event>;
    int epollfd_;
    EventList events_; // 不同于poller中的ChannelMap，可能还没有注册到poller上
};