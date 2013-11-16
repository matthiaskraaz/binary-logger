/* Binary logger by Matthias Kraaz. Creative commons. */
#include "binary_logger.h"
#include "binary_logger_internal.h"
#include "bl_environment.h"
#include <stdbool.h>

/* Message format:
   record separator: 1 byte, ASCII RS
   file ID: 2 bytes, little endian
   line number: 2 bytes, little endian
   length of parameters: highest bit is overflow 8 flag
   parameters, if any
   checksum: 1 byte, XOR sum running over RS, length, and payload. 
*/

#define RS 0x1E
/* Number of bytes for everything before parameters */
#define HEAD_LENGTH 6
#define MAX_PARAMETERS_LENGTH 100
/* Number of bytes for everything after parameters */
#define TAIL_LENGTH 1
#define CHECKSUM_SEED 0xFF

static uint8_t my_message[HEAD_LENGTH + MAX_PARAMETERS_LENGTH + TAIL_LENGTH];
static uint_fast8_t my_write_index;
static uint8_t my_checksum;
static bool my_overflow;

static void bl_output_byte(uint8_t b)
{
    if (my_write_index < (HEAD_LENGTH + MAX_PARAMETERS_LENGTH))
    {
        my_message[my_write_index] = b;
        my_checksum ^= b;
        my_write_index++;
    }
    else
    {
        my_overflow = true;
    }
}

void bl_start(uint16_t file_id, uint16_t lineno)
{
    bl_lock_producers();

    my_write_index = 0;
    my_checksum = CHECKSUM_SEED;
    my_overflow = false;

    bl_output_byte(RS);
    bl_log_ui16( file_id );
    bl_log_ui16( lineno );

    /* Jump over length byte, is written later */
    my_write_index++;
}

void bl_end(void)
{
    /* Calculate and write length byte to buffer */
    my_message[5] = my_write_index - HEAD_LENGTH;
    if (my_overflow)
    {
        my_message[5] |= 0x80;
    }
    my_checksum ^= my_message[5];

    /* Write checksum to buffer */
    my_message[my_write_index] = my_checksum;
    my_write_index++;

    bl_write(my_message, my_write_index);

    bl_unlock_producers( );
}

void bl_log_i8(int8_t value)
{
    bl_output_byte((uint8_t)value); 
}

void bl_log_ui8(uint8_t value)
{
    bl_output_byte((uint8_t)value); 
}

void bl_log_i16(int16_t value)
{
    bl_log_ui16((uint16_t)value);
}

void bl_log_ui16(uint16_t value)
{
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
}

void bl_log_i32(int32_t value)
{
    bl_log_ui32((uint32_t)value);
}

void bl_log_ui32(uint32_t value)
{
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
}

void bl_log_i64(int64_t value)
{
    bl_log_ui64((uint64_t)value);
}

void bl_log_ui64(uint64_t value)
{
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
    value = value >> 8;
    bl_output_byte((uint8_t)value); 
}

void bl_log_string(const bl_char_t *value)
{
    uint32_t i;
    for(i = 0u; i < BL_MAX_STR_LEN; i++)
    {
        if(value[i] == (uint8_t)0)
        {
            break;
        }
        bl_output_byte( value[i] );
    }
    bl_output_byte( (uint8_t)0 );
}
