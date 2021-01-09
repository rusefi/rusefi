
#include "big_buffer.h"

#include <cstddef>

static BigBufferUser s_currentUser;
static uint8_t s_bigBuffer[BIG_BUFFER_SIZE];

void releaseBuffer(void* bufferPtr, BigBufferUser user) {
	if (bufferPtr != &s_bigBuffer || user != s_currentUser) {
		// todo: panic!
	}

	s_currentUser = BigBufferUser::None;
}

BigBufferHandle::BigBufferHandle(void* buffer, BigBufferUser user)
	: m_bufferPtr(buffer)
	, m_user(user)
{
}

BigBufferHandle::~BigBufferHandle()
{
	if (m_bufferPtr) {
		releaseBuffer(m_bufferPtr, m_user);
	}
}


