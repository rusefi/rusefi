package com.opensr5.io;

/**
* @author Andrey Belomutskiy
*         3/3/14
*/
public interface DataListener {
    void onDataArrived(byte[] freshData);
}
