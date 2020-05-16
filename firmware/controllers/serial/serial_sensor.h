/**
 * @file	serial_sensor.h
 *
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#define AFRMIN 0
#define AFRMAX 33

#define AFR_ERROR 30

void IdentifyInnovateSerialMsg();
void ParseInnovateSerialMsg();
void ParseSerialData();
void ResetSerialSensor();
void ClearSerialBuffer();