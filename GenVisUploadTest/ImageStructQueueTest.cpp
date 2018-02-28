#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../GenVisUploadLib/ImageStructQueue.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::NiceMock;
using ::testing::_;

TEST(ImageStructQueueTest, constructor)
{
	ImageStructQueue imageStructQueue;
	ASSERT_EQ(true, imageStructQueue.NeedMoreItems());
}

TEST(ImageStructQueueTest, try_dequeue_EmptyQueue)
{
	ImageStructQueue imageStructQueue;
	ImageStructPtr imageStructPtr;
	
	ASSERT_EQ(false, imageStructQueue.try_dequeue(imageStructPtr));
	ASSERT_EQ(nullptr, imageStructPtr.get());
	ASSERT_EQ(true, imageStructQueue.NeedMoreItems());
}

TEST(ImageStructQueueTest, try_dequeue_NonEmptyQueue)
{
	ImageStructQueue imageStructQueue;
	std::vector<unsigned char> buffer;
	ImageStructPtr imageStructPtr(new ImageStruct(1, buffer));
	imageStructQueue.enqueue(imageStructPtr);
	ASSERT_EQ(true, imageStructQueue.try_dequeue(imageStructPtr));
	imageStructQueue.WaitForFetchMore();
	ASSERT_EQ(true, imageStructQueue.NeedMoreItems());
	ASSERT_EQ(1, imageStructPtr->imageID);
}


TEST(ImageStructQueueTest, WaitForEmptyQueue_EmptyQueue)
{
	ImageStructQueue imageStructQueue;
	imageStructQueue.WaitForFetchMore();
}

TEST(ImageStructQueueTest, WaitForEmptyQueue_EmptiedQueue)
{
	ImageStructQueue imageStructQueue;
	ImageStructPtr imageStructPtr;
	imageStructQueue.enqueue(imageStructPtr);
	std::thread producerThread([&]() {
		imageStructQueue.WaitForFetchMore();
	});
	ASSERT_EQ(true, imageStructQueue.try_dequeue(imageStructPtr));
	ASSERT_EQ(false, imageStructQueue.try_dequeue(imageStructPtr));
	producerThread.join();
}
