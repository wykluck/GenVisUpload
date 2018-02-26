
#include "SqliteImageFetcher.h"
#include <sqlite3.h>

SqliteImageFetcher::SqliteImageFetcher(DBWrapperPtr dbWrapperPtr) 
	: m_dbWrapperPtr(dbWrapperPtr)
{
}


SqliteImageFetcher::~SqliteImageFetcher()
{
}


void SqliteImageFetcher::SelectCallBack(int id, std::vector<unsigned char>& data) 
{
	ImageStructPtr imageStructPtr(new ImageStruct(id, data));
	m_queuePtr->enqueue(imageStructPtr, true);
}

void SqliteImageFetcher::WaitToFetchOnce(ImageStructQueuePtr imageStructQueuePtr)
{
	//wait for the condition variable
	imageStructQueuePtr->WaitForAllItemProcessed();

	//Select item from database and push them into the queue
	m_queuePtr = imageStructQueuePtr;
	auto callback = std::bind(&SqliteImageFetcher::SelectCallBack, this, std::placeholders::_1, std::placeholders::_2);
	m_dbWrapperPtr->SelectItems(callback);
}

void SqliteImageFetcher::FetchTo(ImageStructQueuePtr imageStructQueuePtr)
{
	while (1)
	{
		WaitToFetchOnce(imageStructQueuePtr);
	}
}