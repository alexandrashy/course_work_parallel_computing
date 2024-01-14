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
#include <future>

void threadFunction(std::string Path,int i, int delimeter, std::promise<std::unordered_map<std::string, std::vector<std::string>>>&& resultPromise) {
   // std::cout << Path <<"-" << i <<"-"<< delimeter << "\n";
    std::unordered_map<std::string, std::vector<std::string>> InvertIn = CreateInvertedIndex(Path, i , delimeter);
    resultPromise.set_value(InvertIn);
}

unsigned thread_count = 5; //td::thread::hardware_concurrency()
int child_threads_count = 2;
class thread_pool
{
    std::atomic_bool done;
    std::vector<std::unique_ptr<threadsafe_queue<USER_DATA>>> queues;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread(int i)
    {
        while (!done)
        {
            USER_DATA task;
            if (queues[i]->TryPop(task))
            {
                std::vector<std::thread> child_threads;
                std::vector<std::promise<std::unordered_map<std::string, std::vector<std::string>>>> promises;
                std::vector<std::future<std::unordered_map<std::string, std::vector<std::string>>>> futures;

                // Create threads, promises, and futures
                for (int child_c = 0; child_c < child_threads_count; child_c++) {
                    promises.emplace_back();  // Create a promise
                    futures.push_back(promises.back().get_future());  // Get future
                    child_threads.emplace_back(threadFunction, task.Path, child_c, child_threads_count, std::move(promises.back()));  // Визиваємо функцію
                }
                // Retrieve results from futures
                for (int fut = 0; fut < child_threads_count; ++fut) {
                    std::unordered_map<std::string, std::vector<std::string>> result = futures[fut].get();
                    std::ofstream outputFile("users_data\\" + task.USER_NAME + std::to_string(fut) + ".txt", std::ios::app);
                    for (const auto& pair : result) {
                        outputFile << '"' << pair.first << "\":";
                        for (const auto& chain : pair.second)
                            outputFile << "\"" << chain << "\",";
                        outputFile << "\n";
                    }
                    outputFile << "==== future delimeter ====\n\n";
                    outputFile.close();
                }
                // Join the threads
                auto payload_begin = high_resolution_clock::now();
                for (int child_c = 0; child_c < child_threads_count; child_c++) {
                    child_threads[child_c].join();
                }
                auto payload_end = high_resolution_clock::now();
                auto elapsed = duration_cast<nanoseconds>(payload_end - payload_begin);
                std::cout << "\nPayload Time for joining: " << elapsed.count() * 1e-9 << " seconds.\n";
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

