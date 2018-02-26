#pragma once
#include "./thirdparty/concurrentqueue/concurrentqueue.h"
#include <stdexcept>
#include <condition_variable>
#include "ImageStruct.h"
class ImageStructQueue
{
public:
	ImageStructQueue() :m_itemCountForProcessing(0)
	{

	}
	void enqueue(ImageStructPtr imageStructPtr, bool fromProducer) {
		m_internalQueue.enqueue(imageStructPtr);
		if (fromProducer)
		{
			std::unique_lock<std::mutex> lck(cv_m);
			m_itemCountForProcessing++;
		}
	}
	bool try_dequeue(ImageStructPtr& imageStructPtr) {
		return m_internalQueue.try_dequeue(imageStructPtr);
	}
	void NotifyProducerIfNecessary(bool bProcessedOneItem)
	{
		std::unique_lock<std::mutex> lck(cv_m);
		if (bProcessedOneItem)
			m_itemCountForProcessing--;
		if (m_itemCountForProcessing == 0)
			cv.notify_all();
	}
	void WaitForAllItemProcessed()
	{
		std::unique_lock<std::mutex> lck(cv_m);
		cv.wait(lck, [&]() {return m_itemCountForProcessing == 0; });
	}
private:

	moodycamel::ConcurrentQueue<ImageStructPtr> m_internalQueue;
	std::condition_variable cv;
	std::mutex cv_m;
	int m_itemCountForProcessing;
};

typedef std::shared_ptr<ImageStructQueue> ImageStructQueuePtr;
