/* Binary logger by Matthias Kraaz. Creative commons. */
/** @file Public interface of binary logger */

#ifndef BINARY_LOGGER_INCLUDED
#define BINARY_LOGGER_INCLUDED

#include "binary_logger_internal.h"

/** Sets file ID of current file. */
#define LOG_FILE_ID(file_id) static const uint16_t bl_file_id = (uint16_t)file_id

/** Logs a message.
    @param message Format string. Not included into firmware, formatting is offloaded from device.
    Format of format string: something like "{0} {1:x} {2:2X}". TODO: Elaborate.
    @param ... Arguments given as LOG_I8(i), LOG_S(s) etc.
 */
#define LOG(message, ...) \
	do { \
		BL_STATIC_ASSERT(__LINE__ <= UINT16_MAX ); /* Ensure that line number fits into binary format */ \
		bl_start(bl_file_id, __LINE__); \
		__VA_ARGS__; \
		bl_end( ); \
	} while(0)

/** Logs version of source code (a string).
    You need the version of the source code to translate the binary stream into readable output.
    Uses special file ID and line number to easily recognize it in the binary stream.
*/
#define LOG_VERSION(version) \
	do { \
		bl_start(0u, 0u); \
		bl_log_string(version); \
		bl_end( ); \
	} while(0)

/** Supplies parameter for log message */
#define LOG_I8(value)   bl_log_i8( value )

/** Supplies parameter for log message */
#define LOG_UI8(value)  bl_log_ui8( value )

/** Supplies parameter for log message */
#define LOG_I16(value)  bl_log_i16( value )

/** Supplies parameter for log message */
#define LOG_UI16(value) bl_log_ui16( value )

/** Supplies parameter for log message */
#define LOG_I32(value)  bl_log_i32( value )

/** Supplies parameter for log message */
#define LOG_UI32(value) bl_log_ui32( value )

/** Supplies parameter for log message */
#define LOG_I64(value)  bl_log_i64( value )

/** Supplies parameter for log message */
#define LOG_UI64(value) bl_log_ui64( value )

/** Supplies parameter for log message */
#define LOG_S(value)    bl_log_string( value )

#endif

