/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loggers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:17 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/29 13:02:07 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	log_event(t_phil *philo, e_phstates state);


/*
Function to print logs for each event, indicating the current state of the
philosopher printing the log
	- Prints logs by locking/unlocking df->mtx
	- If successful, returns 0
	- If mutex errors are detected, then the corresponding error message is
	  printed by print_mutex_error() fn call and -1 is returned
*/
int	log_event(t_phil *philo, e_phstates state)
{
	t_df	*df;
	
	df = get_df();
	if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx)) != 0)
		return (-1);
	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
		printf("%ld	"Y"%d "C"has taken a fork"RST"\n", get_sim_time(MILLI), philo->id);
	else if (state == EATING)
		printf("%ld	"Y"%d "G"is eating"RST"\n", get_sim_time(MILLI), philo->id);
	else if (state == SLEEPING)
		printf("%ld	"Y"%d "B"is sleeping"RST"\n", get_sim_time(MILLI), philo->id);
	else if (state == THINKING)
		printf("%ld	"Y"%d "W"is thinking"RST"\n", get_sim_time(MILLI), philo->id);
	else if (state == DIED)
		printf("%ld	"Y"%d "R"died"RST"\n", get_sim_time(MILLI), philo->id);
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx)) != 0)
		return (-1);
	return (0);
}
