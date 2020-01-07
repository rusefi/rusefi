

#include "scheduling_batch.h"

SchedulingBatch::~SchedulingBatch()
{
	auto current = m_head;

	while(current)
	{
		auto x = current;
		current = current->nextScheduling_s;

		m_exec->
	}
}