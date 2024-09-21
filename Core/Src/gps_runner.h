/*
 * gps_runner.h
 *
 *  Created on: Sep 21, 2024
 *      Author: jaspartapgoomer
 */

#ifndef SRC_GPS_RUNNER_H_
#define SRC_GPS_RUNNER_H_

#include "stm32f4xx_hal.h"  // Include HAL library for STM32

// Buffer size for receiving GPS data
#define GPS_BUFFER_SIZE 100

// Function to initialize GPS module (UART)
void GPS_Init(void);

// Function to process and extract GPS coordinates from received NMEA sentences
void GPS_GetCoordinates(void);

#endif /* SRC_GPS_RUNNER_H_ */
