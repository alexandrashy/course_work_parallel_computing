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
#include "InvertedIndexF.h"

unsigned const thread_count = 20; //td::thread::hardware_concurrency()

class thread_pool
{
    std::atomic_bool done;
    std::vector<std::unique_ptr<threadsafe_queue<std::string>>> queues;
    std::vector<std::thread> threads;
    join_threads joiner;
    std::mutex mtx;
    void worker_thread(int i)
    {
        while (!done)
        {
            std::string task;
            if (queues[i]->TryPop(task))
            {
                BoolInver(task);
                std::cout << "taskfin";
                queues[i]->TryPop(task);
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
                queues.push_back(std::make_unique<threadsafe_queue<std::string>>());
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
    bool submit(FunctionType f)
    {
        for (unsigned i = 0; i < thread_count; ++i) {
            if (queues[i]->empty())
            {
                queues[i]->push(f);
                queues[i]->push(f);
                return true;
            }
            else {
                std::cout << "\nno threads available\n";
                return false;
            }
        }
        return false;
    }
};

