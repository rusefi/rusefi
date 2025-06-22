/*
 * mlq_reader.h
 *
 */

#pragma once

#include <vector>
#include <map>
#include "MlqDataType.h"
#include "MlqRecord.h"
#include "LogLine.h"

class BinarySensorReader {
public:
  std::vector<Record> records;
  std::map<const Record*, float> snapshot;
  std::vector<LogLine> logContent;
  int recordCounter = 0;

  void readMlq(const std::string fileName);
  void readBlocks(std::ifstream& ifs);
  void readLoggerFieldData(std::ifstream& ifs);

private:
  int readRecordsMetadata(std::ifstream &ifs, int numberOfFields);

};

