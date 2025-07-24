#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>

typedef struct
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} SEM;

/**
 * @brief Creates and initializes a new semaphore.
 *
 * Allocates and initializes a semaphore with the specified initial value.
 * If initialization fails at any stage, all allocated resources are released,
 * and @c errno is set to indicate the error.
 *
 * The semaphore may be initialized with a negative value. In such cases, the
 * counter must be incremented (via the V-operation) @c -initVal times to
 * reset the semaphore's value back to zero.
 *
 * @param value The initial value of the semaphore.
 * @return Pointer to the initialized semaphore, or @c NULL on failure.
 */
SEM *semCreate( int value );

/**
 * @brief Destroys a semaphore and releases its resources.
 *
 * Ensures the semaphore's internal mutex, condition variable, and the semaphore structure
 * itself are properly destroyed.
 *
 * @param sem Pointer to the semaphore to be destroyed.
 */
void semDestroy( SEM *sem );

/**
 * @brief Performs the P (proberen/test) operation on a semaphore.
 *
 * Attempts to decrement the semaphore's value by 1. If the semaphore's value is not a
 * positive number, the operation blocks until a V-operation increments the value
 * and the P-operation succeeds.
 *
 * @param sem Pointer to the semaphore to be decremented.
 */
void P( SEM *sem );

/**
 * @brief Performs the V (verhogen/increment) operation on a semaphore.
 *
 * Increments the semaphore's value by 1 and notifies P-operations that are
 * blocked on the semaphore of the change.
 *
 * @param sem Pointer to the semaphore to be incremented.
 */
void V( SEM *sem );

#endif
