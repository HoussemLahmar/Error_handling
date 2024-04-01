# RTOS Approach

## Changes Made:
- Introduced an RTOS-based approach to error handling.
- Added support for FreeRTOS.
- Added a FreeRTOS queue to store incoming errors (`error_queue`).
- Implemented a continuous error processing loop within the error handler function.
- Utilized FreeRTOS API functions (`xQueueCreate`, `xQueueSend`, `xQueueReceive`) to manage the error queue.
- Ensured thread safety by using a mutex (`buffer_mutex`) to protect access to the circular buffer.
- Modified the error handling logic to enqueue errors into the error queue instead of processing them immediately.
- Added error queue management and circular buffer update logic within the error handler function.
- Updated the markdown file to reflect the changes made and summarize the additions.

## New Codes/Functions Added:
- `QueueHandle_t error_queue`: FreeRTOS queue handle to store incoming errors.
- `xQueueCreate`: Function to create the error queue with a defined size (`ERROR_QUEUE_SIZE`).
- `xQueueSend`: Function to enqueue errors into the error queue.
- `xQueueReceive`: Function to dequeue errors from the error queue.
- Continuous error processing loop within the error handler function.
- Mutex (`buffer_mutex`) for thread safety while accessing the circular buffer.
