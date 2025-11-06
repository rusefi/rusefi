#include "pch.h"
#include "mlg_reader.h"

using ::testing::_;
using ::testing::StrictMock;

constexpr float executor_dt = FAST_CALLBACK_PERIOD_MS * 0.001f;
static constexpr bool verbose = false;

static bool firstRun = true;
static float prevTime = 0;
static float dt = 0 ;

static void my_log_handler(std::map<const std::string, float>& snapshot) {
	//static size_t counter = 0;
	//std::cout << "Lambda callback received snapshot. It has " << snapshot.size()
	//		<< " entries.\n";

	float time = snapshot["Time"];

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

		if (verbose) {
			if (counter) {
				std::cout << time << ": periodicFastCallback() executed " << counter << " times\n";
			}

			printf("%.3f: CLT: %3.0f, RPM %3.0f, MAP %3.0f, Lambdas: %f %f\n",
				time,
				snapshot["CLT"],
				snapshot["RPM"],
				snapshot["MAP"],
				snapshot["Front Lambda"],
				snapshot["Rear Lambda"]);
		}
	}

	// TODO: find way to mock target AFR?

	// now update sensors with new snapshot
	Sensor::setMockValue(SensorType::Clt, snapshot["CLT"]);
	Sensor::setMockValue(SensorType::Lambda1, snapshot["Front Lambda"]);
	Sensor::setMockValue(SensorType::Lambda2, snapshot["Rear Lambda"]);
	Sensor::setMockValue(SensorType::Rpm, snapshot["RPM"]);
	Sensor::setMockValue(SensorType::Map, snapshot["MAP"]);

	prevTime = time;
	firstRun = false;
}

void runLtftSandbox() {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BinarySensorReader reader;

	reader.openMlg("pretty-happy-reference.mlg");

	reader.readMlg(my_log_handler);

	printf("LTFT test: miss: %d, hit %d, deadband %d\n",
		engine->module<LongTermFuelTrim>()->ltftCntMiss,
		engine->module<LongTermFuelTrim>()->ltftCntHit,
		engine->module<LongTermFuelTrim>()->ltftCntDeadband);
}
