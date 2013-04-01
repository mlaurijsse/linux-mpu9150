////////////////////////////////////////////////////////////////////////////
//
//  This file is part of linux-mpu9150
//
//  Copyright (c) 2013 Pansenti, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of 
//  this software and associated documentation files (the "Software"), to deal in 
//  the Software without restriction, including without limitation the rights to use, 
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
//  Software, and to permit persons to whom the Software is furnished to do so, 
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all 
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef MPU9150_H
#define MPU9150_H

#include "quaternion.h"

#define SENSOR_RANGE 4096

typedef struct {
	short range[3];
} accelcal_t;

typedef struct {
	short offset[3];
	short range[3];
} magcal_t;

typedef struct {
	short rawGyro[3];
	short rawAccel[3];
	long rawQuat[4];
	unsigned long dmpTimestamp;

	short rawMag[3];
	unsigned long magTimestamp;

	vector3d_t calibratedAccel;
	vector3d_t calibratedMag;

	quaternion_t fusedQuat;
	vector3d_t fusedEuler;

	float lastDMPYaw;
	float lastYaw;
} mpudata_t;


int mpu9150_init(int i2c_bus, int sample_rate, int yaw_mixing_factor);
void mpu9150_exit();
int mpu9150_read(mpudata_t *mpu);
void mpu9150_set_accel_cal(accelcal_t *cal);
void mpu9150_set_mag_cal(magcal_t *cal);

#endif /* MPU9150_H */

