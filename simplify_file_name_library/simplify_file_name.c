#include "simplify_file_name.h"
#include <string.h>
#include <stdlib.h>

// Function to simplify the file name if its length exceeds the limit
const char *simplify_file_name(const char file_name[]) {
    // Check if file_name is NULL or empty
    if (file_name == NULL || file_name[0] == '\0') {
        return file_name;
    }

    size_t file_length = strlen(file_name);

    // If the length of the file name is less than or equal to the limit, no need to simplify
    if (file_length <= MAX_FILE_NAME_LENGTH) {
        return file_name;
    } else {
        // Allocate memory for the new simplified file name
        char *simplified_file_name = (char *)malloc(MAX_FILE_NAME_LENGTH + 1);
        if (simplified_file_name == NULL) {
            // Handle memory allocation failure (unlikely)
            // Still return the original file name instead of NULL
            return file_name;
        }

        // Copy characters from file_name to simplified_file_name up to MAX_FILE_NAME_LENGTH
        for (size_t i = 0; i < MAX_FILE_NAME_LENGTH; ++i) {
            simplified_file_name[i] = file_name[i];
        }
        simplified_file_name[MAX_FILE_NAME_LENGTH] = '\0'; // Ensure the string is correctly terminated

        return simplified_file_name;
    }
}
