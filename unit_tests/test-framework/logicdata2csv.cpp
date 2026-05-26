#include "pch.h"
#include "logicdata.h"
#include <cstdio>
#include <iostream>
#include <string>

// Reverse of runCsv2LogicData: takes a .logicdata file and produces a .csv.
// TODO: implement parsing of the .logicdata binary format (see logicdata.cpp
// for the writer side). For now this is a stub that reports the intent so the
// command is wired up via unit_tests/main.cpp.
void runLogicData2Csv(const char* logicDataFileName) {
    std::string input(logicDataFileName);
    std::string output;
    size_t lastDot = input.find_last_of(".");
    if (lastDot == std::string::npos) {
        output = input + ".csv";
    } else {
        output = input.substr(0, lastDot) + ".csv";
    }

    std::cout << "logicdata2csv: input=" << input << " output=" << output << std::endl;
    std::cerr << "logicdata2csv: .logicdata parser is not implemented yet" << std::endl;
}
