/*
 * gps_runner.c
 *
 *  Created on: Sep 21, 2024
 *      Author: jaspartapgoomer
 */

#include "gps_runner.h"
#include <string.h>  // For string functions like strstr and sscanf

extern UART_HandleTypeDef huart1;  // UART handle for USART1

static char gps_buffer[GPS_BUFFER_SIZE];  // Buffer to store incoming GPS data
static volatile uint8_t gps_data_received = 0;  // Flag to signal when data is received

// Function to initialize GPS module (UART)
void GPS_Init(void)
{
    // Start receiving data from the GPS module using interrupt mode
    HAL_UART_Receive_IT(&huart1, (uint8_t *)gps_buffer, 1);
}

// Callback function for UART interrupt when data is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)  // Check if the interrupt is from USART1
    {
        static uint8_t idx = 0;  // Buffer index

        gps_buffer[idx++] = gps_buffer[0];  // Store received data

        // Check if we have received a complete NMEA sentence (terminated by newline)
        if (gps_buffer[0] == '\n')
        {
            gps_buffer[idx] = '\0';  // Null-terminate the string
            idx = 0;  // Reset index for the next sentence
            gps_data_received = 1;  // Set flag to indicate a complete sentence
        }

        // Continue receiving more data
        HAL_UART_Receive_IT(&huart1, (uint8_t *)gps_buffer, 1);
    }
}

// Function to process and return GPS coordinates
uint8_t GPS_GetCoordinates(char *latitude, char *longitude)
{
    if (gps_data_received)  // Check if new GPS data is available
    {
        gps_data_received = 0;  // Reset the flag

        // Look for the $GPRMC sentence (contains GPS coordinates)
        char *ptr = strstr(gps_buffer, "$GPRMC");

        if (ptr)  // If GPRMC sentence found
        {
            // Parse the GPRMC sentence to extract latitude and longitude
            char time[10];
            sscanf(ptr, "$GPRMC,%[^,],%*[^,],%[^,],%*[^,],%[^,]", time, latitude, longitude);
            return 1;  // Return success (valid GPS data found)
        }
    }

    return 0;  // Return 0 if no valid data or incomplete sentence
}
