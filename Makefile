CC=g++
SOURCE_DIR=./Source/
BUILD_DIR=./Source/Build/

club: Time.o Table.o Club.o Parser.o
	$(CC) $(BUILD_DIR)Club.o $(BUILD_DIR)Parser.o $(BUILD_DIR)Table.o $(BUILD_DIR)Time.o $(SOURCE_DIR)main.cpp -o club

Parser.o: $(SOURCE_DIR)Parser.cpp
	$(CC) -c $^ -o $(BUILD_DIR)$@

Club.o: $(SOURCE_DIR)Club.cpp
	$(CC) -c $^ -o $(BUILD_DIR)$@
	
Table.o: $(SOURCE_DIR)Table.cpp
	$(CC) -c $^ -o $(BUILD_DIR)$@

Time.o: $(SOURCE_DIR)Time.cpp
	$(CC) -c $^ -o $(BUILD_DIR)$@

clear:
	rm -rf club
	rm $(BUILD_DIR)*

