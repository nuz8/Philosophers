/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:45:06 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 15:38:32 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			philo_pickup_forks(t_df *df, t_phil *philo);
int			philo_drop_forks(t_phil *philo, t_check drop);

static int	update_fork(t_fork *fork, t_phil *philo, t_fstates state);

// Function to lock fork mutexes and update fork states
int	philo_pickup_forks(t_df *df, t_phil *philo)
{
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork1->mtx)) != 0)
		return (-1);
	if (log_event_safe(philo, TOOK_FORK_1))
		return (philo_drop_forks(philo, ONE), SIM_COMPLETED);
	update_fork(philo->fork1, philo, TAKEN);
	if (df->total_philos > 1)
	{
		if (philo_should_exit(df, philo, BOTH))
			return (philo_drop_forks(philo, ONE), SIM_COMPLETED);
		if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork2->mtx)))
			return (philo_drop_forks(philo, ONE), -1);
		if (log_event_safe(philo, TOOK_FORK_2))
			return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
		update_fork(philo->fork2, philo, TAKEN);
	}
	else
		return (philo_drop_forks(philo, ONE), SIM_COMPLETED);
	if (philo_should_exit(df, philo, BOTH))
		return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
	return (0);
}

// Function to unlock fork mutexes and update fork states
int	philo_drop_forks(t_phil *philo, t_check drop)
{
	if (drop == ONE)
	{
		update_fork(philo->fork1, philo, FREE);
		if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&philo->fork1->mtx)))
			return (-1);
	}
	else if (drop == TWO)
	{
		update_fork(philo->fork2, philo, FREE);
		if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&philo->fork2->mtx)))
			return (-1);
	}
	else if (drop == BOTH)
	{
		update_fork(philo->fork1, philo, FREE);
		if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&philo->fork1->mtx)))
			return (-1);
		update_fork(philo->fork2, philo, FREE);
		if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&philo->fork2->mtx)))
			return (-1);
	}
	return (0);
}

static int	update_fork(t_fork *fork, t_phil *philo, t_fstates state)
{
	if (state == TAKEN)
	{
		fork->state = TAKEN;
		fork->taker_id = philo->id;
	}
	else if (state == FREE)
	{
		fork->state = FREE;
		fork->taker_id = 0;
	}
	return (0);
}
