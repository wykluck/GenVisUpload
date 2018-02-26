#pragma once
#include "ImageStructQueue.h"

class IImageUploader
{
public:
	virtual void UploadFrom(ImageStructQueuePtr imageStructQueuePtr) = 0;
};