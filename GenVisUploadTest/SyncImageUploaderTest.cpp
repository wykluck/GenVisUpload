#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../GenVisUploadLib/Mock_ImageStructQueue.h"
#include "../GenVisUploadLib/Mock_SqliteDBWrapper.h"
#include "../GenVisUploadLib/Mock_DelegateUploader.h"
#include "../GenVisUploadLib/SyncImageUploader.h"
#include "../GenVisUploadLib/ImageStruct.h"
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::NiceMock;
using ::testing::_;

class StubImageUploaderTestFixture : public ::testing::Test
{
public:
	StubImageUploaderTestFixture() {
		// initialization code here

	}

	void SetUp() {
		// code here will execute just before the test ensues 
		m_dbWrapperPtr.reset(new NiceMock<Mock_SqliteDBWrapper>());
		m_imageStructQueuePtr.reset(new NiceMock<Mock_ImageStructQueue>());
		m_imageStructPtr.reset(new ImageStruct(1, std::vector<unsigned char>()));
		m_delegateUploaderPtr.reset(new NiceMock<Mock_DelegateUploader>());
	}

	void TearDown() {
		// code here will be called just after the test completes
		// ok to through exceptions from here if need be
	}

	~StubImageUploaderTestFixture() {
		// cleanup any pending stuff, but no exceptions allowed
	}

	// put in any custom data members that you need 
	DBWrapperPtr m_dbWrapperPtr;
	ImageStructQueuePtr m_imageStructQueuePtr;
	ImageStructPtr m_imageStructPtr;
	DelegateUploaderPtr m_delegateUploaderPtr;
};

TEST_F(StubImageUploaderTestFixture, TryUploadOneItem_NonEmptyQueue)
{
	EXPECT_CALL((NiceMock<Mock_ImageStructQueue>&)*m_imageStructQueuePtr, try_dequeue(_)).Times(1).WillOnce(DoAll(SetArgReferee<0>(m_imageStructPtr), Return(true)));

	SyncImageUploader imageUploader(m_dbWrapperPtr, m_delegateUploaderPtr);

	ASSERT_EQ(true, imageUploader.TryUploadOneItem(m_imageStructQueuePtr));
}

TEST_F(StubImageUploaderTestFixture, TryUploadOneItem_EmptyQueue)
{
	EXPECT_CALL((NiceMock<Mock_ImageStructQueue>&)*m_imageStructQueuePtr, try_dequeue(_)).Times(1).WillOnce(DoAll(SetArgReferee<0>(m_imageStructPtr), Return(false)));

	SyncImageUploader imageUploader(m_dbWrapperPtr, m_delegateUploaderPtr);

	ASSERT_EQ(false, imageUploader.TryUploadOneItem(m_imageStructQueuePtr));

}

TEST_F(StubImageUploaderTestFixture, TryUploadOneItem_UploadSuccess)
{
	ON_CALL((NiceMock<Mock_ImageStructQueue>&)*m_imageStructQueuePtr, try_dequeue(_)).WillByDefault(DoAll(SetArgReferee<0>(m_imageStructPtr), Return(true)));
	ON_CALL((NiceMock<Mock_DelegateUploader>&)*m_delegateUploaderPtr, Upload(m_imageStructPtr)).WillByDefault(Return(true));
	EXPECT_CALL((NiceMock<Mock_SqliteDBWrapper>&)*m_dbWrapperPtr, DeleteItem(m_imageStructPtr->imageID));

	SyncImageUploader imageUploader(m_dbWrapperPtr, m_delegateUploaderPtr);

	imageUploader.TryUploadOneItem(m_imageStructQueuePtr);
}

TEST_F(StubImageUploaderTestFixture, TryUploadOneItem_UploadFailure)
{
	ON_CALL((NiceMock<Mock_ImageStructQueue>&)*m_imageStructQueuePtr, try_dequeue(_)).WillByDefault(DoAll(SetArgReferee<0>(m_imageStructPtr), Return(true)));
	ON_CALL((NiceMock<Mock_DelegateUploader>&)*m_delegateUploaderPtr, Upload(m_imageStructPtr)).WillByDefault(Return(false));
	EXPECT_CALL((NiceMock<Mock_ImageStructQueue>&)*m_imageStructQueuePtr, enqueue(m_imageStructPtr, false));

	SyncImageUploader imageUploader(m_dbWrapperPtr, m_delegateUploaderPtr);

	imageUploader.TryUploadOneItem(m_imageStructQueuePtr);
}