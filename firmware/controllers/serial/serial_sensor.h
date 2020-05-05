/**
 * @file	serial_sensor.h
 *
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#define AFRMIN 0
#define AFRMAX 40

#define AFR_DEFAULT 14.70

void SerialSendTest();
void IdentifyInnovateSerialMsg();
void ParseInnovateSerialMsg();
void ParseSerialData();