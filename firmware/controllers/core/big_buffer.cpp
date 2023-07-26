#include "pch.h"

#include "big_buffer.h"

static BigBufferUser s_currentUser;

// this buffer requires 4 byte alignment
// Some users place C++ objects in this memory, and that has certain alignment
// requirements.
static __attribute__((aligned(4))) uint8_t s_bigBuffer[BIG_BUFFER_SIZE];

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
	// swap contents of the two objects
	m_bufferPtr = other.m_bufferPtr;
	other.m_bufferPtr = nullptr;

	m_user = other.m_user;
	other.m_user = BigBufferUser::None;

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
