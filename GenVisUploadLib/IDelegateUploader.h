#pragma once
#include <string>
#include "ImageStruct.h"
#include <memory>
class IDelegateUploader
{
public:
	virtual void Connect(const std::string& uploadUrl) = 0;
	virtual bool Upload(ImageStructPtr imageStructPtr) = 0;
};

typedef std::shared_ptr<IDelegateUploader> DelegateUploaderPtr;