#include "pch.h"
#include "mlq_reader.h"

using ::testing::_;
using ::testing::StrictMock;

//static int counter = 0;

constexpr float executor_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;

static BinarySensorReader reader;

const Record *timeRecord;
const Record *rpmRecord;
const Record *cltRecord;
const Record *mapRecord;
const Record *lambda1Record, *lambda2Record;

EngineTestHelper eth(engine_type_e::TEST_ENGINE);

static bool firstRun = true;
static float prevTime = 0;
static float dt = 0 ;

static void afterHeader() {
	timeRecord = reader.recordByName["Time"];
	rpmRecord = reader.recordByName["RPM"];
	cltRecord = reader.recordByName["CLT"];
	lambda1Record = reader.recordByName["Front Lambda"];
	lambda2Record = reader.recordByName["Rear Lambda"];
	rpmRecord = reader.recordByName["RPM"];
	mapRecord = reader.recordByName["MAP"];

	//for (const auto& element : reader.recordByName)
	//	std::cout << element.first << "\n";

	firstRun = true;
	prevTime = 0;
	dt = 0 ;
}

static void my_log_handler(std::map<const Record*, float> &snapshot) {
	//static size_t counter = 0;
	//std::cout << "Lambda callback received snapshot. It has " << snapshot.size()
	//		<< " entries.\n";
	
	float time = snapshot[timeRecord];

	if (!firstRun) {
		dt += time - prevTime;

		size_t counter = 0;
		while (dt > executor_dt) {
			// TODO: adjust time
			// run the ignition math
			engine->periodicFastCallback();
			dt -= executor_dt;
			counter++;
		}
		if (counter) {
			std::cout << time << ": periodicFastCallback() executed " << counter << " times\n";
		}

		printf("%.3f: CLT: %3.0f, RPM %3.0f, MAP %3.0f, Lambdas: %f %f\n",
			time,
			snapshot[cltRecord],
			snapshot[rpmRecord],
			snapshot[mapRecord],
			snapshot[lambda1Record],
			snapshot[lambda2Record]);
	}

	// TODO: find way to mock target AFR?

	// now update sensors with new snapshot
	Sensor::setMockValue(SensorType::Clt, snapshot[cltRecord]);
	Sensor::setMockValue(SensorType::Lambda1, snapshot[lambda1Record]);
	Sensor::setMockValue(SensorType::Lambda2, snapshot[lambda2Record]);
	Sensor::setMockValue(SensorType::Rpm, snapshot[rpmRecord]);
	Sensor::setMockValue(SensorType::Map, snapshot[mapRecord]);

	prevTime = time;
	firstRun = false;
}

void runSandbox() {
	reader.afterHeaderCallback = afterHeader;

	reader.callback = my_log_handler;

	reader.openMlq("pretty-happy-reference.mlg");

	reader.readMlq();

	printf("LTFT test: miss: %d, hit %d, deadband %d\n",
		engine->module<LongTermFuelTrim>()->ltftCntMiss,
		engine->module<LongTermFuelTrim>()->ltftCntHit,
		engine->module<LongTermFuelTrim>()->ltftCntDeadband);
}
