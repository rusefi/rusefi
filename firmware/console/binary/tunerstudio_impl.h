#pragma once

#include <cstdint>

struct TsChannelBase;

class TunerStudioBase {
public:
	int handleCrcCommand(TsChannelBase* tsChannel, char *data, int incomingPacketSize);

protected:
	virtual void cmdOutputChannels(TsChannelBase* tsChannel, uint16_t offset, uint16_t count) = 0;
};

class TunerStudio : public TunerStudioBase {
public:
	void cmdOutputChannels(TsChannelBase* tsChannel, uint16_t offset, uint16_t count) override;

private:
	void sendErrorCode(TsChannelBase* tsChannel, uint8_t code);
};
