#include "pch.h"
#include "mlq_reader.h"

//static int counter = 0;

static BinarySensorReader reader;

const Record *timeRecord;
const Record *rpmRecord;
const Record *cltRecord;

static void afterHeader() {
	timeRecord = reader.recordByName["Time"];
	rpmRecord = reader.recordByName["RPM"];
	cltRecord = reader.recordByName["CLT"];
}

static void my_log_handler(std::map<const Record*, float> &snapshot) {
//	std::cout << "Lambda callback received snapshot. It has " << snapshot.size()
//			<< " entries.\n";

	float time = snapshot[timeRecord];
	float rpm = snapshot[rpmRecord];
	float clt = snapshot[cltRecord];

//	std::cout << counter++ << " CLT " << clt << "\n";
}

void runSandbox() {
	reader.afterHeaderCallback = afterHeader;

	reader.callback = my_log_handler;

	reader.readMlq("pretty-happy-reference.mlg");
}
