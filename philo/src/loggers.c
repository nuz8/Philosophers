/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loggers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:17 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 19:17:30 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	log_event_safe(t_phil *philo, t_phstates state);

int	log_event_safe(t_phil *philo, t_phstates state)
{
	// long	start_time;
	t_df	*df;

	df = get_df();
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) != 0)
		return (-1);
	if (philo_should_exit(df, philo, BOTH))
		return (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)),
			SIM_COMPLETED);
	// start_time = get_sim_time(MILLI);
	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
		printf("%ld	%d has taken a fork\n", get_sim_time(MILLI), philo->id);
	else if (state == EATING)
		printf("%ld	%d is eating\n", get_sim_time(MILLI), philo->id);
	else if (state == SLEEPING)
		printf("%ld	%d is sleeping\n", get_sim_time(MILLI), philo->id);
	else if (state == THINKING)
		printf("%ld	%d is thinking\n", get_sim_time(MILLI), philo->id);
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)) != 0)
		return (-1);
	return (0);
}
