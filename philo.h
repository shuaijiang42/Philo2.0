/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:30:16 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 19:54:14 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> 
# include <stdlib.h>
# include <stdint.h> 
# include <unistd.h>
# include <stdbool.h> 
# include <errno.h> 
# include <string.h> //- memset: Fills memory with a constant byte.
# include <pthread.h> 

/* 
- pthread_create: creates a new thread.
- pthread_detach: detaches a thread.
- pthread_join: joins with a terminated thread.
- pthread_mutex_init: initializes a mutex.
- pthread_mutex_destroy: destroys a mutex.
- pthread_mutex_lock: locks a mutex.
- pthread_mutex_unlock: unlocks a mutex.
*/
# include <sys/time.h> // gettimeofday

/* PHILO MAX */
# ifndef PHILO_MAX
#  define PHILO_MAX 200
# endif


//*******************************   HANDLE STATUS    *******************************

/* Philo states */
typedef enum e_philo_status
{
    EATING,
    SLEEPING,
    THINKING,
    TAKE_FIRST_FORK,
    TAKE_SECOND_FORK,
    DIED,
    NOTHING,
}   t_philo_status;

/* Time units */

typedef enum e_time_unit
{
    SECOND,
    MILLISECOND,
    MICROSECOND,
}   t_time_unit;

/* Operation code*/
typedef enum e_opcode
{
    INIT,
    DESTROY,
    CREATE,
    JOIN,
    DETACH,
}   t_opcode;




//*******************************   STRUCTS    *******************************

typedef struct s_table t_table; // define it later bcs it is needed to define philo

typedef pthread_mutex_t t_mtx; //just alia
typedef struct s_fork
{
    t_mtx fork;
    int fork_id;
    int occupied;
}   t_fork;

typedef struct s_philo //a philo is a thread
{
    pthread_t thread_id; 
    int philo_id;
    t_fork *first_fork;
    t_fork *second_fork;
    long meals_count;
    long action_time;
    long time_to_die;
    t_table *table;
}   t_philo;


/*
  table contains all the datas
  ./philo philo_nbr time_to_die time_to_eat time_to_sleep [max_meals]
  ./philo 5         800         200         200           [5]  

*/
typedef struct s_table
{
    int    philo_nbr;
    long    time_to_die;
    long    time_to_eat;
    long    time_to_sleep;
    int    max_meals;
    int    running_threads_nbr;
    long    fiesta_starts_time;
    bool    fiesta_ends;
    long    full_philo_nbr;

    t_philo *philos;
    t_fork  *forks;
    pthread_t monitor;
    t_mtx table_mutex;
}   t_table;



//***************   SAFE WRAPPERS     ***************
void *safe_malloc(size_t bytes);
void safe_thread(pthread_t *thread, void *(*start_routine) (void *), void *arg, t_opcode opcode);
void safe_mutex(t_mtx *mtx, t_opcode opcode);

//***************    PARSING INPUT   ***************
int is_valid_number(char *str);
void  parse_input(t_table *table, char **argv);

//***************    INIT TABLE & PHILO    ***************
void    table_init(t_table *table);


//***************    UTILS     ***************
void error_exit(const char *err_msg);
void ft_doing(long milliseconds, t_table *table);
long get_current_time(t_time_unit unit);
void clean_table(t_table *table);


//***************         ***************
int    routine(t_table *table);

void    monitor_log(t_philo_status status, t_philo *philo);
void	philo_wait(t_philo *philo, long wait, t_philo_status status);


//*******************************   BETTER UX    *******************************
/* 
** ANSI Escape Sequences for Bold Text Colors
** Usage: 
**     printf(R "This is red text." RST);
**     printf(B "This is blue text." RST);
** Remember to reset the color after setting it.
*/

# define RST    "\033[0m"      /* Reset to default color */
# define RED	"\033[1;31m"   /* Bold Red */
# define G      "\033[1;32m"   /* Bold Green */
# define Y      "\033[1;33m"   /* Bold Yellow */
# define B      "\033[1;34m"   /* Bold Blue */
# define M      "\033[1;35m"   /* Bold Magenta */
# define C      "\033[1;36m"   /* Bold Cyan */
# define W      "\033[1;37m"   /* Bold White */





#endif