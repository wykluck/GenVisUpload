#pragma once
#include "sqlite3.h"
#include "IDBWrapper.h"
#include "DllExport.h"
#include <string>
#include <memory>
#include <functional>
#include <vector>
class EXPORTED SqliteDBWrapper : public IDBWrapper
{
public:
	SqliteDBWrapper(const std::string& dbUrl);
	virtual ~SqliteDBWrapper();

	virtual void DeleteItem(const int imageId);
	virtual void SelectItems(std::function<void(int, std::vector<unsigned char>& data)> callback);

private:
	sqlite3* m_db;
};


typedef std::shared_ptr<SqliteDBWrapper> SqliteDBWrapperPtr;