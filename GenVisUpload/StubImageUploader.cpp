
#include "StubImageUploader.h"
#include <cstdlib>
#include <ctime>
#include <mutex>

StubImageUploader::StubImageUploader()
{
}


StubImageUploader::~StubImageUploader()
{
}

void StubImageUploader::Connect(const std::string& uploadUrl, const std::string& dbUrl)
{
	m_dbWrapperPtr.reset(new SqliteDBWrapper(dbUrl));

	return;
}

void StubImageUploader::uploadInternal(ImageStructPtr imageStructPtr, 
	ImageStructQueuePtr imageStructQueuePtr)
{
	m_dbWrapperPtr->BeginTransaction();
	m_dbWrapperPtr->DeleteItem(imageStructPtr->imageID);
	std::srand(std::time(NULL)); // use current time as seed for random generator
	int random_variable = std::rand();
	if (random_variable % 2 == 0)
	{
		
		printf("Image with ID(%d) is uploaded successfully!\n", imageStructPtr->imageID);
		//Commit delete if it is successful
		m_dbWrapperPtr->Commit();
		imageStructQueuePtr->NotifyProducerIfNecessary(true);
	}
	else
	{
		printf("Image with ID(%d) failed to be uploaded. Will try later !\n", imageStructPtr->imageID);
		//push the item back to the queue for retrying and rollback delete
		imageStructQueuePtr->enqueue(imageStructPtr, false);
		m_dbWrapperPtr->RollBack();
	}
}

void StubImageUploader::UploadFrom(ImageStructQueuePtr imageStructQueuePtr)
{
	while (1)
	{
		ImageStructPtr imageStructPtr;
		if (imageStructQueuePtr->try_dequeue(imageStructPtr))
		{
			uploadInternal(imageStructPtr, imageStructQueuePtr);
		}
		else
		{
			//queue is empty, notify the fetcher to fetch more and sleep a bit
			imageStructQueuePtr->NotifyProducerIfNecessary(false);
			std::this_thread::sleep_for(std::chrono::microseconds(100));

		}
	}
}