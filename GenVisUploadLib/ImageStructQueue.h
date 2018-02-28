#pragma once
#include "./thirdparty/concurrentqueue/concurrentqueue.h"
#include <stdexcept>
#include <condition_variable>
#include "ImageStruct.h"
#include "DllExport.h"

class EXPORTED ImageStructQueue
{
public:
	ImageStructQueue() :m_isQueueEmpty(true)
	{

	}
	virtual void enqueue(ImageStructPtr imageStructPtr) {
		m_internalQueue.enqueue(imageStructPtr);
	}
	virtual bool try_dequeue(ImageStructPtr& imageStructPtr) {
		bool dequeueRes = m_internalQueue.try_dequeue(imageStructPtr);
		if (!dequeueRes)
		{
			std::unique_lock<std::mutex> lck(cv_m);
			m_isQueueEmpty = true;
			cv.notify_all();
		}	
		return dequeueRes;
	}
	virtual void WaitForFetchMore()
	{
		if (m_internalQueue.size_approx() > 0)
			m_isQueueEmpty = false;
		std::unique_lock<std::mutex> lck(cv_m);
		cv.wait(lck, [&]() {return m_isQueueEmpty; });
	}
	bool NeedMoreItems()
	{
		return m_isQueueEmpty;
	}
private:

	moodycamel::ConcurrentQueue<ImageStructPtr> m_internalQueue;
	std::condition_variable cv;
	std::mutex cv_m;
	bool m_isQueueEmpty;
};

typedef std::shared_ptr<ImageStructQueue> ImageStructQueuePtr;
