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
#include <unordered_map>
#include "UserDataStructure.h"
unsigned thread_count = 1; //td::thread::hardware_concurrency()

class thread_pool
{
    std::atomic_bool done;
    std::vector<std::unique_ptr<threadsafe_queue<USER_DATA>>> queues;
    std::vector<std::thread> threads;
    join_threads joiner;
    std::mutex mtx;
    void worker_thread(int i)
    {
        while (!done)
        {
            USER_DATA task;
            if (queues[i]->TryPop(task))
            {
                std::chrono::seconds duration(5);
                std::this_thread::sleep_for(duration);
                std::unordered_map<std::string, std::vector<std::string>> InvertIn = CreateInvertedIndex(task.Path);
                std::ofstream outputFile("users_data\\" + task.USER_NAME+".txt");
                // Check if the file is opened successfully

                for (const auto& pair : InvertIn) {
                  outputFile << '"' << pair.first << "\":";
                  for (const auto& chain : pair.second)
                      outputFile << "\"" << chain << "\",";
                  outputFile << "\n";
                }
                outputFile << "\n\n";
                outputFile.close();
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
                queues.push_back(std::make_unique<threadsafe_queue<USER_DATA>>());
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
        for (unsigned int i = 0; i < thread_count; ++i) {
            if (queues[i]->empty())
            {
                queues[i]->push(f);
                queues[i]->push(f);
                return true;
            }
            else {
                continue;
            }
        }
        std::cout << "\nno threads available\n";
        return false;
    }
};

