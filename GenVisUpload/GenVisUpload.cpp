// GenVisUpload.cpp : Defines the entry point for the console application.
//

#include "../GenVisUploadLib/SqliteImageFetcher.h"
#include "../GenVisUploadLib/SyncImageUploader.h"
#include "../GenVisUploadLib/SqliteDBWrapper.h"
#include "../GenVisUploadLib/StubDelegateUploader.h"
#include <stdexcept>
#include "../GenVisUploadLib/ImageStructQueue.h"
static ImageStructQueue s_imageStructQueue;
static int uploadThreadNum = 16;
static unsigned short maxSelectionRecord = 20;
static std::vector<std::thread> threadVec;
/* argv[1] is sqlite db path, argv[2] is "aws", argv[3] is optional a log file path*/
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Invalid command options\n");
		return 1;
	}


	std::shared_ptr<ImageStructQueue> imageStructQueuePtr(&s_imageStructQueue);
	try
	{
		DBWrapperPtr dbWrapperPtr(new SqliteDBWrapper(argv[1]));
		DelegateUploaderPtr delegateUploaderPtr(new StubDelegateUploader());
		delegateUploaderPtr->Connect(argv[2]);
		SqliteImageFetcher imageFetcher(dbWrapperPtr, maxSelectionRecord);
		SyncImageUploader imageUploader(dbWrapperPtr, delegateUploaderPtr);
		//start different upload thread to pull item from the queue and upload
		for (int i = 0; i < uploadThreadNum; i++)
		{
			threadVec.push_back(std::thread([&]() {
				imageUploader.UploadFrom(imageStructQueuePtr);
			}));
		}
		//main thread to continously fetch images from database to the queue
		imageFetcher.FetchTo(imageStructQueuePtr);
	}
	catch (std::runtime_error& ex)
	{
		fprintf(stderr, ex.what());
		return -1;
	}


	//no need to join the threads as it should be running forever

	return 0;
}

