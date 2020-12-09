#pragma once

class ts_channel_s;

class TunerStudioBase {
public:
	int handleCrcCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize);
};
