#pragma once
#include "./thirdparty/concurrentqueue/concurrentqueue.h"
#include <stdexcept>
#include <condition_variable>
#include "ImageStruct.h"
#include "DllExport.h"

class EXPORTED ImageStructQueue
{
public:
	ImageStructQueue() :m_itemCountForProcessing(0)
	{

	}
	virtual void enqueue(ImageStructPtr imageStructPtr, bool fromProducer) {
		m_internalQueue.enqueue(imageStructPtr);
		if (fromProducer)
		{
			std::unique_lock<std::mutex> lck(cv_m);
			m_itemCountForProcessing++;
		}
	}
	virtual bool try_dequeue(ImageStructPtr& imageStructPtr) {
		return m_internalQueue.try_dequeue(imageStructPtr);
	}
	virtual void NotifyProducerIfNecessary(bool bProcessedOneItem)
	{
		std::unique_lock<std::mutex> lck(cv_m);
		if (bProcessedOneItem)
			m_itemCountForProcessing--;
		if (m_itemCountForProcessing == 0)
			cv.notify_all();
	}
	virtual void WaitForAllItemProcessed()
	{
		std::unique_lock<std::mutex> lck(cv_m);
		cv.wait(lck, [&]() {return m_itemCountForProcessing == 0; });
	}
	bool NeedMoreItems()
	{
		return m_itemCountForProcessing == 0;
	}
private:

	moodycamel::ConcurrentQueue<ImageStructPtr> m_internalQueue;
	std::condition_variable cv;
	std::mutex cv_m;
	int m_itemCountForProcessing;
};

typedef std::shared_ptr<ImageStructQueue> ImageStructQueuePtr;
