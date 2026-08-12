/*
 * @file logicdata_csv_reader.h
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

const int NORMAL_ORDER[2] = {0, 1};

const int REVERSE_ORDER[2] = {1, 0};

class CsvReader {
public:
	CsvReader(size_t triggerCount, size_t vvtCount) : CsvReader(triggerCount, vvtCount, 0.0) {}
	CsvReader(size_t triggerCount, size_t vvtCount, double timestampOffset)
		: m_triggerCount(triggerCount)
		, m_vvtCount(vvtCount)
		, m_timestampOffset(timestampOffset)
	{
	}
	~CsvReader();

  /* when reading two cam channels it's either on intake one exhaust or two intakes on different banks */
	bool twoBanksSingleCamMode = true;

	void open(const char *fileName, const int* triggerColumnIndeces = NORMAL_ORDER, const int *vvtColumnIndeces = NORMAL_ORDER);
	bool haveMore();
	void processLine(EngineTestHelper *eth);
	void readLine(EngineTestHelper *eth);
	double readTimestampAndValues(double *v);

	bool flipOnRead = false;
	bool flipVvtOnRead = false;
	// a way to skip some columns
	int readingOffset = 0;
	// Index (0-based) of the column holding the timestamp. The default value of
	// -1 keeps the legacy behavior where the very first column is the timestamp
	// and the trigger/vvt columns follow it (after 'readingOffset' columns).
	// Some recordings (e.g. rusEFI internal CSV logs) keep the timestamp in a
	// later column while the trigger channels come first - set this index to
	// support that layout.
	int timestampColumnIndex = -1;
	// Multiplier applied to the raw timestamp value to convert it into seconds.
	// Logic-analyzer captures store seconds (scale 1.0); rusEFI internal CSV
	// logs store milliseconds, so use 0.001 for those.
	double timestampScale = 1.0;
	// When true the timestamp of the very first processed data row is subtracted
	// from all timestamps so that replay starts near zero. This is required for
	// recordings captured after a long ECU uptime: the unit-test mock clock is
	// 32-bit and large absolute timestamps overflow during us->NT conversion.
	bool normalizeTimestamps = false;
	double lastTimeStamp = 0.0;

	int lineIndex() const {
		return m_lineIndex;
	}

  cyclic_buffer<double, 720> history;

private:
	const size_t m_triggerCount;
	const size_t m_vvtCount;
	const double m_timestampOffset;

	FILE *fp = nullptr;
	char buffer[255];

	bool currentState[TRIGGER_INPUT_PIN_COUNT] = {0, 0};
	bool currentVvtState[CAM_INPUTS_COUNT] = {0, 0};

	int m_lineIndex = -1;

	// base timestamp (in seconds) subtracted from all rows when normalizeTimestamps is set
	double m_timestampBase = 0.0;
	bool m_haveTimestampBase = false;

	const int* triggerColumnIndeces;
	const int* vvtColumnIndeces;
};

