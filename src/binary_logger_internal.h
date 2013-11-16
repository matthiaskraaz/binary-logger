/* Binary logger by Matthias Kraaz. Creative commons. */
/** @file Internals of binary logger, not to be used directly */
#ifndef BINARY_LOGGER_INTERNAL_INCLUDED
#define BINARY_LOGGER_INTERNAL_INCLUDED

#include <stdint.h>

/** Max number of characters in a string. Longer strings are truncated */
#define BL_MAX_STR_LEN 16u

/** Compile time assert */
#define BL_STATIC_ASSERT(b) typedef char dummy[(b) ? 1 : -1]

/** Character type. MISRA-C doesn't like using primitive types. */
typedef char bl_char_t;

extern void bl_start(uint16_t file_id, uint16_t lineno);

extern void bl_end(void);

extern void bl_log_i8(int8_t value);

extern void bl_log_ui8(uint8_t value);

extern void bl_log_i16(int16_t value);

extern void bl_log_ui16(uint16_t value);

extern void bl_log_i32(int32_t value);

extern void bl_log_ui32(uint32_t value);

extern void bl_log_i64(int64_t value);

extern void bl_log_ui64(uint64_t value);

extern void bl_log_string(const bl_char_t *value);

#endif

