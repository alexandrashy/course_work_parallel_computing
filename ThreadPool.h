#pragma once
#include <iostream>
#include "SafeQueue.h"
#include "Join.h"
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <random>
#include <queue>
#include <mutex>
#include <memory>

unsigned const thread_count = 8;
int addedToExecution = 0;
int ThrownTasks = 0;

class thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<std::function<void()> > work_queue;
    std::vector<std::unique_ptr<threadsafe_queue<std::function<void()>>>> queues;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread(int i)
    {
        while (!done)
        {
            std::function<void()> task;
            if (queues[i]->TryPop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
public:
    thread_pool() :
        done(false), joiner(threads)
    {
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread([this, i] { worker_thread(i); }));
                queues.push_back(std::make_unique<threadsafe_queue<std::function<void()>>>());
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }
    ~thread_pool()
    {
        done = true;
    }
    template<typename FunctionType>
    void submit(FunctionType f, int a)
    {
        for (unsigned i = 0; i < thread_count; ++i) {
            if (queues[i]->empty())
            {
                queues[i]->push(std::function<void()>(f));
                addedToExecution++;
                std::cout << "Task with index " << a << " started execution\n";
                return;
            }
            else {
                continue;
            }
        }
        std::cout << "All threads is busy, task with index " << a << " leave execution\n";
        ThrownTasks++;

    }
};
