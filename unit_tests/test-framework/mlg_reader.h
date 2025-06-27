/*
 * mlg_reader.h
 *
 */

#pragma once

#include <vector>
#include <map>
#include "MlgDataType.h"
#include "MlgRecord.h"
#include "LogLine.h"
#include <functional>

using after_header_callback_t = std::function<void()>;
using mlg_logline_callback_t = std::function<void(std::map<const std::string, float>& currentSnapshot)>;

class BinarySensorReader {
public:
  after_header_callback_t afterHeaderCallback{};

  void openMlg(const std::string fileName);
  void readMlg(mlg_logline_callback_t callback);
  std::map<const std::string, float>& readBlock();
  bool eof();

  ~BinarySensorReader() {
  for (auto record : records) {
    delete record;
  }
}
private:
  std::ifstream ifs;
  void readLoggerFieldData();
  int readRecordsMetadata(std::ifstream &ifs, int numberOfFields);

  std::vector<Record*> records;
  std::map<std::string, const Record*> recordByName;
  std::map<const std::string, float> currentSnapshot;
  //std::vector<LogLine> logContent;
  int recordCounter = 0;
};
