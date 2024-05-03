/**
 * @file	scheduler.h
 *
 * @date October 1, 2020
 */
#include "pch.h"

#include "scheduler.h"

void action_s::execute() {
	efiAssertVoid(ObdCode::CUSTOM_ERR_ASSERT, m_callback != NULL, "callback==null1");
	m_callback(m_param);
}

schfunc_t action_s::getCallback() const {
	return m_callback;
}

void * action_s::getArgument() const {
	return m_param;
}

#if EFI_UNIT_TEST
efitick_t scheduling_s::getMomentUs() const {
	return NT2US(momentX);
}
#endif
