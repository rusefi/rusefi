package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

/**
 * Support for Saleae .logicdata format.
 * (c) andreika 2020
 * <p>
 * Jun 7 status: this code mostly works but it needs more testing
 *
 * @see LogicdataStreamFileSandbox
 */
public class LogicdataStreamFile extends StreamFile {

    private static final int frequency = 1000000;

    private static final int frequencyDivided = frequency / 10;

	private static final char magic = 0x7f;
	private static final char version = 0x13;
	private static final String title = "Data save2";

	private static final int BLOCK = 0x18;
	private static final int CHANNEL_BLOCK = 0x16;
	private static final int SUB = 0x54;

	private static final int FLAG_NOTEMPTY = 2;
	private static final int FLAG_NOTEMPTY_LONG = 3;
	private static final int FLAG_EMPTY = 5;

	private static final long RECORD_FIRST = 1;
	private static final long RECORD_NEXT = 2;

	//looks these magic numbers are version-specific
	private static final int LOGIC4 = 0x40FD;
	private static final int LOGIC8 = 0x673B;
	private static final int LOGIC1 = 0x07BD;
	private static final int LOGIC16 = 0x533f;

	private static final int [] CHANNEL_FLAGS = {
			0x13458b, 0x0000ff, 0x00a0f9, 0x00ffff, 0x00ff00, 0xff0000, 0xf020a0, 0x000000,
	};

	private static final long SIGN_FLAG = 0x80000000L;

    private static final int numChannels = 16;
    private static final int reservedDurationInSamples = 10;
    private static int realDurationInSamples = 0;
    private static int scaledDurationInSamples = 0;

	private final String fileName;
	private final List<CompositeEvent> eventsBuffer = new ArrayList<>();

	private static final String [] channelNames = {
			"Primary", "Secondary", "Trg", "Sync", "Coil", "Injector", "Channel 6", "Channel 7",
			"Channel 8", "Channel 9", "Channel 10", "Channel 11", "Channel 12", "Channel 13", "Channel 14", "Channel 15"
	};


	public LogicdataStreamFile(String fileName) {
		this.fileName = fileName;
	}

	/*
        public static void writeLogicdata(List<CompositeEvent> events, OutputStream stream) throws IOException {
            LogicdataStreamFile saver = new LogicdataStreamFile();
            saver.stream = stream;
            saver.writeHeader();
            saver.appendEvents(events);
            saver.close();
        }
    */
    @Override
    public void append(List<CompositeEvent> events) {
        try {
            if (stream == null) {
                stream = new LogicdataOutputStream(Files.newOutputStream(Paths.get(fileName)));
                writeHeader();
            }
            eventsBuffer.addAll(events);
        } catch (IOException e) {
            // ignoring this one
        }
    }

	/**
	 * this file format is not streaming, we have to write everything at once
	 */
	private void writeEvents(List<CompositeEvent> events) throws IOException {
    	// we need at least 2 records
    	if (events == null || events.size() < 2)
    		return;
		long firstRecordTs = events.get(0).getTimestamp();
		long lastRecordTs = events.get(events.size() - 1).getTimestamp();
	    // we don't know the total duration, so we create a margin after the last record which equals to the duration of the first event
		// TODO: why do we jump from timestamps to samples?
	    realDurationInSamples = (int)(lastRecordTs + firstRecordTs);
    	scaledDurationInSamples = realDurationInSamples / 4;

		writeChannelDataHeader();

		// the initial state should have zero timestamp
		if (events.get(0).getTimestamp() > 0)
			events.add(0, new CompositeEvent(0, false, false, false, false, false, false));

    	// we need to split the combined events into separate channels
    	for (int ch = 0; ch < numChannels; ch++) {
			boolean useLongDeltas = false;
			List<Long> chDeltas = new ArrayList<>();
			int chPrevState = -1, chInitialState = -1;
			long prevTs = -1, initialTs = 0;
        	for (CompositeEvent event : events) {
        		int chState = getChannelState(ch, event);
        		long ts = event.getTimestamp();

        		if (chPrevState == -1) {
        			chPrevState = chState;
        		}
				if (prevTs == -1) {
					prevTs = ts;
				}
				if (chInitialState == -1) {
					chInitialState = chState;
					initialTs = ts;
				}
				if (chState != chPrevState) {
					long delta = ts - prevTs;
					if (delta > 0x7fff) {
						useLongDeltas = true;
					}
					// encode state
					if (chState == 0)
						delta |= SIGN_FLAG;

					chDeltas.add(delta);

					prevTs = ts;
					chPrevState = chState;
				}
			}

			if (useLongDeltas) {
				System.out.println("using long deltas for ch #" + ch);
			}

	    	// TODO: why do we pass a timestamp as a record index?
			writeChannelData(ch, chDeltas, chPrevState, (int)prevTs, chInitialState, (int)initialTs, useLongDeltas);
        }

        writeChannelDataFooter();
        stream.flush();
    }

    private int getChannelState(int ch, CompositeEvent event) {
		switch (ch) {
			case 0:
				return event.isPrimaryTriggerAsInt();
			case 1:
				return event.isSecondaryTriggerAsInt();
			case 2:
				return event.isTrgAsInt();
			case 3:
				return event.isSyncAsInt();
		}
		if (ch >= 4 && ch < 4 + 6) {
			return event.isCoil(ch - 4);
		}
		if (ch >= 10 && ch < 10 + 6) {
            return event.isInjector(ch - 10);
		}
		return -1;
	}

    ////////////////////////////////////////////////////////////////////

    private void writeHeader() throws IOException {
        writeByte(magic);

		stream.writeVarLength(version);
		stream.writeString(title);
        stream.flush();

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(SUB);
		stream.writeVarLength(frequency);
		stream.writeVarLength(0);
		stream.writeVarLength(reservedDurationInSamples);
		stream.writeVarLength(frequencyDivided);
		write(0, 2);
		stream.writeVarLength(numChannels);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
		}
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);

		writeId(7, 7);
		stream.writeVarLength(SUB);
		stream.writeVarLength(0);
	}

    private void writeChannelHeader(int ch) throws IOException {
		stream.writeVarLength(0xff);
		stream.writeVarLength(ch);
		stream.writeString(channelNames[ch]);
		write(0, 2);
		stream.writeDouble(1.0);

		stream.writeVarLength(0);

		stream.writeDouble(0.0);
		stream.writeVarLength(1);	// or 2
		stream.writeDouble(0.0);	// or 1.0

		// this part sounds like the 'next' pointer?
		if (ch == numChannels - 1) {
			stream.writeVarLength(0);
		} else {
			writeId(1 + ch, 1);
			for (int i = 0; i < 3; i++) {
				stream.writeVarLength((CHANNEL_FLAGS[ch & 7] >> (i * 8)) & 0xff);
			}
		}
    }

    private void writeChannelDataHeader() throws IOException {
		stream.writeVarLength(BLOCK);
		stream.writeVarLength(scaledDurationInSamples);
		stream.writeVarLength(0);
		write(0, 4);
		stream.writeVarLength(numChannels);
		write(0, 3);
		writeId(0, 1);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		write(0, 3);

		for (int i = 0; i < numChannels; i++) {
			writeChannelHeader(i);
		}

		stream.writeVarLength(BLOCK);
		write(new int[]{ SUB, SUB, 0, SUB, 0, SUB });
		write(0, 6);

		stream.writeVarLength(BLOCK);
		write(0, 2);
		stream.writeVarLength(reservedDurationInSamples);
		stream.writeVarLength(0);
		stream.writeVarLength(SUB);
		stream.writeVarLength(reservedDurationInSamples);
		stream.writeVarLength(frequencyDivided);
		write(0, 2);
		stream.writeVarLength(SUB);
		write(0, 2);
		stream.writeVarLength(1);
		write(0, 3);
		//writeId(0, 0);
		writeId(7, 0);

		stream.writeVarLength(BLOCK);
		write(new int[]{ (int)realDurationInSamples, (int)realDurationInSamples, (int)realDurationInSamples });
		stream.writeVarLength(0);
		stream.writeVarLength(SUB);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		write(SUB, 4);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(frequency);
		write(0, 3);
		stream.writeVarLength(1);
		write(0, 3);
		writeId(0, 0);
		write(new int[]{ 0, 1, 1, 0, 1, 0x13 });
		stream.writeVarLength(SUB);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(0);
		stream.writeVarLength(realDurationInSamples);
		write(0, 2);
		stream.writeVarLength(numChannels);
		write(new int[]{ 1, 0, 1 });
	}

	private void writeChannelData(int ch, List<Long> chDeltas, int chLastState, int lastRecord, int chInitialState, int initialRecord, boolean useLongDeltas) throws IOException {
    	int numEdges = chDeltas.size();
    	if (numEdges == 0) {
			initialRecord = 0;
			lastRecord = 0;
		}
		stream.writeVarLength(CHANNEL_BLOCK);
		// channel#0 is somehow special...
    	if (ch == 0) {
			stream.writeVarLength(SUB);
			stream.writeVarLength(BLOCK);
		}

		stream.writeVarLength(ch + 1);
		stream.writeVarLength(0);
		stream.writeVarLength(realDurationInSamples);
		stream.writeVarLength(1);
		stream.writeVarLength(lastRecord);

		// todo: why do we convert from
		int numSamplesLeft = (int)(realDurationInSamples - lastRecord);
		stream.writeVarLength(numSamplesLeft);

		stream.writeVarLength(chLastState);

		int chFlag = (numEdges == 0) ? FLAG_EMPTY : (useLongDeltas ? FLAG_NOTEMPTY_LONG : FLAG_NOTEMPTY);
		stream.writeVarLength(chFlag);

		if (ch == 0) {
			stream.writeVarLength(0);
			stream.writeVarLength(BLOCK);
			write(0, 11);
			if (useLongDeltas) {
				stream.writeVarLength(BLOCK);
				write(0, 6);
			}
			stream.writeVarLength(BLOCK);
		} else {
			write(0, 10);
			if (useLongDeltas) {
				write(0, 5);
			}
		}

		stream.writeVarLength(numEdges);
		stream.writeVarLength(0);
		stream.writeVarLength(numEdges);
		// this fixes "32k records limit"
		stream.writeVarLength(numEdges >> 15);
		stream.writeVarLength(numEdges & 0x7fff);

		writeEdges(chDeltas, useLongDeltas);

		if (ch == 0) {
			stream.writeVarLength(BLOCK);
			write(0, 6);
			if (!useLongDeltas) {
				stream.writeVarLength(BLOCK);
				write(0, 6);
			}
			stream.writeVarLength(BLOCK);
		} else {
			write(0, 4);
			if (!useLongDeltas) {
				write(0, 5);
			}
		}

		if (numEdges == 0) {
			write(0, 5);
			return;
		}

		int numRecords = 1;
		stream.writeVarLength(numRecords);
		stream.writeVarLength(0);
		stream.writeVarLength(numRecords);
		stream.writeVarLength(0);
		stream.writeVarLength(numRecords);
		writeRecord(0, 0, -1, chFlag);
    }

	private void writeEdges(List<Long> chDeltas, boolean useLongDeltas) throws IOException {
		for (long d : chDeltas) {
			// set 16-bit 'sign' flag
			if (!useLongDeltas && (d & SIGN_FLAG) == SIGN_FLAG)
				d = (d & 0x7fff) | (SIGN_FLAG >> 16);
			writeByte((byte)(d & 0xff));
			writeByte((byte)((d >> 8) & 0xff));
			if (useLongDeltas) {
				writeByte((byte)((d >> 16) & 0xff));
				writeByte((byte)((d >> 24) & 0xff));
			}
		}
		writeByte(0x00);
	}

	private void writeByte(int i) throws IOException {
		stream.writeByte(i);
	}

	private void writeWord(int d) throws IOException {
		writeByte((byte)(d & 0xff));
		writeByte((byte)((d >> 8) & 0xff));
	}
	private void writeQword(long l) throws IOException {
		for (int i = 7; i >= 0; i--) {
			writeByte((byte)(l & 0xFF));
			l >>= 8;
		}
	}

	private void writeRecord(long record, long idx, long prevIdx, long type) throws IOException {
		writeQword(record);
		writeQword(idx);
		writeQword(prevIdx);
		writeQword(type);
	}

	private void writeChannelDataFooter() throws IOException {
    	write(0, 3);
		stream.writeVarLength(1);
		stream.writeVarLength(1);
		stream.writeVarLength(0);
		stream.writeVarLength(numChannels);
	}

    @Override
	protected void writeFooter() throws IOException {
		if (stream == null)
			return;
		System.out.println("Writing " + eventsBuffer.size() + " event(s)");
		writeEvents(eventsBuffer);
		stream.writeVarLength(BLOCK);
		for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
        }
		stream.writeVarLength(1);
		writeId(numChannels, BLOCK);
        for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
        }
		stream.writeVarLength(1);
		stream.writeVarLength(0);
		stream.writeVarLength(frequency);
		write(0, 16);
		stream.writeVarLength(0x01);
		// ???
		stream.writeVarLength(0x23);
		stream.writeVarLength(SUB);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(numChannels + 1);
		stream.writeVarLength(0);
		stream.writeVarLength(0xFFFFFFFFFFFFFFFFL);
		stream.writeVarLength(0xFFFFFFFFL);
		stream.writeVarLength(1);
		write(0, 3);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(0);

		stream.writeVarLength(BLOCK);
		stream.writeVarLength(0);
		stream.writeDouble(1.0);
		stream.writeVarLength(SUB);
		write(0, 6);
		stream.writeVarLength(1);
		write(0, 4);

		stream.writeVarLength(1);
		// ???
		stream.writeVarLength(0x29);
		stream.writeVarLength(SUB);

		writeTimingMarker();

        stream.flush();
		System.out.println("writeFooter " + fileName);
    }

    private void writeTimingMarker() throws IOException {
		stream.writeVarLength(BLOCK);
		stream.writeVarLength(numChannels + 2);
		write(0, 4);
    	stream.writeString("Timing Marker Pair");
    	stream.writeString("A1");
    	stream.writeString("A2");
    	write(0, 2);
		stream.writeVarLength(SUB);
		write(0, 9);
    }

    private void writeId(int i1, int i2) throws IOException {
		stream.writeVarLength(LOGIC16);
		stream.writeVarLength(i1);
		stream.writeVarLength(i2);
	}

    ///////////////////////////////////////////////////////////////////////

    private void write(int value, int num) throws IOException {
    	for (int i = 0; i < num; i++)
			stream.writeVarLength(value);
    }

	private void write(int[] values) throws IOException {
		for (int value : values)
			stream.writeVarLength(value);
	}

	private void writeRaw(int value, int num) throws IOException {
    	for (int i = 0; i < num; i++) {
    		writeByte(value);
    	}
    }
}
