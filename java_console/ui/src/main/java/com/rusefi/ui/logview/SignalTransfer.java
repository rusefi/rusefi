package com.rusefi.ui.logview;

import javax.swing.JComponent;
import javax.swing.TransferHandler;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;

/**
 * Drag and drop of a log signal (its field index) inside the viewer: from the signal list to a plot,
 * from one plot's legend to another, or back to the list to remove it.
 */
final class SignalTransfer {
    static final DataFlavor FLAVOR = new DataFlavor(Integer.class, "rusEFI log signal");

    private SignalTransfer() {
    }

    static Transferable of(int field) {
        return new Transferable() {
            @Override
            public DataFlavor[] getTransferDataFlavors() {
                return new DataFlavor[]{FLAVOR};
            }

            @Override
            public boolean isDataFlavorSupported(DataFlavor flavor) {
                return FLAVOR.equals(flavor);
            }

            @Override
            public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException {
                if (!isDataFlavorSupported(flavor)) {
                    throw new UnsupportedFlavorException(flavor);
                }
                return field;
            }
        };
    }

    /** The dragged field, or -1 when the transfer is not a signal. */
    static int field(Transferable transferable) {
        try {
            return transferable.isDataFlavorSupported(FLAVOR) ? (Integer) transferable.getTransferData(FLAVOR) : -1;
        } catch (Exception e) {
            return -1;
        }
    }

    /**
     * A component's one transfer handler: signal drags are handled by the subclass, log files dropped on
     * the same component go to the viewer host's file handler, which opens them.
     */
    abstract static class Handler extends TransferHandler {
        private TransferHandler fileDrop;

        void setFileDrop(TransferHandler fileDrop) {
            this.fileDrop = fileDrop;
        }

        TransferHandler fileDrop() {
            return fileDrop;
        }

        @Override
        public int getSourceActions(JComponent component) {
            return MOVE;
        }

        @Override
        public boolean canImport(TransferSupport support) {
            if (support.isDataFlavorSupported(FLAVOR)) {
                return canImportSignal(support);
            }
            return fileDrop != null && fileDrop.canImport(support);
        }

        @Override
        public boolean importData(TransferSupport support) {
            if (support.isDataFlavorSupported(FLAVOR)) {
                int field = field(support.getTransferable());
                return field >= 0 && importSignal(support, field);
            }
            return fileDrop != null && fileDrop.importData(support);
        }

        abstract boolean canImportSignal(TransferSupport support);

        abstract boolean importSignal(TransferSupport support, int field);
    }
}
