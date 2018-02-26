#include "StubDelegateUploader.h"
#include <ctime>


StubDelegateUploader::StubDelegateUploader()
{
}


StubDelegateUploader::~StubDelegateUploader()
{
}


void StubDelegateUploader::Connect(const std::string& uploadUrl)
{
	return;
}

bool StubDelegateUploader::Upload(ImageStructPtr imageStructPtr)
{
	std::srand(std::time(NULL)); // use current time as seed for random generator
	int random_variable = std::rand();
	if (random_variable % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}