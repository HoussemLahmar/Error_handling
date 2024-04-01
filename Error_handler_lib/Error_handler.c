#include "error_handler.h"
#include "../simplify_file_name_library/simplify_file_name.h"
#include "../wn_circular_buffer_library/wn_circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 50


// Define EEPROM base address (example: 0x08080000 for STM32)
#define EEPROM_BASE_ADDRESS 0x08080000

// Define EEPROM addresses for circular buffer parameters
uint32_t NVM_BUFFER_LENGTH_ADDRESS = EEPROM_BASE_ADDRESS;           // Initialize to EEPROM base address
uint32_t NVM_WRITE_INDEX_ADDRESS = EEPROM_BASE_ADDRESS + 4;         // Increment by size of uint32_t (4 bytes)
uint32_t NVM_READ_INDEX_ADDRESS = EEPROM_BASE_ADDRESS + 2 * sizeof(uint32_t);  // Increment by twice the size of uint32_t

pthread_mutex_t buffer_mutex; // Mutex for protecting access to the circular buffer

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

    // Lock the mutex before accessing the circular buffer
    pthread_mutex_lock(&buffer_mutex);

    // Initializing circular buffer parameters
    WN_CircularBuffer_TypeDef circular_buffer;
    ErrorEntry buffer[BUFFER_SIZE];
    // Initialize circular buffer with EEPROM addresses
    WN_APP_InitCircularBuffer(&circular_buffer, buffer, BUFFER_SIZE, sizeof(ErrorEntry), 
                               NVM_BUFFER_LENGTH_ADDRESS, NVM_WRITE_INDEX_ADDRESS, NVM_READ_INDEX_ADDRESS);

    // Add the entry to the circular buffer
    ErrorEntry error_entry = {simplified_file_name, line};
    WN_APP_WriteToCircularBuffer(&circular_buffer, &error_entry);

    // Update EEPROM addresses based on circular buffer position
    NVM_BUFFER_LENGTH_ADDRESS += sizeof(ErrorEntry);
    NVM_WRITE_INDEX_ADDRESS += sizeof(ErrorEntry);
    NVM_READ_INDEX_ADDRESS += sizeof(ErrorEntry);

    // Unlock the mutex after accessing the circular buffer
    pthread_mutex_unlock(&buffer_mutex);

    // Print the error message
    fprintf(stderr, "Error occurred in file %s at line %d\n", simplified_file_name, line);

    // Free the memory allocated for simplified_file_name
    free((void*)simplified_file_name);

    return 0; // Return success code
}
