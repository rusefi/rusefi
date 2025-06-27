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
#include <functional>

using after_header_callback_t = std::function<void()>;
using mlq_logline_callback_t = std::function<void(std::map<const std::string, float>& currentSnapshot)>;

class BinarySensorReader {
public:
  std::vector<Record*> records;
  std::map<std::string, const Record*> recordByName;
  std::map<const std::string, float> currentSnapshot;
  //std::vector<LogLine> logContent;
  int recordCounter = 0;

  after_header_callback_t afterHeaderCallback{};
  mlq_logline_callback_t callback{};

  void openMlq(const std::string fileName);
  void readMlq();
  void readBlocks(std::ifstream& ifs);
  void readLoggerFieldData(std::ifstream& ifs);

private:
  std::ifstream ifs;
  int readRecordsMetadata(std::ifstream &ifs, int numberOfFields);

};
