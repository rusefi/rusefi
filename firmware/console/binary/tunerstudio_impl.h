#pragma once

#include <cstdint>

class ts_channel_s;

class TunerStudioBase {
public:
	int handleCrcCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize);

protected:
	virtual void cmdOutputChannels(ts_channel_s *tsChannel, uint16_t offset, uint16_t count) = 0;
};

class TunerStudio : public TunerStudioBase {
public:
	TunerStudio(Logging* logger)
		: tsLogger(logger)
	{
	}

	void cmdOutputChannels(ts_channel_s *tsChannel, uint16_t offset, uint16_t count) override;

private:
	void sendErrorCode(ts_channel_s* tsChannel, uint8_t code);

	Logging* tsLogger;
};
