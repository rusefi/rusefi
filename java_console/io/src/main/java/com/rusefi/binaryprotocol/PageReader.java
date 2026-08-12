package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.config.generated.Integration;

import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;

/**
 * Utility class for reading configuration pages from the ECU.
 * Supports reading multi-page configurations where each page has a unique identifier.
 */
public class PageReader {

    /**
     * Read data from a specific page number (not page index).
     * Page numbers in INI files are 1-based, but internal arrays are 0-based.
     *
     * @param bp BinaryProtocol instance to use for communication
     * @param iniPageNumber INI page number (1, 2, 3, etc.)
     * @param pageSize Size of the page in bytes
     * @param signature ECU signature string
     * @return ConfigurationImageWithMeta containing the page data, or VOID if read failed
     */
    public static ConfigurationImageWithMeta readPageData(BinaryProtocol bp, int iniPageNumber, int pageSize, String signature) {
        // Convert INI page number to 0-based index
        int pageIndex = iniPageNumber - 1;

        ConfigurationImageMetaVersion0_0 pageMeta = new ConfigurationImageMetaVersion0_0(pageSize, signature);
        ConfigurationImageWithMeta imageWithMeta = new ConfigurationImageWithMeta(pageMeta);
        ConfigurationImage image = imageWithMeta.getConfigurationImage();

        int offset = 0;
        int blockingFactor = bp.getIniFile().getBlockingFactor();

        if (image == null){
            return ConfigurationImageWithMeta.VOID;
        }

        while (offset < image.getSize()) {
            int remainingSize = image.getSize() - offset;
            int requestSize = Math.min(remainingSize, blockingFactor);

            // pageIdentifier uses big-endian format
            byte[] packet = new byte[6];
            packet[0] = 0x00;
            packet[1] = (byte) pageIndex;
            putShort(packet, 2, swap16(offset));
            putShort(packet, 4, swap16(requestSize));

            byte[] response = bp.executeCommand(Integration.TS_READ_COMMAND, packet,
                String.format("read page %d offset=%d size=%d", iniPageNumber, offset, requestSize));

            if (response == null) {
                return ConfigurationImageWithMeta.VOID;
            }

            if (response.length != requestSize + 1) {
                return ConfigurationImageWithMeta.VOID;
            }

            if (response[0] != Integration.TS_RESPONSE_OK) {
                return ConfigurationImageWithMeta.VOID;
            }

            System.arraycopy(response, 1, image.getContent(), offset, requestSize);
            offset += requestSize;
        }

        return imageWithMeta;
    }
}
