/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:04:57 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 20:18:20 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
// action 1 comer
// action 2 dormir
// action 3 morir

static void    write_log(t_philo_status status, t_philo *philo)
{
    long time = philo->action_time;
    if (status == EATING)
        printf("%ld %d is eating\n", time, philo->philo_id);
    else if (status == SLEEPING)
        printf("%ld %d is sleeping\n", time, philo->philo_id);
    else if (status == THINKING)
        printf("%ld %d is thinking\n", time, philo->philo_id);
    else if (status == TAKE_FIRST_FORK)
        printf("%ld %d has taken the first fork[%d]\n", time, philo->philo_id, philo->first_fork->fork_id);
    else if (status == TAKE_SECOND_FORK)
        printf("%ld %d has taken the second fork[%d]\n", time, philo->philo_id, philo->second_fork->fork_id);
    else if (status == DIED)
        printf("%ld %d died\n", time, philo->philo_id);
}

void    monitor_log(t_philo_status status, t_philo *philo)
{
    static  pthread_mutex_t log = PTHREAD_MUTEX_INITIALIZER;
    t_table *table = philo->table;

    if (!table->fiesta_ends)
    {
        pthread_mutex_lock(&log);
        write_log(status, philo);
        if (status==DIED)
        {
            table->fiesta_ends = 1;
            return;
        }
        else if (status == EATING)
        {
            philo->meals_count++;
            if (table->max_meals > 0 && philo->meals_count == table->max_meals)
            {
                table->full_philo_nbr++;
                if (table->full_philo_nbr == table->philo_nbr)
                {
                    table->fiesta_ends = 1;
                    printf("Yeah, all philos are full and happy\n");
                    return; 
                }
            }    
        }
        pthread_mutex_unlock(&log);
   }

}

