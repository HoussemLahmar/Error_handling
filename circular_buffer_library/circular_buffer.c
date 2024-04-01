/********************************************************************************
* @file	wn_circular_buffer.h
* @author  Wattnow firmware team
* @date : 24/12/2020
*
* @brief This file contains circular buffer functions
*       This file contains :
*
*
@verbatim
[..]
(@)
[..]
@endverbatim
******************************************************************************
* @attention
*
* 	<h2><center>&copy; COPYRIGHT(c) 2020 Wattnow</center></h2>
*	All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Wattnow and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Wattnow and its suppliers and may be covered
* by Tunisian and Foreign Patents, patents in process, and are protected
* by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Wattnow.
******************************************************************************
//*
https://medium.com/@charlesdobson/how-to-implement-a-simple-circular-buffer-in-c-34b7e945d30e

we use the same algorithm described in the previous link except for the
processing executed when the buffer is full, in our case we do an
"empty reading", ie we will do the same steps as for a reading, except that
the data will not be read and transferred to another sending routine (or other),
 but it will be deleted; thus the oldest data in the buffer will be deleted and
a new one will take its place.

*/
#include "../simplify_file_name_library/simplify_file_name.h"

#include "wn_circular_buffer.h"
int WN_APP_WriteToCircularBuffer(WN_CicularBuffer_TypeDef *p_circular_buffer, ErrorEntry *p_error_entry) {
    uint32_t address = 0;
    // Verify if the buffer is full
    if (p_circular_buffer->buffer_length == p_circular_buffer->buffer_size) {
        // Dummy read to move the read index to the next position
        WN_APP_DummyReadFromCircularBuffer(p_circular_buffer);
    }
    // Calculate the address to write
    address = (uint32_t)p_circular_buffer->p_buffer + p_circular_buffer->write_index * p_circular_buffer->element_size;
    // Call the function to write the data element to the buffer
    p_circular_buffer->writeBuffer(address, p_error_entry);
    // Increment buffer length
    p_circular_buffer->buffer_length++;
    // Increment write index
    p_circular_buffer->write_index++;
    // Check if the write index is at the end of the buffer or not
    if (p_circular_buffer->write_index == p_circular_buffer->buffer_size) {
        p_circular_buffer->write_index = 0;
    }
    // Write circular buffer parameters to NVM
    WN_APP_WriteCircularBufferParamNvm(p_circular_buffer);

    return 0;
}

int8_t WN_APP_ReadFromCircularBuffer(WN_CicularBuffer_TypeDef *p_circular_buffer,void *p_data)
{
    uint32_t address = 0;
    int8_t rc = 0; // return code:  0 =  no error;-1 = empty buffer; -2 = negative value
    /* emty buffer */
    if(p_circular_buffer->buffer_length == 0)
    {
        rc = -1;
    }
    else if(p_circular_buffer->buffer_length < 0)
    {
        rc = -2;
    }
    else /*(p_circular_buffer->buffer_length > 0)*/
    {
        address = (uint32_t)p_circular_buffer->p_buffer+ p_circular_buffer->read_index * p_circular_buffer->element_size ;
        //
        p_circular_buffer->readBuffer(address, p_data);
        //
        p_circular_buffer->buffer_length--;
        p_circular_buffer->read_index++;

        if (p_circular_buffer->read_index == p_circular_buffer->buffer_size)
        {
            p_circular_buffer->read_index = 0;
        }
        rc = 0;
    }
    //
    return rc;
}
/*
a dummy read to remove the one element from the buffer.
we not need to calculate the address of the element we need only to increment the
read index and to decrement the buffer length. we not need also a pointer to a data
because is a dummy read and the data will not be used for any purpose.
*/
int8_t WN_APP_DummyReadFromCircularBuffer(WN_CicularBuffer_TypeDef *p_circular_buffer)
{
    int8_t rc = 0; // return code:  0 =  no error;-1 = empty buffer; -2 = negative value
    /* emty buffer */
    if(p_circular_buffer->buffer_length == 0)
    {
        rc = -1;
    }
    else if(p_circular_buffer->buffer_length < 0)
    {
        rc = -2;
    }
    else /*(p_circular_buffer->buffer_length > 0)*/
    {
        //
        p_circular_buffer->buffer_length--;
        p_circular_buffer->read_index++;
        //
        if (p_circular_buffer->read_index == p_circular_buffer->buffer_size)
        {
            p_circular_buffer->read_index = 0;
        }
        rc = 0;
    }
    //
    return rc;
}

void WN_APP_InitCircularBuffer(WN_CicularBuffer_TypeDef *p_circular_buffer,
                               ErrorEntry *p_buffer,
                               uint32_t buffer_size,
                               uint8_t element_size,
                               uint32_t nvm_buffer_length_add,
                               uint32_t nvm_write_index_add,
                               uint32_t nvm_read_index_add)
{
    p_circular_buffer->p_buffer = (uint8_t*)p_buffer; // Casting to uint8_t* to match the type
    p_circular_buffer->buffer_size = buffer_size;
    p_circular_buffer->element_size = element_size;
    p_circular_buffer->nvm_buffer_length_add = nvm_buffer_length_add;
    p_circular_buffer->nvm_write_index_add = nvm_write_index_add;
    p_circular_buffer->nvm_read_index_add = nvm_read_index_add;
    WN_APP_ReadCircularBufferParamNvm(p_circular_buffer);
}
/*save circular buffer parameters to nvm*/
void WN_APP_WriteCircularBufferParamNvm(WN_CicularBuffer_TypeDef *p_circular_buffer)
{
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_buffer_length_add , p_circular_buffer->buffer_length);
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_write_index_add, p_circular_buffer->write_index);
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_read_index_add, p_circular_buffer->read_index);
    //
    return;

}
/*read circular buffer parameters from nvm*/
void WN_APP_ReadCircularBufferParamNvm(WN_CicularBuffer_TypeDef *p_circular_buffer)
{
    p_circular_buffer->readFromNvm(p_circular_buffer->nvm_buffer_length_add, &p_circular_buffer->buffer_length);
    p_circular_buffer->readFromNvm(p_circular_buffer->nvm_write_index_add, &p_circular_buffer->write_index);
    p_circular_buffer->readFromNvm(p_circular_buffer->nvm_read_index_add, &p_circular_buffer->read_index);
    //
    return;
}
int8_t WN_APP_EraseCircularBufferParamNvm(WN_CicularBuffer_TypeDef *p_circular_buffer)
{
    p_circular_buffer->buffer_length = 0;
    p_circular_buffer->write_index = 0;
    p_circular_buffer->read_index = 0;
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_buffer_length_add , 0);
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_write_index_add, 0);
    p_circular_buffer->writeToNvm(p_circular_buffer->nvm_read_index_add, 0);
    //
    return 0;

}
