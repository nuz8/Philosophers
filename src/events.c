/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:18:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 21:06:24 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo, bool first_think);

static int	philo_is_full(t_df *df, t_phil *philo);

/*
Function to simulate eating
	- Lock fork1 (only for odd philos or even philos depending on the turn)
	- Update fork fields
	- Lock fork2
	- Update fork fields
	- Write the log for eating			// might need a mutex for this too
	- Update philo fields to indicate eating state
	- Update no. of meals left to be eaten by the philo (meals_left--)
*/
int	philo_eat(t_df *df, t_phil *philo)
{
	long	start_time;

	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);

	if (philo_pickup_forks(df, philo))
		return (SIM_COMPLETED);
	start_time = get_sim_time(MICRO);
	if (log_event_safe(philo, EATING))
		return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
	set_long(&philo->mtx, &philo->lastmeal_time, get_sim_time(MICRO));
	if (ft_usleep(start_time, df->tte) == SIM_COMPLETED)
		return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
	set_long(&philo->mtx, &philo->meals_left, DECREASE);
	if (philo_is_full(df, philo))
		return (SIM_COMPLETED);
	philo_drop_forks(philo, BOTH);
	return (0);
}

static int	philo_is_full(t_df *df, t_phil *philo)
{
	if (df->max_meals >= 0 && get_long(&philo->mtx, &philo->meals_left) == 0)
	{
		if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) == 0)
		{
			if (philo_should_exit(df, philo, BOTH))
				return (philo_drop_forks(philo, BOTH), print_mutex_error(UNLOCK,
						pthread_mutex_unlock(&df->mtx_write)), SIM_COMPLETED);
			set_bool(&philo->mtx, &philo->full, true);
			printf("%ld	%d is sleeping\n", get_sim_time(MILLI), philo->id);
			print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write));
		}
		else
			return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
	}
	return (0);
}

// Enter Sandman function
int	philo_sleep(t_df *df, t_phil *philo)
{
	long	start_time;
	
	start_time = get_sim_time(MICRO);
	if (log_event_safe(philo, SLEEPING) < 0)
		return (-1);
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	
	ft_usleep(start_time, df->tts);
	return (0);
}

int	philo_think(t_df *df, t_phil *philo, bool first_think)
{
	// long	start_time;

	(void)first_think;
	// start_time = get_sim_time(MICRO);
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	if (log_event_safe(philo, THINKING) < 0)
		return (SIM_COMPLETED);
	if (df->total_philos % 2)
	{
		// if (ft_usleep(start_time, 100))
		if (philo->id % 2)
			if (ft_usleep(get_sim_time(MICRO), df->tte / 2))
				return (SIM_COMPLETED);	
	}
	return (0);
}
