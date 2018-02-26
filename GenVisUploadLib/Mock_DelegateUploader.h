#pragma once
#include "IDelegateUploader.h"
#include "gmock/gmock.h"
class Mock_DelegateUploader : public IDelegateUploader
{
public:
	MOCK_METHOD1(Connect, void(const std::string& uploadUrl));
	MOCK_METHOD1(Upload, bool(ImageStructPtr imageStructPtr));
};