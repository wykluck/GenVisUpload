import sqlite3
import time
import sys

def InsertRandomImageToDB(imageFilePath, restInterval, imageDBPath) :
	imageFile = open(imageFilePath, "rb")
	imageData = imageFile.read()
	imageFile.close()

	while True:
		dbConn = sqlite3.connect(imageDBPath)
		cur = dbConn.cursor()
		cur.execute("INSERT INTO Image (ImageContent) VALUES (?)", [memoryview(imageData)])
		dbConn.commit()
		time.sleep(restInterval)	


def main()	:
	if (len(sys.argv) != 3) :
		return -1;
	InsertRandomImageToDB(sys.argv[1], 1, sys.argv[2])

main()

