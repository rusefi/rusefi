package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/**
 * Support for Saleae .logicdata format.
 * (c) andreika 2020
 */
public class LogicdataStreamFile extends StreamFile {

    private static final int frequency = 1000000;
    private static final int frequencyDiv = 10;

	private static final char magic = 0x7f;
	private static final String title = "Data save2";

	private static final int BLOCK = 0x15;
	private static final int CHANNEL_BLOCK = 0x16;
	private static final int SUB = 0x54;

	private static final int FLAG_NOTEMPTY = 2;
	private static final int FLAG_EMPTY = 5;

	private static final int LOGIC4 = 0x40FD;
	private static final int LOGIC8 = 0x673B;

	private static final int [] CHANNEL_FLAGS = { 0x13458b, 0x0000ff, 0x00a0f9, 0x00ffff, 0x00ff00, 0xff0000, 0xf020a0, };

    private static int numChannels = 6;
    private static int reservedDurationInSamples = 10;
    private static int realDurationInSamples = 0;
    private static int scaledDurationInSamples = 0;
    private static int int4or5 = 4;

	private final String fileName;
	private final List<CompositeEvent> eventsBuffer = new ArrayList<>();

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
                stream = new FileOutputStream(fileName);
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
		int firstRecordTs = events.get(1).getTimestamp();
		int lastRecordTs = events.get(events.size() - 1).getTimestamp();
	    // we don't know the total duration, so we create a margin after the last record which equals to the duration of the first event
	    realDurationInSamples = lastRecordTs + firstRecordTs;
    	scaledDurationInSamples = realDurationInSamples / 4;

		writeChannelDataHeader();

    	// we need to split the combined events into separate channels
    	for (int ch = 0; ch < numChannels; ch++) {
    		List<Integer> chDeltas = new ArrayList<>();
			int chPrevState = -1;
			int prevTs = 0;
        	for (CompositeEvent event : events) {
        		int chState = getChannelState(ch, event);
        		int ts = event.getTimestamp();

        		if (chPrevState == -1) {
        			chPrevState = chState;
        		}
				if (chState != chPrevState) {
					int delta = ts - prevTs;
					if (delta > 0x7fff) {
						// todo: split too long events?
						throw new IOException();
					}
					// encode state
					if (chState == 0)
						delta |= 0x8000;

					chDeltas.add(delta);

					prevTs = ts;
					chPrevState = chState;
				}
			}

    		writeChannelData(ch, chDeltas, chPrevState, prevTs);
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
		case 4:
			return event.isCoil();
		case 5:
            return event.isInjector();
		}
		return -1;
	}

    ////////////////////////////////////////////////////////////////////

    private void writeHeader() throws IOException {
        stream.write(magic);

        write(title.length());
        write(title);
        stream.flush();

        write(BLOCK);
        write(SUB);
        write(frequency);
        write(0);
        write(reservedDurationInSamples);
        write(frequency / frequencyDiv);
        write(0, 2);
        write(numChannels);

        write(BLOCK);
        write(0);

        write(BLOCK);
        for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
		}
        write(0);

        write(BLOCK);

        int4or5 = (numChannels == 4) ? 4 : 5;
        writeId(int4or5, int4or5);
		write(SUB);
        write(0);
    }

    private void writeChannelHeader(int ch) throws IOException {
		write(0xff);
		write(ch);
		write("Channel " + ch);
		write(0, 2);
		write(1.0);
		write(0);
		write(0.0);
		write(1);	// or 2
		write(0.0);	// or 1.0

		// this part sounds like the 'next' pointer?
		if (ch == numChannels - 1) {
			write(0);
		} else {
			writeId(1 + ch, 1);
			for (int i = 0; i < 3; i++) {
				write((CHANNEL_FLAGS[ch] >> (i * 8)) & 0xff);
			}
		}
    }

    private void writeChannelDataHeader() throws IOException {
		write(BLOCK);
		write(scaledDurationInSamples);
		write(0, 5);
		write(numChannels);
		write(0, 3);
		writeId(0, 1);
		write(0);

        write(BLOCK);
		write(0, 3);

		for (int i = 0; i < numChannels; i++) {
			writeChannelHeader(i);
		}

        write(BLOCK);
		write(new int[]{ SUB, SUB, 0, SUB, 0, SUB });
		write(0, 6);

        write(BLOCK);
		write(0, 2);
		write(realDurationInSamples);
		write(0);
		write(SUB);
		write(reservedDurationInSamples);
		write(frequency / frequencyDiv);
		write(0, 2);
		write(SUB);
		write(0, 2);
		write(1);
		write(0, 3);
		writeId(int4or5, 0);

		write(BLOCK);
		write(new int[]{ realDurationInSamples, realDurationInSamples, realDurationInSamples });
		write(0);
		write(SUB);
		write(0);

		write(BLOCK);
		write(0);

		write(BLOCK);
		write(SUB, 4);
		write(0);

		write(BLOCK);
    	write(frequency);
		write(0, 3);
		write(1);
		write(0, 3);
		writeId(0, 0);
		write(new int[]{ 0, 1, 1, 0, 1, 0x13 });
		write(SUB);

		write(BLOCK);
		write(0);
		write(realDurationInSamples);
		write(0, 2);
		write(numChannels);
		write(new int[]{ 1, 0, 1 });
	}

    private void writeChannelData(int ch, List<Integer> chDeltas, int chLastState, int lastRecord) throws IOException {
    	int numEdges = chDeltas.size();
    	if (numEdges == 0)
    		lastRecord = 0;
    	write(CHANNEL_BLOCK);
    	// channel#0 is somehow special...
    	if (ch == 0) {
			write(SUB);
			write(BLOCK);
		}

		write(ch + 1);
		write(0);
		write(realDurationInSamples);
		write(1);
		writeAs(lastRecord, 2);

		int numSamplesLeft = realDurationInSamples - lastRecord;
		write(numSamplesLeft);

		write(chLastState);

		if (numEdges == 0) {	// empty
			write(FLAG_EMPTY);
			write(0, 30);
			return;
		}

		write(FLAG_NOTEMPTY);

		if (ch == 0) {
			write(0);
			write(BLOCK);
			write(0, 11);
			write(BLOCK);
		} else {
			write(0, 10);
		}

		write(numEdges);
		write(0);
		write(numEdges);
		write(0);
		write(numEdges);

		writeEdges(chDeltas);

		if (ch == 0) {
			write(BLOCK);
			write(0, 6);

			write(BLOCK);
			write(0, 6);

			write(BLOCK);
		} else {
			write(0, 9);
		}

		write(1);
		write(0);
		write(1);
		write(0);
		write(1);
    	write(0, 16);

    	writeRaw(0xFF, 8);
	   	writeRaw(0x02, 1);
	   	writeRaw(0x00, 7);
    }

    private void writeEdges(List<Integer> chDeltas) throws IOException {
    	for (int d : chDeltas) {
    		stream.write(d & 0xff);
    		stream.write((d >> 8) & 0xff);
    	}
    	stream.write(0x00);
    }

    private void writeChannelDataFooter() throws IOException {
    	write(0, 3);
    	write(1);
    	write(1);
    	write(0);
    	write(numChannels);
    }

    @Override
	protected void writeFooter() throws IOException {
		if (stream == null)
			return;
		writeEvents(eventsBuffer);
        write(BLOCK);
        for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
        }
        write(1);
		writeId(numChannels, 0x15);
        for (int i = 0; i < numChannels; i++) {
        	writeId(i, 1);
        }
        write(1);
        write(0);
        write(frequency);
		write(0, 16);
        write(0x01);
        write(0x23); // ???
        write(SUB);

        write(BLOCK);
        write(numChannels + 1);
        write(0);
        write(0xFFFFFFFFFFFFFFFFL);
        write(0xFFFFFFFFL);
        write(1);
        write(0, 3);

        write(BLOCK);
        write(0);

        write(BLOCK);
        write(0);
        write(1.0);
        write(SUB);
		write(0, 6);
		write(1);
		write(0, 4);

        write(1);
        write(0x29);  // ???
        write(SUB);

        writeTimingMarker();

        stream.flush();
		System.out.println("writeFooter " + fileName);
    }

    private void writeTimingMarker() throws IOException {
    	write(BLOCK);
    	write(numChannels + 2);
    	write(0, 4);
    	write("Timing Marker Pair");
    	write("A1");
    	write("A2");
    	write(0, 2);
    	write(SUB);
    	write(0, 9);
    }

    private void writeId(int i1, int i2) throws IOException {
		write((numChannels == 4) ? LOGIC4 : LOGIC8);
		write(i1);
		write(i2);
    }

    ///////////////////////////////////////////////////////////////////////

    private void write(int value, int num) throws IOException {
    	for (int i = 0; i < num; i++)
    		write(value);
    }

	private void write(int[] values) throws IOException {
		for (int value : values)
			write(value);
	}

    private void writeAs(long value, int numBytes) throws IOException {
    	if (value == 0) {
   			stream.write(0);
   		} else {
			stream.write(numBytes);
			for (int i = 0; i < numBytes; i++) {
				stream.write((byte)((value >> (i * 8)) & 0xff));
			}
		}
    }

    // This is the main secret of this format! :)
    private void write(long value) throws IOException {
    	if (value < 0 || value > 0xFFFFFFFFL) {
    		writeAs(value, 8);
    	} else if (value == 0) {
    		stream.write(0);
		} else if (value <= 0xff) {
			writeAs(value, 1);
		} else if (value <= 0xffff) {
			writeAs(value, 2);
		} else if (value <= 0xffffff) {
			writeAs(value, 3);
		} else {
			writeAs(value, 4);
		}
	}

    private void write(double value) throws IOException {
		if (value == 0.0) {
			stream.write(0);
		} else {
			stream.write(8);
			// doubles are saved little-endian, sorry Java :)
			ByteBuffer bb = ByteBuffer.allocate(8);
			bb.order(ByteOrder.LITTLE_ENDIAN);
	        bb.putDouble(value);
	        bb.rewind();
	        for (int i = 0; i < 8; i++) {
	        	stream.write(bb.get());
	        }
		}
    }

    private void write(String str) throws IOException {
   		write(str.length());
    	for (char c : str.toCharArray()) {
    		stream.write(c);
    	}
	}

	private void writeRaw(int value, int num) throws IOException {
    	for (int i = 0; i < num; i++) {
    		stream.write(value);
    	}
    }
}
