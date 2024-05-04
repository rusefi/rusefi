// This file handles the "big buffer" - a shared buffer that can be used by multiple users depending on which function is enabled

#pragma once

#ifndef BIG_BUFFER_SIZE
#define BIG_BUFFER_SIZE 8192
#endif

enum class BigBufferUser {
	None,
	ToothLogger,
	PerfTrace,
	TriggerScope,
	KnockSpectrogram,
};

class BigBufferHandle {
public:
	BigBufferHandle() = default;
	BigBufferHandle(void* buffer, BigBufferUser user);
	~BigBufferHandle();

	// But allow moving (passing ownership of the buffer)
	BigBufferHandle(BigBufferHandle&& other);
	BigBufferHandle& operator=(BigBufferHandle&&);

	// Implicit conversion operator to bool, so you can do things like if (myBuffer) { ... } as if it was a raw pointer
	constexpr explicit operator bool() const {
		return m_bufferPtr != nullptr;
	}

	template <class TBuffer>
	const TBuffer* get() const {
		return reinterpret_cast<TBuffer*>(m_bufferPtr);
	}

	template <class TBuffer>
	TBuffer* get() {
		return reinterpret_cast<TBuffer*>(m_bufferPtr);
	}

	size_t size() const {
		return BIG_BUFFER_SIZE;
	}

private:
	void* m_bufferPtr = nullptr;
	BigBufferUser m_user = BigBufferUser::None;
};

BigBufferHandle getBigBuffer(BigBufferUser user);
