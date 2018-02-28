#pragma once
#include "gmock/gmock.h"
#include "IDBWrapper.h"
class Mock_SqliteDBWrapper : public IDBWrapper
{
public:
	MOCK_METHOD1(DeleteItem, void(const int imageId));
	MOCK_METHOD3(SelectItems, void(boost::optional<int> lastSelectionId, unsigned short maxSelectionRecord,
		std::function<void(int, std::vector<unsigned char>& data)> callback));
};