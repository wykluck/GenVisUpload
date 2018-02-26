#pragma once
#include "sqlite3.h"
#include <string>
#include <memory>
#include <functional>
#include <vector>
class SqliteDBWrapper
{
public:
	SqliteDBWrapper(const std::string& dbUrl);
	virtual ~SqliteDBWrapper();

	void BeginTransaction();
	void RollBack();
	void Commit();
	void DeleteItem(const int imageId);
	void SelectItems(std::function<void(int, std::vector<unsigned char>& data)> callback);

private:
	sqlite3* m_db;
};

typedef std::shared_ptr<SqliteDBWrapper> SqliteDBWrapperPtr;