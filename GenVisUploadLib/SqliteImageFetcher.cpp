
#include "SqliteImageFetcher.h"
#include <sqlite3.h>

SqliteImageFetcher::SqliteImageFetcher(DBWrapperPtr dbWrapperPtr, unsigned short maxSelectionRecord)
	: m_dbWrapperPtr(dbWrapperPtr), m_maxSelectionRecord(maxSelectionRecord)
{
}


SqliteImageFetcher::~SqliteImageFetcher()
{
}


void SqliteImageFetcher::SelectCallBack(int id, std::vector<unsigned char>& data) 
{
	if (!m_lastSelectionId.is_initialized() ||
		id > m_lastSelectionId)
		m_lastSelectionId = id;	
	ImageStructPtr imageStructPtr(new ImageStruct(id, data));
	m_queuePtr->enqueue(imageStructPtr);
}

void SqliteImageFetcher::WaitToFetchOnce(ImageStructQueuePtr imageStructQueuePtr)
{
	//wait for the condition variable
	imageStructQueuePtr->WaitForFetchMore();

	//Select item from database and push them into the queue
	m_queuePtr = imageStructQueuePtr;
	auto callback = std::bind(&SqliteImageFetcher::SelectCallBack, this, std::placeholders::_1, std::placeholders::_2);
	m_dbWrapperPtr->SelectItems(m_lastSelectionId, m_maxSelectionRecord, callback);
}

void SqliteImageFetcher::FetchTo(ImageStructQueuePtr imageStructQueuePtr)
{
	while (1)
	{
		WaitToFetchOnce(imageStructQueuePtr);
	}
}