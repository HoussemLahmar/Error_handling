# Error Handling Module

This module provides comprehensive functionality for error handling and logging within an embedded system. It includes libraries for simplifying file names, implementing circular buffers, and handling errors in both bare-metal and FreeRTOS environments.

## Simplify File Name Library

The `simplify_file_name` library includes a function to simplify file names if their length exceeds a certain limit.

### Header File: `simplify_file_name.h`

```c
#include "simplify_file_name.h"
#include <string.h>
#include <stdlib.h>

const char *simplify_file_name(const char file_name[]);
```

### Description

The `simplify_file_name` function takes a file name as input and returns a simplified version if its length exceeds a predefined limit. This is useful for error logging, as it ensures that file names do not exceed the maximum length supported by the system.

---

## Circular Buffer Library

The circular buffer library provides data structures and functions for implementing circular buffers, which are commonly used for storing data in embedded systems.

### Overview

Circular buffers offer efficient storage and retrieval of data, particularly in scenarios where data needs to be continuously processed in a cyclical manner. This library provides the necessary tools to create, manipulate, and manage circular buffers within embedded applications.

### Functionality

- **Initialization**: Initialize a circular buffer with specified parameters such as buffer size and element size.
- **Write Operation**: Write data to the circular buffer, automatically handling buffer wrap-around.
- **Read Operation**: Read data from the circular buffer, ensuring data integrity and proper buffer management.
- **Parameter Management**: Save and retrieve circular buffer parameters to and from non-volatile memory (NVM).

### Usage

To utilize the circular buffer library, include the appropriate header file (`wn_circular_buffer.h`) in your application code and call the provided functions to perform buffer operations.

---

## Error Handler Library

The error handler library implements error handling functions for both bare metal and FreeRTOS environments, facilitating robust error management and logging capabilities.

### Bare Metal Implementation

#### Header File: `error_handler_bare_metal.h`

```c
#include "../simplify_file_name_library/simplify_file_name.h"
#include "../wn_circular_buffer_library/wn_circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 50

int error_handler(const char file_name[], int line);
```

#### Description

The `error_handler` function is designed for error handling in bare metal environments. It enqueues error entries into a circular buffer for later processing, ensuring efficient and reliable error logging without relying on an operating system.

### FreeRTOS Implementation

#### Header File: `error_handler_freertos.h`

```c
#include "../simplify_file_name_library/simplify_file_name.h"
#include "../wn_circular_buffer_library/wn_circular_buffer.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 50

int error_handler(const char file_name[], int line);
void error_task(void *pvParameters);
```

#### Description

The FreeRTOS implementation of the `error_handler` function operates within a FreeRTOS task, allowing for error handling in multitasking environments. It follows a similar approach to the bare metal version but runs asynchronously, ensuring minimal impact on system performance.

### Usage

To integrate error handling into your application, include the appropriate header file (`error_handler_bare_metal.h` or `error_handler_freertos.h`) and call the `error_handler` function whenever an error occurs in the system. Additionally, ensure that the circular buffer and other dependencies are properly initialized before invoking the error handler.

---

