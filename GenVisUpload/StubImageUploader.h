#pragma once
#include "IImageUploader.h"
#include "SqliteDBWrapper.h"

class StubImageUploader : public IImageUploader
{
public:
	StubImageUploader();
	virtual ~StubImageUploader();
	virtual void Connect(const std::string & uploadUrl, const std::string& dbUrl) override;
	virtual void UploadFrom(ImageStructQueuePtr imageStructQueuePtr) override;
private:
	void uploadInternal(ImageStructPtr imageStructPtr,
		ImageStructQueuePtr sourceQueuePtr);
	SqliteDBWrapperPtr m_dbWrapperPtr;
};

