/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_wrapper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:50:09 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 19:43:56 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void *safe_malloc(size_t bytes)
{
    void *memory;
    memory = malloc(bytes);
    if (!memory)
        error_exit("malloc failed");
    return (memory);
}

/*** Safe thread ***/
static void check_thread_error(int status, t_opcode opcode)
{
    if (status == 0)
        return ;
    printf("Error: thread %d failed: %d - ", opcode, status);
    if (status == EAGAIN)
        error_exit("Insufficient resources to create another thread.\n");
    else if (status == EDEADLK)
        error_exit("A  deadlock  was  detected.\n");
    else if (status == EINVAL)
        error_exit("Not a joinable thread\n");
    else if (status == ESRCH)
        error_exit("No thread with the ID thread could be found.\n");
    else
        error_exit("Unknown error occurs during mutex operation");

}

/* Thread ops: create, join, detach */
void safe_thread(pthread_t *thread, void *(*start_routine) (void *), void *arg, t_opcode opcode)
{
    int status = 0;
    if (opcode == CREATE)
        status = pthread_create(thread, NULL, start_routine, arg);
    else if (opcode == JOIN)
        status = pthread_join(*thread, NULL);
    else if (opcode == DETACH)
        status = pthread_detach(*thread);
    else
        error_exit("Thread opcode error. Valid input: CREATE, JOIN or DETACH");
    check_thread_error(status, opcode);
}

/*** Safe mutex ***/
static void check_mutex_error(int status, t_opcode opcode)
{
    if (status == 0)
        return ;   
    printf("Error: mutex %d failed: %d - ", opcode, status);
    if (status == EAGAIN)
        error_exit("Maximum number of recursive locks exceeded.\n");
    else if (status == EINVAL)
        error_exit("The value specified by mutex is invalid.\n");
    else if (status == EDEADLK)
        error_exit("Deadlock condition detected.\n");
    else if (status == EBUSY)
        error_exit("EBUSY - Mutex already locked.\n");
    else if (status == EPERM)
        error_exit("EPERM - Current thread does not own the mutex.\n");
    else if (status == ENOMEM)
        error_exit("Insufficient memory exists to initialize the mutex.\n");
    else if (status == ENOTRECOVERABLE)
        error_exit("The protected state of the mutex cannot be recovered.\n");
    else if (status == EOWNERDEAD)
        error_exit("Robust mutex, and the process of the thread that owns the mutex terminated while holding the mutex lock.\n");
    else
        error_exit("Unknown error occurs during mutex operation");

} 



  //mutex oprations: INIT, DESTORY   I took LOCK, UNLOCK away from this function because I need them to be fast, checking so many conditions takes time
void safe_mutex(t_mtx *mtx, t_opcode opcode)
{
    int status = 0;
    if (opcode == INIT)
        status = pthread_mutex_init(mtx, NULL);
    else if (opcode == DESTROY)
        status = pthread_mutex_destroy(mtx);
    else
        error_exit("Mutex opcode error. Valid input: INIT or DESTROY");
    check_mutex_error(status, opcode);
}

