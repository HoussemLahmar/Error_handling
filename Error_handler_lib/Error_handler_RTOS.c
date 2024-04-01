#include "Error_handler.h"
#include "simplify_file_name.h"
#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "FreeRTOS.h"
#include "queue.h"


#define BUFFER_SIZE 50

// Define EEPROM base address (example: 0x08080000 for STM32)
#define EEPROM_BASE_ADDRESS 0x08080000

// Define EEPROM addresses for circular buffer parameters
uint32_t NVM_BUFFER_LENGTH_ADDRESS = EEPROM_BASE_ADDRESS;           // Initialize to EEPROM base address
uint32_t NVM_WRITE_INDEX_ADDRESS = EEPROM_BASE_ADDRESS + 4;         // Increment by size of uint32_t (4 bytes)
uint32_t NVM_READ_INDEX_ADDRESS = EEPROM_BASE_ADDRESS + 2 * sizeof(uint32_t);  // Increment by twice the size of uint32_t

pthread_mutex_t buffer_mutex; // Mutex for protecting access to the circular buffer
QueueHandle_t error_queue;    // FreeRTOS queue handle for storing errors
#define ERROR_QUEUE_SIZE 10    // Define the size of the error queue

// Initialize mutex dynamically
void init_mutex() {
    if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        fprintf(stderr, "Error: Mutex initialization failed\n");
        exit(EXIT_FAILURE);
    }
}

int error_handler(const char file_name[], int line) {
    // Simplify the file name
    const char *simplified_file_name = simplify_file_name(file_name);
    if (simplified_file_name == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for simplified file name\n");
        return MEMORY_ALLOCATION_ERROR; 
    }

    // Create the error queue if it hasn't been created yet
    if (error_queue == NULL) {
        error_queue = xQueueCreate(ERROR_QUEUE_SIZE, sizeof(ErrorEntry));
        if (error_queue == NULL) {
            fprintf(stderr, "Error: Failed to create error queue\n");
            free((void*)simplified_file_name);  
            return ERROR_QUEUE_CREATION_ERROR;
        }
    }

    // Initialize circular buffer parameters outside the loop
    WN_CircularBuffer_TypeDef circular_buffer;
    ErrorEntry buffer[BUFFER_SIZE];
    WN_APP_InitCircularBuffer(&circular_buffer, buffer, BUFFER_SIZE, sizeof(ErrorEntry), 
                               NVM_BUFFER_LENGTH_ADDRESS, NVM_WRITE_INDEX_ADDRESS, NVM_READ_INDEX_ADDRESS);

    // Create an error entry
    ErrorEntry error_entry = {simplified_file_name, line};

    // Enqueue the error into the error queue
    if (xQueueSend(error_queue, &error_entry, portMAX_DELAY) != pdPASS) {
        fprintf(stderr, "Error: Failed to enqueue error\n");
        free((void*)simplified_file_name);  // Free allocated memory before returning
        return ERROR_QUEUE_SEND_ERROR;
    }

    // Print the error message
    fprintf(stderr, "Error enqueued: File %s, Line %d\n", simplified_file_name, line);

    // Free the memory allocated for simplified_file_name
    free((void*)simplified_file_name);

    // Process errors from the error queue and write them to the circular buffer
    while (1) {
        ErrorEntry error_entry;
        if (xQueueReceive(error_queue, &error_entry, portMAX_DELAY) == pdPASS) {
            // Lock the mutex before accessing the circular buffer
            pthread_mutex_lock(&buffer_mutex);

            // Add the entry to the circular buffer
            WriteToCircularBuffer(&circular_buffer, &error_entry);

            // Update or increment EEPROM addresses based on circular buffer position
            NVM_BUFFER_LENGTH_ADDRESS += sizeof(ErrorEntry);
            NVM_WRITE_INDEX_ADDRESS += sizeof(ErrorEntry);
            NVM_READ_INDEX_ADDRESS += sizeof(ErrorEntry);

            // Unlock the mutex after accessing the circular buffer
            pthread_mutex_unlock(&buffer_mutex);

            // Print the error message
            fprintf(stderr, "Error processed: File %s, Line %d\n", error_entry.file_name, error_entry.line_number);
        }
    }

    return 0;
}