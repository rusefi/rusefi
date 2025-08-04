// file engine_csv_reader.h

#pragma once

#include "logicdata_csv_reader.h"

class EngineCsvReader {
public:
  EngineCsvReader(size_t triggerCount, size_t vvtCount) : cvsReader(triggerCount, vvtCount) {
  }

  bool gotRpm = false;
  bool gotSync = false;
  bool prevSync = false;

  int expectedFirstRpm = -1;
  int expectedFirstRpmAtIndex = -1;

  // composition good, inheritance less good?
  CsvReader cvsReader;

  	void open(const char *fileName, const int* triggerColumnIndexes = NORMAL_ORDER, const int *vvtColumnIndexes = NORMAL_ORDER) {
  	  cvsReader.open(fileName, triggerColumnIndexes, vvtColumnIndexes);
  	}
  	bool haveMore() {
  	  return cvsReader.haveMore();
  	}

  	int lineIndex() const {
  	  return cvsReader.lineIndex();
  	}

  	void setReadingOffset(int offset) {
  	  cvsReader.readingOffset = offset;
  	}

  	void setFlipOnRead(bool v) {
  	  cvsReader.flipOnRead = v;
  	}
    void setFlipVvtOnRead(bool v) {
      cvsReader.flipVvtOnRead = v;
    }

  void processLine(EngineTestHelper *eth) {
    cvsReader.processLine(eth);
  }

  void assertFirstRpm(int expectedFirstRpm, int expectedFirstRpmAtIndex) {
    auto rpm = Sensor::getOrZero(SensorType::Rpm);

		if (!gotRpm && rpm) {
			gotRpm = true;

  			EXPECT_NEAR(rpm, expectedFirstRpm, 1);
		  	EXPECT_EQ(lineIndex(), expectedFirstRpmAtIndex);
		}
  }

};
