#pragma once
#include <string>
#include <vector>
#include <memory>
struct ImageStruct
{
public:
	ImageStruct(int imageID_, std::vector<unsigned char>& imageBuffer_)
		:imageID(imageID_), imageBuffer(imageBuffer_)
	{

	}
	int imageID;
	std::vector<unsigned char> imageBuffer;
};

typedef std::shared_ptr<ImageStruct> ImageStructPtr;