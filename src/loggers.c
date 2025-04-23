/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loggers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:17 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/23 12:20:02 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	log_event(t_phil *philo, e_phstates state);


/*
Function to print logs for each event, indicating the current state of each
philosopher
*/
void	log_event(t_phil *philo, e_phstates state)
{
	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
		printf("%ld	%d has taken a fork\n", get_sim_time(2), philo->id);
	else if (state == EATING)
		printf("%ld	%d is eating\n", get_sim_time(2), philo->id);
	else if (state == SLEEPING)
		printf("%ld	%d is sleeping\n", get_sim_time(2), philo->id);
	else if (state == THINKING)
		printf("%ld	%d is thinking\n", get_sim_time(2), philo->id);
	else if (state == DIED)
		printf("%ld	%d died\n", get_sim_time(2), philo->id);
}
