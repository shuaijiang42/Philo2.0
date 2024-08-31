/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:42:06 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 17:03:29 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int  is_digit(char c)
{
    return (c >='0'&& c <= '9');
}


int is_valid_number(char *str)
{
    if (*str == '+')
        str++;
    if (*str == '\0')
        error_exit("The input has to be a valid positive number.");
    while (*str != '\0')
    {
        if(!is_digit(*str))
            error_exit("The input has to be a valid positive number.");
        str++;
    }
    return (1);
}


/*
    timestamps >= 60ms // Should not set action time values lower than 60ms 
    usleep function needs micro sec 

*/
//input: ./philo  nbr_philos time_to_die time_to_eat time_to_sleep [nbr_meals_max] (optional)
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long ft_atol(const char *str) {
    long result = 0;
    int sign = 1;
    size_t i = 0;

    is_valid_number((char *)str);
    while (is_digit(str[i])) {
        int digit = str[i] - '0';

        if (result > (LONG_MAX - digit) / 10) {
            return (sign == 1) ? LONG_MAX : LONG_MIN;
        }

        result = result * 10 + digit;
        i++;
    }

    return result * sign;
}

void  parse_input(t_table *table, char **argv)
{
    table->philo_nbr = ft_atol(argv[1]);
    table->time_to_die = ft_atol(argv[2])* 1000;
    table->time_to_eat = ft_atol(argv[3])* 1000;
    table->time_to_sleep = ft_atol(argv[4])* 1000;
    if (table->time_to_die < 60 * 1e3 || table->time_to_eat < 60 * 1e3 || table->time_to_sleep < 60 * 1e3)
        error_exit("Should not set action time values lower than 60ms.");
    if (argv[5])
        table->max_meals = ft_atol(argv[5]);
    else
        table->max_meals = -1;
}

