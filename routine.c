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
	monitor_log(THINKING, philo);
	while(philo->first_fork->ocupied != 0 && !philo->table->fiesta_ends)
		usleep(10);
	pthread_mutex_lock(&philo->first_fork->fork);
	printf("here %d\n", philo->first_fork->ocupied);
	philo->first_fork->ocupied = 1;
	printf("here2 %d\n", philo->first_fork->ocupied);
	monitor_log(TAKE_FIRST_FORK, philo);
	while(philo->second_fork->ocupied != 0 && !philo->table->fiesta_ends)
		usleep(10);
	pthread_mutex_lock(&philo->first_fork->fork);
	philo->first_fork->ocupied = 1;	
	monitor_log(TAKE_SECOND_FORK, philo);
}

static void eat(t_philo *philo)
{
	if (philo->table->fiesta_ends)
	{
		pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_unlock(&philo->second_fork->fork);
		return ;
	}
	monitor_log(EATING, philo);
	philo_doing(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->first_fork->fork);
	philo->first_fork->ocupied = 0;	

	pthread_mutex_unlock(&philo->second_fork->fork);
	philo->first_fork->ocupied = 0;	

}

static void psleep(t_philo *philo)
{
    if (philo->table->fiesta_ends)
		return ;
	monitor_log(SLEEPING, philo);
	philo_doing(philo->table->time_to_eat);

}

static void    *simulation(void *data)
{
	t_philo *philo = (t_philo *)data;
	t_table *table = philo->table;
	
	table->running_threads_nbr++;
	
	while (table->running_threads_nbr != table->philo_nbr)
		;
	table->fiesta_starts_time = get_current_time(MILLISECOND);
	while (!philo->table->fiesta_ends)
	{
		think(philo);
		eat(philo);
		psleep(philo);
	}
	return NULL;
}

void    routine(t_table *table)
{
	int i;

	i = 0;
	if (table->max_meals == 0)
		return;
	if (table->philo_nbr == 1)
	{
		table->fiesta_starts_time = get_current_time(MILLISECOND);
		usleep(table->time_to_die * 1e3);
		monitor_log(DIED, &table->philos[0]);
		return;
	}
	for (i = 0; i < table->philo_nbr; i++) {
		safe_thread(&table->philos[i].thread_id, simulation, &table->philos[i], CREATE);
	}
	for (i = 0; i < table->philo_nbr; i++) {
		safe_thread(&table->philos[i].thread_id, NULL, NULL, JOIN);
	}

}