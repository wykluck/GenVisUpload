#pragma once
#include <string>
#include "./thirdparty/concurrentqueue/concurrentqueue.h"
#include "ImageStructQueue.h"

class IImageFetcher
{
public:
	virtual void Open(const std::string& imageSourceUrl) = 0;
	virtual void FetchTo(ImageStructQueuePtr imageStructQueuePtr) = 0;
};