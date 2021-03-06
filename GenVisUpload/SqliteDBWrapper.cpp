
#include "SqliteDBWrapper.h"
#include <sstream>
#include <vector>
#include <string.h>

SqliteDBWrapper::SqliteDBWrapper(const std::string& dbUrl): m_db(nullptr)
{
	int rc;

	rc = sqlite3_open(dbUrl.c_str(), &m_db);
	if (rc)
	{
		std::string errorMsg;
		errorMsg.append("Can't open database at ").
			append(dbUrl).append(" due to ").append(sqlite3_errmsg(m_db));
		throw std::runtime_error(errorMsg.c_str());
	}
}


SqliteDBWrapper::~SqliteDBWrapper()
{
	if (m_db)
		sqlite3_close(m_db);
}


void SqliteDBWrapper::BeginTransaction()
{
	sqlite3_exec(m_db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
}

void SqliteDBWrapper::RollBack()
{
	sqlite3_exec(m_db, "ROLLBACK;", NULL, NULL, NULL);
}

void SqliteDBWrapper::Commit()
{
	sqlite3_exec(m_db, "END TRANSACTION;", NULL, NULL, NULL);
}

void SqliteDBWrapper::DeleteItem(const int imageId)
{
	std::ostringstream sqlStrStream;
	char *sErrMsg = 0;
	sqlStrStream << "DELETE from Image WHERE ID=" << imageId << ";";
	int rc = sqlite3_exec(m_db, sqlStrStream.str().c_str(), nullptr, nullptr, &sErrMsg);
	if (rc != SQLITE_OK)
	{
		std::string sSqlErr = sErrMsg;
		sqlite3_free(sErrMsg);
		throw std::runtime_error(sSqlErr);
	}
}

void SqliteDBWrapper::SelectItems(std::function<void(int, std::vector<unsigned char>& data)> callback)
{
	sqlite3_stmt *statement;
	const char* sql = "SELECT ID, ImageContent FROM Image";
	if (sqlite3_prepare_v2(m_db, sql, strlen(sql), &statement, 0) != SQLITE_OK)
	{
		sqlite3_finalize(statement);
		throw std::runtime_error("Sql running error in preparing to select.");
	}

	int result = 0;
	while (true)
	{
		result = sqlite3_step(statement);

		if (result == SQLITE_ROW)
		{
			int id = sqlite3_column_int(statement, 0);
		
			int size = sqlite3_column_bytes(statement, 1);

			unsigned char* p = (unsigned char*)sqlite3_column_blob(statement, 1);
			std::vector<unsigned char> data(p, p+size);

			callback(id, data);
		}
		else
		{
			break;
		}
	}
	sqlite3_finalize(statement);
}