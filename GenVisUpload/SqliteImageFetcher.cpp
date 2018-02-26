
#include "SqliteImageFetcher.h"
#include <sqlite3.h>

SqliteImageFetcher::SqliteImageFetcher()
{
}


SqliteImageFetcher::~SqliteImageFetcher()
{
}

void SqliteImageFetcher::Open(const std::string& imageSourceUrl)
{
	m_dbWrapperPtr.reset(new SqliteDBWrapper(imageSourceUrl));
}

void SqliteImageFetcher::SelectCallBack(int id, std::vector<unsigned char>& data) 
{
	ImageStructPtr imageStructPtr(new ImageStruct(id, data));
	m_queuePtr->enqueue(imageStructPtr, true);
}

void SqliteImageFetcher::FetchTo(ImageStructQueuePtr imageStructQueuePtr)
{
	while (1)
	{
		//wait for the condition variable
		imageStructQueuePtr->WaitForAllItemProcessed();

		//Select item from database and push them into the queue
		m_queuePtr = imageStructQueuePtr;
		auto callback = std::bind(&SqliteImageFetcher::SelectCallBack, this, std::placeholders::_1, std::placeholders::_2);
		m_dbWrapperPtr->SelectItems(callback);
	}
}