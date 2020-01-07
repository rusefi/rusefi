
#include "scheduler.h"

class SchedulingBatch
{
public:
	SchedulingBatch(ExecutorInterface* exec);
	~SchedulingBatch();

	void scheduleForLater(int delayUs, schfunc_t callback, void* param);

private:
	scheduling_s* m_head;
};
