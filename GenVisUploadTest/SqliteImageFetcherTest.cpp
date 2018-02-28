#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../GenVisUploadLib/Mock_ImageStructQueue.h"
#include "../GenVisUploadLib/Mock_SqliteDBWrapper.h"
#include "../GenVisUploadLib/Mock_DelegateUploader.h"
#include "../GenVisUploadLib/SqliteImageFetcher.h"
#include "../GenVisUploadLib/ImageStruct.h"
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::NiceMock;
using ::testing::_;

TEST(SqliteImageFetcherTest, WaitToFetchOnce_EmptiedQueue)
{
	ImageStructQueuePtr imageStructQueuePtr(new ImageStructQueue());
	DBWrapperPtr dbWrapperPtr(new Mock_SqliteDBWrapper());
	SqliteImageFetcher sqliteImageFetcher(dbWrapperPtr, 20);

	EXPECT_CALL((Mock_SqliteDBWrapper&)*dbWrapperPtr, SelectItems(_, _, _)).Times(1);
	sqliteImageFetcher.WaitToFetchOnce(imageStructQueuePtr);
	
}