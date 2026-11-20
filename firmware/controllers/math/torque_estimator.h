#pragma once

void estimateTorqueTable();

/**
 * Interpolate indicated torque in Nm from the configured page 4 table.
 * torqueLoadBins must contain MAP in kPa; torqueRpmBins contain RPM.
 * Inputs outside the axes use the nearest table edge.
 */
float getIndicatedTorque(float map, float rpm);
