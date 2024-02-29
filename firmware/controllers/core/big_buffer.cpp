#include "pch.h"

#include "big_buffer.h"

static BigBufferUser s_currentUser;
// uint32_t type to get 4-byte alignment
// alignment is required since we sometimes allocate objects in the buffer (like Timer of CompositeBuffer)
// we've only observed issue on F7 in -Os compiler configuration but technically all processors care
static uint32_t s_bigBuffer[BIG_BUFFER_SIZE / sizeof(uint32_t)];

#if EFI_UNIT_TEST
BigBufferUser getBigBufferCurrentUser() {
  return s_currentUser;
}
#endif // EFI_UNIT_TEST

static void releaseBuffer(void* bufferPtr, BigBufferUser user) {
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

BigBufferHandle::BigBufferHandle(BigBufferHandle&& other) {
	// swap contents of the two objects
	m_bufferPtr = other.m_bufferPtr;
	other.m_bufferPtr = nullptr;

	m_user = other.m_user;
	other.m_user = BigBufferUser::None;
}

BigBufferHandle& BigBufferHandle::operator= (BigBufferHandle&& other) {
	if (this != &other) {
		if (m_bufferPtr) {
			releaseBuffer(m_bufferPtr, m_user);
		}
		// swap contents of the two objects
		m_bufferPtr = other.m_bufferPtr;
		other.m_bufferPtr = nullptr;

		m_user = other.m_user;
		other.m_user = BigBufferUser::None;
	}
	return *this;
}

BigBufferHandle::~BigBufferHandle() {
	if (m_bufferPtr) {
		releaseBuffer(m_bufferPtr, m_user);
	}
}

BigBufferHandle getBigBuffer(BigBufferUser user) {
	if (s_currentUser != BigBufferUser::None) {
		// fatal
		return {};
	}

	s_currentUser = user;

	return BigBufferHandle(s_bigBuffer, user);
}
