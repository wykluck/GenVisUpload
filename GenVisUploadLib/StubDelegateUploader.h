#pragma once
#include "IDelegateUploader.h"
#include "DllExport.h"
class EXPORTED StubDelegateUploader :
	public IDelegateUploader
{
public:
	StubDelegateUploader();
	virtual ~StubDelegateUploader();

	virtual void Connect(const std::string& uploadUrl);
	virtual bool Upload(ImageStructPtr imageStructPtr);
};

