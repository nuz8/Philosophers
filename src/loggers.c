/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loggers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:17 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 12:55:33 by pamatya          ###   ########.fr       */
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
		printf("%ld	"Y"%d "C"has taken a fork"RST"\n", get_sim_time(2), philo->id);
	else if (state == EATING)
		printf("%ld	"Y"%d "G"is eating"RST"\n", get_sim_time(2), philo->id);
	else if (state == SLEEPING)
		printf("%ld	"Y"%d "B"is sleeping"RST"\n", get_sim_time(2), philo->id);
	else if (state == THINKING)
		printf("%ld	"Y"%d "W"is thinking"RST"\n", get_sim_time(2), philo->id);
	else if (state == DIED)
		printf("%ld	"Y"%d "R"died"RST"\n", get_sim_time(2), philo->id);
}
