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

// Function that runs in a separate thread
void threadFunction(std::string Path,int i, int delimeter, std::promise<std::unordered_map<std::string, std::vector<std::string>>>&& resultPromise) {
    // Perform some computation
   // std::cout << Path <<"-" << i <<"-"<< delimeter << "\n";
    std::unordered_map<std::string, std::vector<std::string>> InvertIn = CreateInvertedIndex(Path, i , delimeter);
    // Set the result in the promise
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
    std::mutex mtx;
    void worker_thread(int i)
    {
        while (!done)
        {
            USER_DATA task;
            if (queues[i]->TryPop(task))
            {
                //std::chrono::seconds duration(5);
                //std::this_thread::sleep_for(duration);
                std::vector<std::thread> child_threads;
                std::vector<std::promise<std::unordered_map<std::string, std::vector<std::string>>>> promises;
                std::vector<std::future<std::unordered_map<std::string, std::vector<std::string>>>> futures;

                // Create threads, promises, and futures
                for (int child_c = 0; child_c < child_threads_count; child_c++) {
                    promises.emplace_back();  // Create a promise for each thread
                    futures.push_back(promises.back().get_future());  // Get the future associated with the promise
                    child_threads.emplace_back(threadFunction, task.Path, child_c, child_threads_count, std::move(promises.back()));  // Start a thread with the promise
                }
                //std::cout << futures.size() << "future";
                //std::chrono::seconds duration(10);
                //std::this_thread::sleep_for(duration);
                // Retrieve results from futures
                for (int fut = 0; fut < child_threads_count; ++fut) {
                    std::unordered_map<std::string, std::vector<std::string>> result = futures[fut].get();
                    std::ofstream outputFile("users_data\\" + task.USER_NAME + ".txt", std::ios::app);
                    // Check if the file is opened successfully
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
                for (int child_c = 0; child_c < child_threads_count; child_c++) {
                    child_threads[child_c].join();
                }
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

