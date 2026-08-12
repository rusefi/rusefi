#include "pch.h"
#include "logicdata.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

static char* trim(char *str) {
	while (str != nullptr && str[0] == ' ') {
		str++;
	}
	if (str != nullptr) {
		char *end = str + strlen(str) - 1;
		while (end >= str && (*end == ' ' || *end == '\r' || *end == '\n')) {
			*end = '\0';
			end--;
		}
	}
	return str;
}

void runCsv2LogicData(const char* csvFileName) {
    std::string input(csvFileName);
    std::string output;
    size_t lastDot = input.find_last_of(".");
    if (lastDot == std::string::npos) {
        output = input + ".logicdata";
    } else {
        output = input.substr(0, lastDot) + ".logicdata";
    }

    std::ifstream file(csvFileName);
    if (!file.is_open()) {
        std::cerr << "Could not open " << csvFileName << std::endl;
        return;
    }

    std::vector<CompositeEvent> events;
    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        if (line.empty()) continue;

        char* buf = strdup(line.c_str());
        char* token = strtok(buf, ",");
        if (!token) {
            free(buf);
            continue;
        }

        CompositeEvent event;
        double timestamp = std::atof(trim(token));
        event.timestamp = (uint32_t)(timestamp * 1000000);

        auto readNextBool = [&]() -> bool {
            token = strtok(nullptr, ",");
            if (!token) return false;
            char* t = trim(token);
            return t[0] == '1';
        };

        event.primaryTrigger = readNextBool();
        event.secondaryTrigger = readNextBool();
        // The CSV columns are: Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET
        // logicdata.h CompositeEvent has:
        // uint32_t timestamp;
        // bool primaryTrigger;
        // bool secondaryTrigger;
        // bool isTDC;
        // bool sync;
        // bool coil;
        // bool injector;

        // Skip Cam 2, 3, 4
        strtok(nullptr, ","); // Cam 2
        strtok(nullptr, ","); // Cam 3
        strtok(nullptr, ","); // Cam 4

        event.sync = readNextBool();
        event.isTDC = readNextBool();
        event.coil = readNextBool();
        event.injector = readNextBool();

        events.push_back(event);
        free(buf);
    }

    if (events.empty()) {
        std::cerr << "No events found in CSV" << std::endl;
        return;
    }

    std::cout << "Writing " << events.size() << " events to " << output << std::endl;
    writeLogicDataFile(output.c_str(), events);
}
