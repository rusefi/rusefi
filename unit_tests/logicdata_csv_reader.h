/*
 * @file logicdata_csv_reader.h
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


class CsvReader {
public:
	FILE *fp;
	char buffer[255];

	bool currentState[2];

	int lineIndex = -1;

	int * columnIndeces;

	void open(char *fileName, int * columnIndeces);
	bool haveMore();
	void processLine(EngineTestHelper *eth);
	void readLine(EngineTestHelper *eth);
};

