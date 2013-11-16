/* Binary logger by Matthias Kraaz. Creative commons. */
/** @file Adaption of binary logger to environment */

#ifndef BL_ENVIRONMENT_INCLUDED
#define BL_ENVIRONMENT_INCLUDED

#include "binary_logger_internal.h"

/** Supposed to lock out other producers with a mutex or by disabling interrupts. 
    Feel free to define a macro instead, for example:
#define bl_lock_producers() __disable_interrupt()
 */
void bl_lock_producers(void);

/** Undo bl_lock_producers. Feel free to define a macro instead, for example:
#define bl_unlock_producers() __enable_interrupt()
 */
void bl_unlock_producers(void);

/** Output bytes. */
void bl_write(const uint8_t *buf, uint_fast8_t count);

#endif
