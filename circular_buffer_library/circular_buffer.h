#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

#include <stdint.h>

// Structure to hold error information
typedef struct {
    const char *file_name;
    uint32_t line_number;
} ErrorEntry;

// Structure representing a circular buffer
typedef struct {
    uint32_t element_size;
    void *p_buffer;
    uint32_t buffer_size;
    uint32_t buffer_length;
    uint32_t write_index;
    uint32_t read_index;
    uint32_t nvm_buffer_length_add;
    uint32_t nvm_write_index_add;
    uint32_t nvm_read_index_add;
    // Function pointers
    void (*WriteToCircularBuffer)(void *p_data, uint32_t size, void *p_buffer, uint32_t *write_index, uint32_t buffer_size);
    void (*ReadFromCircularBuffer)(void *p_data, uint32_t size, void *p_buffer, uint32_t *read_index, uint32_t buffer_size);
    void (*DummyReadFromCircularBuffer)(void *p_data, uint32_t size, void *p_buffer, uint32_t *read_index, uint32_t buffer_size);
    void (*InitCircularBuffer)(void *p_buffer, uint32_t buffer_size);
    void (*WriteCircularBufferParamNvm)(void *p_buffer, uint32_t buffer_size);
    void (*ReadCircularBufferParamNvm)(void *p_buffer, uint32_t buffer_size);
    void (*EraseCircularBufferParamNvm)(void *p_buffer, uint32_t buffer_size);
} CircularBuffer_TypeDef;

// Function declarations
void WriteToCircularBuffer(void *p_data, uint32_t size, void *p_buffer, uint32_t *write_index, uint32_t buffer_size);
void ReadFromCircularBuffer(void *p_data, uint32_t size, void *p_buffer, uint32_t *read_index, uint32_t buffer_size);
void DummyReadFromCircularBuffer(void *p_data, uint32_t size, void *p_buffer, uint32_t *read_index, uint32_t buffer_size);
void InitCircularBuffer(void *p_buffer, uint32_t buffer_size);
void WriteCircularBufferParamNvm(void *p_buffer, uint32_t buffer_size);
void ReadCircularBufferParamNvm(void *p_buffer, uint32_t buffer_size);
void EraseCircularBufferParamNvm(void *p_buffer, uint32_t buffer_size);

#endif // __CIRCULAR_BUFFER_H
