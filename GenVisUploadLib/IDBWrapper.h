#pragma once
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <boost/optional.hpp>
class IDBWrapper
{
public:
	virtual void DeleteItem(const int imageId) = 0;
	virtual void SelectItems(boost::optional<int> lastSelectionId, unsigned short maxSelectionRecord,
		std::function<void(int, std::vector<unsigned char>& data)> callback) = 0;
};
typedef std::shared_ptr<IDBWrapper> DBWrapperPtr;