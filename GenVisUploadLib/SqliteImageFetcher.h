#pragma once
#include "IImageFetcher.h"
#include "IDBWrapper.h"
#include <condition_variable>
#include <boost/optional.hpp>
#include "DllExport.h"
class EXPORTED SqliteImageFetcher :
	public IImageFetcher
{
public:
	SqliteImageFetcher(DBWrapperPtr dbWrapperPtr, unsigned short maxSelectionRecord);
	virtual ~SqliteImageFetcher();
	virtual void FetchTo(ImageStructQueuePtr imageStructQueuePtr);
	virtual void WaitToFetchOnce(ImageStructQueuePtr imageStructQueuePtr);
private:
	void SelectCallBack(int id, std::vector<unsigned char>& data);
	DBWrapperPtr m_dbWrapperPtr;
	ImageStructQueuePtr m_queuePtr;
	boost::optional<int> m_lastSelectionId;
	unsigned short m_maxSelectionRecord;
};

