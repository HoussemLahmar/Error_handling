#ifndef ERROR_HANDLER_H_INCLUDED
#define ERROR_HANDLER_H_INCLUDED

#include "../simplify_file_name_library/simplify_file_name.h"
// Define structure to hold error information
typedef struct {
    const char *file_name;  // Original file name
    int line_number;         // Line number where the error occurred
} ErrorEntry;


// Function to handle errors
void error_handler(const char file_name[], int line);
#endif // ERROR_HANDLER_H_INCLUDED
