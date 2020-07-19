
#include <cstdio>
#include "logicdata.h"

static CompositeEvent events[100];


void setEvent(CompositeEvent *events, int index,
		int timestamp, bool primaryTrigger, bool secondaryTrigger, bool trg, bool sync, bool coil, bool injector) {
	events[index].timestamp = timestamp;
	events[index].primaryTrigger = primaryTrigger;
}

int main(int argc, char **argv) {
	printf(".logicdata Sandbox 20200719\n");


	int index = 0;
    setEvent(events, index++, 10, false, false, false, false, false, false);
    setEvent(events, index++, 20, true, false, true, false, false, false);
    setEvent(events, index++, 30, false, false, false, false, false, false);
    setEvent(events, index++, 1000030, false, false, false, false, true, false);
    setEvent(events, index++, 2000030, false, false, true, false, false, true);

	writeFile(events, index);

	printf("Done!\n");
}
