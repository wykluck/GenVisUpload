#pragma once
#include <string>
#include "./thirdparty/concurrentqueue/concurrentqueue.h"
#include "ImageStructQueue.h"

class IImageFetcher
{
public:
	virtual void FetchTo(ImageStructQueuePtr imageStructQueuePtr) = 0;
};