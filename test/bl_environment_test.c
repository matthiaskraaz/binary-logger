/* Binary logger by Matthias Kraaz. Creative commons. */
/** @file Adaption of binary logger to running inside a POSIX process for testing purposed. */ 
#include "bl_environment.h"
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bl_lock_producers(void)
{
  int ok = pthread_mutex_lock( &mutex );
  assert(ok == 0);
}

void bl_unlock_producers(void)
{
  int ok = pthread_mutex_unlock( &mutex );
  assert(ok == 0);
}

void bl_write(const uint8_t *buf, uint_fast8_t count)
{
  int ok = write(STDOUT_FILENO, buf, (size_t)count);
  assert(ok == count);
}
