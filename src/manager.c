/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:55:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 02:13:35 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*supervise(void *arg);
bool	philo_should_exit(t_df *df, t_phil *philo, e_check check);

static bool	philo_died(t_df *df, t_phil *philo, int *philos_full);


/*
Function to execute supervising role using the df-thread 'manager'

Possible data-races info:
	Accessed philo-fields: philos->	(dead, meals_left)
	Accessed df-fields:		df->	(total_philos, max_meals, sim_finished)
*/
void	*supervise(void *arg)
{
	t_df	*df;
	int		philos_full;
	int		i;
	
	df = (t_df *)arg;
	while (!all_threads_running(&df->mtx, &df->threads_running_nbr,
			df->total_philos))
		continue ;
	while (!get_bool(&df->mtx, &df->sim_finished))
	{
		philos_full = 0;
		i = -1;
		while (++i < df->total_philos && !get_bool(&df->mtx, &df->sim_finished))
		{
			if (philo_died(df, df->philos + i, &philos_full))
				return (NULL);
		}
		if (philos_full == df->total_philos)
		{
			set_bool(&df->mtx, &df->sim_finished, true);
			printf("Exiting by all philos being full.\n");
			break ;
		}
	}
	printf(M"%ld\t\t\t\tManager thread is exiting.\n"RST, get_sim_time(MILLI));
	return (NULL);
}

/*
Function to check whether the provided philo has died by checking its meal time
	- Uses mutexes
*/
static bool	philo_died(t_df *df, t_phil *philo, int *philos_full)
{
	long	time_without_food;

	time_without_food = 0;	
	time_without_food = get_sim_time(MICRO) - get_long(&philo->mtx,
			&philo->lastmeal_time);
	if (get_bool(&philo->mtx, &philo->full) == false
			&& time_without_food > df->ttd)
	{
		set_bool(&philo->mtx, &philo->dead, true);
		if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) == 0)
		{
			set_bool(&df->mtx, &df->sim_finished, true);
			printf("%ld	%d died\n", get_sim_time(MILLI), philo->id);
			print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write));
		}
		return (printf("\t\t\t"M"Manager thread exiting here instead due to "Y"%d"M" dying\n"RST, philo->id), true) ;
	}
	if (get_long(&philo->mtx, &philo->meals_left) == 0)
		(*philos_full)++;
	return (false);
}

bool	philo_should_exit(t_df *df, t_phil *philo, e_check check)
{
	// if (check == PHILO)
	if (philo && check == PHILO)
	{
		if (get_bool(&philo->mtx, &philo->full) == true)
			return (true);
	}
	// else if (check == SIMULATION)
	else if (df && check == SIMULATION)
	{
		if (get_bool(&df->mtx, &df->sim_finished))
			return (true);	
	}
	// else if (check == BOTH)
	else if (df && philo && check == BOTH)
	{
		if (get_bool(&df->mtx, &df->sim_finished)
				|| get_bool(&philo->mtx, &philo->full) == true)
			return (true);
	}
	return (false);
}
