/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:33:27 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 19:44:42 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_current_time(t_time_unit unit)
{
    struct timeval time;

    if (gettimeofday(&time, NULL))
        error_exit("Gettimeofday() failed.");
    if (unit == SECOND)
        return (time.tv_sec + (time.tv_usec / 1e6));
    else if (unit == MILLISECOND)
        return ((time.tv_sec * 1e3) + (time.tv_usec / 1e3));
    else if (unit == MICROSECOND)
        return ((time.tv_sec * 1e6) + time.tv_usec);
    return (-1);
}

void philo_doing(long action_time)
{
    long start;

    start = get_current_time(MICROSECOND);
    while (get_current_time(MICROSECOND) - start < action_time * 1e3)
        usleep(10);
}

void error_exit(const char *err_msg)
{
    printf(RED "%s\n" RST, err_msg);
    exit(EXIT_FAILURE);
}


void clean_table(t_table *table)
{
    for (int i = 0; i < table->philo_nbr; i++) {
        safe_mutex(&table->forks[i].fork, DESTROY);
    }
    safe_mutex(&table->table_mutex, DESTROY);

    free(table->philos);
    free(table->forks);
}

