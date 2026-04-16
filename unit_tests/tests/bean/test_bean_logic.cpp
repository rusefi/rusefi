#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(bean, mr2_cluster) {
	CsvReader reader(1, /* vvtCount */ 0);
	reader.open("tests/bean/resources/mr2_cluster_bean_bench1.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}
}
