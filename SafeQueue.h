#pragma once
#pragma once
#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<std::shared_ptr<T> > TaskQueue;
	std::condition_variable condVariable;
public:
	threadsafe_queue()
	{}
	void WaitPop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		condVariable.wait(lk, [this] {return !TaskQueue.empty(); });
		value = std::move(*TaskQueue.front());
		TaskQueue.pop();
	}
	bool TryPop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (TaskQueue.empty())
			return false;
		value = std::move(*TaskQueue.front());
		TaskQueue.pop();
		return true;
	}
	std::shared_ptr<T> WaitPop()
	{
		std::unique_lock<std::mutex> lk(mut);
		condVariable.wait(lk, [this] {return !TaskQueue.empty(); });
		std::shared_ptr<T> res = TaskQueue.front();
		TaskQueue.pop();
		return res;
	}
	std::shared_ptr<T> TryPop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (TaskQueue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res = TaskQueue.front();
		TaskQueue.pop();
		return res;
	}
	void push(T new_value)
	{
		std::shared_ptr<T> data(
			std::make_shared<T>(std::move(new_value)));
		std::lock_guard<std::mutex> lk(mut);
		TaskQueue.push(data);
		condVariable.notify_one();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return TaskQueue.empty();
	}
};
