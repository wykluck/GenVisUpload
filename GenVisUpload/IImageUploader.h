#pragma once
#include "ImageStructQueue.h"

class IImageUploader
{
public:
	virtual void Connect(const std::string& uploadUrl, const std::string& dbUrl) = 0;
	virtual void UploadFrom(ImageStructQueuePtr imageStructQueuePtr) = 0;
};