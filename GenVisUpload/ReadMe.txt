========================================================================
    CONSOLE APPLICATION : GenVisUpload Project Overview
========================================================================

About Design:
GenVisUpload is using a sqlite database to store images. There is a sample database ImageDB.db which contains 100 png images. It could be opened from Sqlite browser in Windows.

Main thread: One SqliteImageFetcher is supposed to fetch(select) image binaries from database and push into a lockfree queue. Then it waits there until all of the items are processed by the StubImageUploader.

Upload threads: Each StubImageUploader works as a separate thead and try to pull items from the lockfree queue. After retrieving an item and it will generate a random number to mimic upload success or failure behaviour. If it is successfully, the item will be deleted from the database. Otherwise, the item will be put back into the queue for retrying. If all of the items have been deleted (successful upload), the thread will signal SqliteImageFetcher to fetch more items. 

Easy to extend: There is two interface IImageFetcher and IImageUploader. So they should be easily extended to different Fetcher and Uploader. Say there could be FileImageFetcher which could directly push image files from a directory into the queue. 

Easy to test: The two interfaces make unit tests easier as well. Because they can be easily mocked up to test other classes. 

How the requirements are met:
1. There should be no image duplications in the cloud. 
	Once one item is pushed into the queue, it will be either uploaded and therefore deleted from the database so that it will not be pushed into the queue again next time. Or it will stay in the queue if any failure of uploading happens.
2. All images should be uploaded to the cloud successfully. 
	Once one item is pushed into the queue, it will stay in the queue until it will be uploaded successfully.
3. Failed uploads should be retried. 
	Failed item will be pushed back into the queue for processing again.
4. Robustness – loss of network, recovery after exception, crash, etc. 
	If the application is crashed, the items will stay in a good state. Because the item has to be in three states: NotInQueue, InQueue, Deleted when crashing. The first two means the item has not been uploaded successfully and the item is still in the database. Once recovery, the item will be pushed into the queue for processing. So there will be no missing items for uploading once the application crashed. Deleted means the item has been successfully uploaded and deleted from the database. So there will be no duplicated items for uploading. 
5. Performance and threading consideration.
	Considering upload an item through a network could be time consuming and is an independant operation. The application will spawn 8 threads to upload in parallel. But the number of the threads should be configurable. Because it is using a lockfree, a producer(StubImageUploader) which does the database selection and multiple consumers(StubImageUploader) which do uploading can be done in parallel.
	
Things to improve:
1. Currently, SqliteImageFetcher will select all of the items at the time and push them into the queue, if there are a lot of items, the memory usage of the queue could be big. It is better to configure it so that it could select maximum N number into the queue. N should be configurable.
2. The application does not have recovery mechanism and it should be working as a daemon in Linux OS or Windows service in Windows. 
3. SqliteImageFetcher and SqliteImageFetcher are created explicitly. It is better to do this in a factory pattern so that creating them can be configurable.

Above the requirements:
It not only support Linux platform but also Windows.

Dependancies:
1. A Lockfree concurrentqueue from https://github.com/KjellKod/Moody-Camel-s-concurrentqueue/. They are head only.
2. Sqlite3 library.

How to build in Linux:
1. Install sqlite3 and sqlite-dev package. For Unbuntu, type "sudo apt install sqlite3 libsqlite3-dev".
2. Install g++. For Unbuntu, type "sudo apt install g++". g++ version support c++11. Version about 5.x should be fine. 4.8 or 4.9 could be fine as well.
2. Enter into GenVisUpload folder where the makefile is. Type make.
3. After building successfully, type "GenVisUpload .\ImageDB.db aws".

How to build in Windows:
1. Windows build is based on VisualStudio 2015. 
2. Install Vcpkg from https://github.com/Microsoft/vcpkg. It integrate a lot of open sources libaries to visual studio projects nicely including building and running.
3. Install the sqlite3 library using Vcpkg like ".\vcpkg install sqlite3:x64-windows"
4. Open the GenVisUpload.sln using VisualStudio 2015 and choose platform and configuration to build. 
5. After building successfully, type "GenVisUpload.exe .\ImageDB.db aws".
