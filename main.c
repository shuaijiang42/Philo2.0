/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:37:57 by shujiang          #+#    #+#             */
/*   Updated: 2024/07/20 16:45:04 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h> 
//input: ./philo  nbr_philos time_to_die time_to_eat time_to_sleep [nbr_meals_max] (optional)
int main(int args, char **argv)
{
    t_table table;

    if (args == 5 || args == 6)
    {
        parse_input(&table, argv);
        table_init(&table);
        routine(&table);
        clean_table(&table);
        return 0;
    }    
    else
        error_exit("Error: Wrong number of arguments!");
    return(0);

}