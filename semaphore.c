#include "semaphore.h"

#include <errno.h>
#include <stdlib.h>

SEM *semCreate( int value )
{
    SEM *sem;

    // Allocate memory for the semaphore structure
    if( ( sem = malloc( sizeof( SEM ) ) ) == NULL )
    {
        return NULL;
    }

    // Initialize the condition variable used for passive waiting
    if( errno = pthread_cond_init( &sem->condition, NULL ) )
    {
        free( sem );
        return NULL;
    }

    // Initialize the mutex used to make the semaphore operations atomic
    pthread_mutex_init( &sem->mutex, NULL );

    // Set the initial semaphore value
    sem->value = value;

    return sem;
}

void semDestroy( SEM *sem )
{
    int ret;

    // Attempt to destroy the mutex; ensure no thread is using it
    do
    {
        ret = pthread_mutex_destroy( &sem->mutex );
    } while( ret == EBUSY );

    // Attempt to destroy the condition variable; ensure no thread is waiting on it
    do
    {
        ret = pthread_cond_destroy( &sem->condition );
    } while( ret == EBUSY );

    // Deallocate the semaphore structure
    free( sem );
}

void P( SEM *sem )
{
    // Acquire the semaphore's mutex to update its value atomically
    pthread_mutex_lock( &sem->mutex );

    // Passively wait until the semaphore's value is positive
    while( sem->value <= 0 )
    {
        pthread_cond_wait( &sem->condition, &sem->mutex );
    }

    // Decrement the semaphore's value by one
    sem->value--;

    // Release the mutex so other threads can access the semaphore
    pthread_mutex_unlock( &sem->mutex );
}

void V( SEM *sem )
{
    // Acquire the semaphore's mutex to update its value atomically
    pthread_mutex_lock( &sem->mutex );

    // Increment the semaphore's value by one
    sem->value++;

    // Notify a P-operation that is blocked on the semaphore
    if( sem->value >= 1 )
    {
        pthread_cond_signal( &sem->condition );
    }

    // Release the mutex so other threads can access the semaphore
    pthread_mutex_unlock( &sem->mutex );
}
