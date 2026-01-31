package com.rusefi.io;

/**
 * there is only one instance of this per ECU connector
 * for business logic level UI code see {@link ConnectionStatusLogic#addListener}
 *
 * @author Andrey Belomutskiy
 *         3/1/2017
 * @see ConnectionStatusValue
 * @see ConnectionStatusLogic
 */
public interface ConnectionStateListener extends ConnectionFailedListener {
    ConnectionStateListener VOID = new AbstractConnectionStateListener();

    /**
     * This method is invoked once we have connection & configuration from controller
     */
    void onConnectionEstablished();
}
