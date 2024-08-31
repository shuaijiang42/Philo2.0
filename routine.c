/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:49:26 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 20:37:22 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void think(t_philo *philo)
{
	//printf("1 action time: %ld\n", philo->action_time);
	if (philo->table->fiesta_ends)
		return ;
	monitor_log(THINKING, philo);
	while(!philo->table->fiesta_ends && philo->first_fork->occupied != 0)
        philo_wait(philo, 10, NOTHING);
	pthread_mutex_lock(&philo->first_fork->fork);
	philo->first_fork->occupied = 1;
	monitor_log(TAKE_FIRST_FORK, philo);

	while(!philo->table->fiesta_ends && philo->second_fork->occupied != 0)
        philo_wait(philo, 10, NOTHING);
	pthread_mutex_lock(&philo->second_fork->fork);
	philo->second_fork->occupied = 1;	
	monitor_log(TAKE_SECOND_FORK, philo);
}
/* 
#define TIMEOUT 1000 // Time to wait in microseconds before giving up

static void think(t_philo *philo)
{
	if (philo->table->fiesta_ends)
		return ;
	monitor_log(THINKING, philo);

    // Try to pick up the first fork
    while (philo->first_fork->occupied != 0 && !philo->table->fiesta_ends)
        philo_wait(philo, 10, NOTHING);
    pthread_mutex_lock(&philo->first_fork->fork);
    philo->first_fork->occupied = 1;
    monitor_log(TAKE_FIRST_FORK, philo);

    // Try to pick up the second fork with timeout
    int waited_time = 0;
    while (philo->second_fork->occupied != 0 && !philo->table->fiesta_ends)
    {
        philo_wait(philo, 10, NOTHING);
        waited_time += 10;
        if (waited_time >= TIMEOUT)
        {
            // Release the first fork and retry
            philo->first_fork->occupied = 0;
            pthread_mutex_unlock(&philo->first_fork->fork);
            usleep(TIMEOUT); // Wait a bit before retrying
            think(philo);
            return;
        }
    }

    pthread_mutex_lock(&philo->second_fork->fork);
    philo->second_fork->occupied = 1;
    monitor_log(TAKE_SECOND_FORK, philo);
} */

static void eat(t_philo *philo)
{
	t_table *table;

	table = philo->table;
	if (table->fiesta_ends)
	{
		philo->first_fork->occupied = 0;
		pthread_mutex_unlock(&philo->first_fork->fork);
		philo->first_fork->occupied = 0;
		pthread_mutex_unlock(&philo->second_fork->fork);
		return ;
	}
	philo_wait(philo, table->time_to_eat, EATING);
  
    philo->first_fork->occupied = 0;
    pthread_mutex_unlock(&philo->first_fork->fork);
    
    philo->second_fork->occupied = 0;
    pthread_mutex_unlock(&philo->second_fork->fork);

}

static void psleep(t_philo *philo)
{
    if (philo->table->fiesta_ends)
		return ;
	philo_wait(philo, philo->table->time_to_sleep, SLEEPING);

}

static void    *simulation(void *data)
{
	t_philo *philo = (t_philo *)data;
	t_table *table = philo->table;
	
	table->running_threads_nbr++;
	
	while (table->running_threads_nbr != table->philo_nbr)
		;
	table->fiesta_starts_time = get_current_time(MILLISECOND);
	while (!table->fiesta_ends)
	{
		think(philo);
		eat(philo);
		psleep(philo);
	}
	return NULL;
}

int    routine(t_table *table)
{
	pthread_t	threads[3];
	int i;

	i = 0;
	if (table->max_meals == 0)
		return 0;
	if (table->philo_nbr == 1)
	{
		table->fiesta_starts_time = get_current_time(MILLISECOND);
		usleep(table->time_to_die);
        printf("%ld %d died\n", table->time_to_die/1000, table->philos[0].philo_id);
		return 0;
	}
	for (i = 0; i < table->philo_nbr && !table->fiesta_ends; i++) {
		//safe_thread(&table->philos[i].thread_id, simulation, &table->philos[i], CREATE);
		if (pthread_create(&threads[i], NULL,
				simulation, &table->philos[i]))
			return 1;
	}
	printf("ehlooo\n");
	for (i = 0; i < table->philo_nbr; i++) {
		//safe_thread(&table->philos[i].thread_id, NULL, NULL, JOIN);

		pthread_join(threads[i], NULL);
		printf("i = %d\n", i);
	}
	return 0;
}