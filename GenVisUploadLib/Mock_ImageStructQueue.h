#pragma once
#include "gmock/gmock.h"
#include "ImageStructQueue.h"

class Mock_ImageStructQueue : public ImageStructQueue
{
public:
	MOCK_METHOD1(enqueue, void(ImageStructPtr imageStructPtr));
	MOCK_METHOD1(try_dequeue, bool(ImageStructPtr& imageStructPtr));
	MOCK_METHOD0(WaitForFetchMore, void());
};