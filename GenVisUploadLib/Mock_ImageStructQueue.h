#pragma once
#include "gmock/gmock.h"
#include "ImageStructQueue.h"

class Mock_ImageStructQueue : public ImageStructQueue
{
public:
	MOCK_METHOD2(enqueue, void(ImageStructPtr imageStructPtr, bool fromProducer));
	MOCK_METHOD1(try_dequeue, bool(ImageStructPtr& imageStructPtr));
	MOCK_METHOD1(NotifyProducerIfNecessary, void(bool bProcessedOneItem));
	MOCK_METHOD0(WaitForAllItemProcessed, void());
};