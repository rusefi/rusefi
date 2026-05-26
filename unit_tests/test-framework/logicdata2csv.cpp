#include "pch.h"
#include "logicdata.h"
#include "logicdata_reader.h"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// Reverse of runCsv2LogicData: takes a .logicdata file and produces a .csv.
// CSV columns mirror those consumed by runCsv2LogicData:
//   Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors
// The writer only knows 6 channels (Primary, Secondary, TDC, Sync, Coil,
// Injector), so we map them as:
//   Cam 1 = Secondary, Cam 2..4 = 0
void runLogicData2Csv(const char* logicDataFileName) {
    std::string input(logicDataFileName);
    std::string output;
    size_t lastDot = input.find_last_of(".");
    if (lastDot == std::string::npos) {
        output = input + ".csv";
    } else {
        output = input.substr(0, lastDot) + ".csv";
    }

    std::vector<CompositeEvent> events;
    try {
        events = readLogicDataFile(logicDataFileName);
    } catch (const std::exception& e) {
        std::cerr << "logicdata2csv: failed to read " << logicDataFileName
                  << ": " << e.what() << std::endl;
        return;
    }

    std::ofstream out(output);
    if (!out.is_open()) {
        std::cerr << "logicdata2csv: cannot open " << output << " for writing" << std::endl;
        return;
    }

    out << "Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors\n";
    out << std::fixed << std::setprecision(15);
    for (const auto& ev : events) {
        double t = ev.timestamp / 1000000.0;
        out << t << ", "
            << (ev.primaryTrigger ? 1 : 0) << ", "
            << (ev.secondaryTrigger ? 1 : 0) << ", "
            << 0 << ", " << 0 << ", " << 0 << ", "
            << (ev.sync ? 1 : 0) << ", "
            << (ev.isTDC ? 1 : 0) << ", "
            << (ev.coil ? 1 : 0) << ", "
            << (ev.injector ? 1 : 0) << "\n";
    }

    std::cout << "logicdata2csv: wrote " << events.size() << " events to "
              << output << std::endl;
}
