#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    
    int res = 0;
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    printf("thread: wait_to_obtain  %d seconds \n", thread_func_args->wait_to_obtain * 1000);

    /* Waiting for obtaining the thread (in seconds) ... */
    usleep(thread_func_args->wait_to_obtain * 1000);

    /* Initializing thread_complete_success ... */
    thread_func_args->thread_complete_success = true;

    printf("thread: mutex_lock");
    res = pthread_mutex_lock(thread_func_args->mutex);
    if (res != 0) {
        ERROR_LOG("Error on mutex lock");
        printf("thread: error mutex_lock");
        thread_func_args->thread_complete_success = false;
    }
    else {

        printf("thread: wait_to_release  %d seconds \n", thread_func_args->wait_to_release * 1000);
        /* Waiting for keepimng the mutex (in seconds) ... */
        usleep(thread_func_args->wait_to_release * 1000);

        printf("thread: mutex_unlock");
        res = pthread_mutex_unlock(thread_func_args->mutex);
        if (res != 0) {
            printf("thread: mutex_unlock");
            ERROR_LOG("Error on mutex unlock");
            thread_func_args->thread_complete_success = false;
        }
    }
    
    printf("thread: return");
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

    bool retcode = true;

    struct thread_data* pdata = (struct thread_data*)malloc(sizeof(struct thread_data));
    if (pdata == NULL) {
        ERROR_LOG("Error allocating memory for thread_data");
        printf("main: error malloc\n");
        retcode = false;
    }
    else {
        /* initializing pdata */
        pdata->mutex = mutex;
        pdata->wait_to_obtain = wait_to_obtain_ms;
        pdata->wait_to_release = wait_to_release_ms;
        pdata->thread_complete_success = false;

        /* Creating the thread */
        printf("main: pthread_create\n");
        printf("Info:\n\twait1 %d ms\n\twait2 %d ms\n", pdata->wait_to_obtain, pdata->wait_to_release);
        int res = pthread_create(thread, NULL, threadfunc, pdata);
        if (res != 0) {
            ERROR_LOG("Error creating the thread");
            printf("main: error pthread_create\n");
            retcode = false;
        }
        printf("main: realeasing the memory\n");
        /* Release the memory */
        free(pdata);
    }

    printf("main: return %d\n", retcode);
    return retcode;
}

