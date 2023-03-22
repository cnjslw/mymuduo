#pragma once
#include "noncopyable.h"
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>

class Thread : noncopyable {
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc func, const std::string& name = std::string());
    ~Thread();

public:
    void start();
    void join();
    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }
    static int numCreated() { return numCreated_; }

private:
    void setDefaultName();

private:
    bool started_;
    bool joined_;
    // 通过智能指针来管理线程启动的时机
    std::shared_ptr<std::thread> thread_;

    pid_t tid_;
    ThreadFunc func_;
    std::string name_; // 线程名字，方便调试
    static std::atomic_int numCreated_;
};