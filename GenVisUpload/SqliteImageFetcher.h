#pragma once
#include "IImageFetcher.h"
#include "SqliteDBWrapper.h"
#include <condition_variable>
class SqliteImageFetcher :
	public IImageFetcher
{
public:
	SqliteImageFetcher();
	virtual ~SqliteImageFetcher();
	virtual void Open(const std::string& imageSourceUrl);
	virtual void FetchTo(ImageStructQueuePtr imageStructQueuePtr);
private:
	void SelectCallBack(int id, std::vector<unsigned char>& data);
	SqliteDBWrapperPtr m_dbWrapperPtr;
	ImageStructQueuePtr m_queuePtr;
};

