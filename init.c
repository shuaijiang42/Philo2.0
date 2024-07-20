/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:45:26 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 20:26:04 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void    assign_forks(t_philo *philo, t_fork *forks, int philo_seat)
{
    if (philo_seat % 2 == 0)
    {
        philo->first_fork = &forks[philo_seat];
        if (philo_seat == 0)
            philo->second_fork = &forks[philo->table->philo_nbr - 1];
        else
            philo->second_fork = &forks[philo_seat - 1];
    }
    else
    {
        philo->first_fork = &forks[philo_seat - 1];
        philo->second_fork = &forks[philo_seat];
    }

}

static void    philo_init(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    while (i < table->philo_nbr)
    {
        philo = &table->philos[i];
        philo->philo_id = i+1;
        philo->meals_count = 0;
        philo->table = table;
        philo->last_meal_time = get_current_time(MILLISECOND);
        assign_forks(philo, table->forks, i);
        i++;
    }
}

void    table_init(t_table *table)
{
    int i = 0;

    table->fiesta_ends = false;
    table->philos = safe_malloc(table->philo_nbr * sizeof(t_philo));
    table->forks = safe_malloc(table->philo_nbr * sizeof(t_fork));
    safe_mutex(&table->table_mutex, INIT);
    table->running_threads_nbr = 0;
    table->full_philo_nbr=0;
    while(i < table->philo_nbr)
    {   
        safe_mutex(&table->forks[i].fork, INIT);
        table->forks[i].fork_id = i;
        table->forks[i].ocupied = 0;
        i++;
    }
    philo_init(table);
}