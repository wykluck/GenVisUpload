CXX=g++
CXXFLAGS=-Wall -std=c++11 -pthread
OBJS=GenVisUpload.o SqliteDBWrapper.o SqliteImageFetcher.o StubImageUploader.o
LDFLAGS=-pthread
LIBS=-lsqlite3

GenVisUpload: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	
clean:
	rm -f *.o