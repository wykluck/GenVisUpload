
#include "SyncImageUploader.h"
#include <cstdlib>
#include <mutex>

SyncImageUploader::SyncImageUploader(DBWrapperPtr dbWrapperPtr, DelegateUploaderPtr delegateUploaderPtr)
	:m_dbWrapperPtr(dbWrapperPtr), m_delegateUploaderPtr(delegateUploaderPtr)
{
}


SyncImageUploader::~SyncImageUploader()
{
}

bool SyncImageUploader::TryUploadOneItem(ImageStructQueuePtr imageStructQueuePtr)
{
	ImageStructPtr imageStructPtr;
	if (imageStructQueuePtr->try_dequeue(imageStructPtr))
	{
		if (m_delegateUploaderPtr->Upload(imageStructPtr))
		{
			printf("Image with ID(%d) is uploaded successfully!\n", imageStructPtr->imageID);
			//Commit delete if it is successful
			m_dbWrapperPtr->DeleteItem(imageStructPtr->imageID);
			imageStructQueuePtr->NotifyProducerIfNecessary(true);
		}
		else
		{
			printf("Image with ID(%d) failed to be uploaded. Will try later !\n", imageStructPtr->imageID);
			//push the item back to the queue for retrying
			imageStructQueuePtr->enqueue(imageStructPtr, false);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void SyncImageUploader::UploadFrom(ImageStructQueuePtr imageStructQueuePtr)
{
	while (1)
	{
		if (!TryUploadOneItem(imageStructQueuePtr))
		{
			//queue is empty, notify the fetcher to fetch more and sleep a bit
			imageStructQueuePtr->NotifyProducerIfNecessary(false);
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}
}