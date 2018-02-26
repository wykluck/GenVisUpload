#pragma once
#include "DllExport.h"
#include "IImageUploader.h"
#include "IDBWrapper.h"
#include "IDelegateUploader.h"
class EXPORTED SyncImageUploader : public IImageUploader
{
public:
	SyncImageUploader(DBWrapperPtr dbWrapperPtr, DelegateUploaderPtr delegateUploaderPtr);
	~SyncImageUploader();


	virtual void UploadFrom(ImageStructQueuePtr imageStructQueuePtr);
	bool TryUploadOneItem(ImageStructQueuePtr imageStructQueuePtr);
private:
	DBWrapperPtr m_dbWrapperPtr;
	DelegateUploaderPtr m_delegateUploaderPtr;
};