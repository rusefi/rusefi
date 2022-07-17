package com.rusefi.app;

import android.content.ContextWrapper;
import android.widget.TextView;

import com.rusefi.Listener;
import com.rusefi.dfu.DfuImage;
import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.shared.FileUtil;

import java.io.File;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;

public class DfuUpload {
    private static final String BUNDLE_FILE = "rusefi_bundle_autoupdate.zip";
    //public static final String FILE = "rusefi_bundle_mre_f4_autoupdate.zip";
    //private static final String DFU_FILE_NAME = "rusefi_mre_f4.dfu";
    private static final String DFU_FILE_NAME = "rusefi_.dfu";

    public final String localDfuImageFileName;
    public final String localFullFile;
    public final File localFolder;

    public DfuUpload(ContextWrapper context) {
        localFolder = context.getExternalFilesDir(null);
        localFullFile = localFolder + File.separator + BUNDLE_FILE;
        localDfuImageFileName = localFolder + File.separator + DFU_FILE_NAME;
    }

    void downloadFileIfNotPresent(final TextView mResultView) {
        if (new File(this.localFullFile).exists()) {
            mResultView.append(BUNDLE_FILE + " found!\n");
            uncompressFile(this.localFullFile, this.localFolder, this.localDfuImageFileName, mResultView);
        } else {
            mResultView.append(BUNDLE_FILE + " not found!\n");

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        ConnectionAndMeta c = new ConnectionAndMeta(BUNDLE_FILE).invoke(ConnectionAndMeta.BASE_URL_LATEST);
                        ConnectionAndMeta.downloadFile(localFullFile, c, new ConnectionAndMeta.DownloadProgressListener() {
                            @Override
                            public void onPercentage(final int currentPercentage) {
                                mResultView.post(() -> mResultView.append("Downloaded " + currentPercentage + "%\n"));
                            }
                        });
                        mResultView.post(() -> mResultView.append("Downloaded! " + "\n"));
                        uncompressFile(localFullFile, localFolder, localDfuImageFileName, mResultView);

                    } catch (IOException e) {
                        mResultView.post(() -> mResultView.append("Error downloading " + e + "\n"));
                    }
                }
            }).start();
        }
    }

    public void uncompressFile(final String localFullFile, final File localFolder, final String localDfuImageFileName, final TextView mResultView) {
        final Listener<Integer> onSuccess = new Listener<Integer>() {
            @Override
            public void onResult(final Integer size) {
                mResultView.post(new Runnable() {
                    @Override
                    public void run() {
                        mResultView.append(localDfuImageFileName + " File size: " + size + "\n");
                    }
                });
                DfuImage dfuImage = new DfuImage();
                try {
                    dfuImage.read(localDfuImageFileName);
                } catch (Throwable e) {
                    mResultView.post(() -> mResultView.append("Error reading " + e));
                }
            }
        };

        new Thread(() -> {
            try {
                FileUtil.unzip(localFullFile, localFolder);
                final int size = (int) new File(localDfuImageFileName).length();
                onSuccess.onResult(size);

            } catch (final IOException e) {
                mResultView.post(() -> mResultView.append("Error uncompressing " + e + "\n"));
            }

        }).start();
    }
}
