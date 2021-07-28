/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.logger.ecu.comms.manager;

public final class PollingStateImpl implements PollingState {
    private static int currentState;
    private static int lastpollState;
    private static boolean newQuery;
    private static boolean lastQuery;
    private static boolean fastPoll;

    public PollingStateImpl() {
        setCurrentState(0);
        setLastState(0);
        setNewQuery(true);
        setLastQuery(false);
        setFastPoll(false);
    }

    public int getCurrentState() {
        return currentState;
    }

    public void setCurrentState(int i) {
        currentState = i;
    }

    public int getLastState() {
        return lastpollState;
    }

    public void setLastState(int i) {
        lastpollState = i;
    }

    public boolean isNewQuery() {
        return newQuery;
    }

    public void setNewQuery(boolean state) {
        newQuery = state;
    }

    public boolean isLastQuery() {
        return lastQuery;
    }

    public void setLastQuery(boolean state) {
        lastQuery = state;
    }

    public boolean isFastPoll() {
        return fastPoll;
    }

    public void setFastPoll(boolean state) {
        fastPoll = state;
    }

    public String toString() {
        final String state = String.format(
                "Polling State [isFastPoll=%s, CurrentState=%d, LastState=%d, " +
                "isNewQuery=%s, isLastQuery=%s]",
               isFastPoll(),
            getCurrentState(),
            getLastState(),
            isNewQuery(),
            isLastQuery()
        );
        return state;
    }
}
