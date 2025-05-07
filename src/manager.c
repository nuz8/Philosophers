/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:55:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/07 16:49:02 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*supervise(void *arg);
static int	update_philo_death(t_df *df, t_phil *philo);
// static int	update_philo_deaths(t_df *df);

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
	philos_full = 0;
	while (!get_bool(&df->mtx, &df->sim_finished))
	{
		philos_full = 0;
		i = -1;
		while (++i < df->total_philos && !get_bool(&df->mtx, &df->sim_finished))
		{
			update_philo_death(df, df->philos + i);
			if ((df->philos + i)->dead == true)
			{
				if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) == 0)
				{
					set_bool(&df->mtx, &df->sim_finished, true);
					printf("%ld	%d died\n", get_sim_time(MILLI), (df->philos + i)->id);
					print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write));
				}
				return (printf("\t\t\t"M"Manager thread exiting here instead due to "Y"%d"M" dying\n"RST, (df->philos + i)->id), NULL) ;
			}
			if (get_int(&(df->philos + i)->mtx, &(df->philos + i)->meals_left) == 0)
				philos_full++;
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
static int	update_philo_death(t_df *df, t_phil *philo)
{
	long	time_without_food;

	time_without_food = 0;	
	time_without_food = get_sim_time(MICRO) - get_long(&philo->mtx,
			&philo->lastmeal_time);
	if (get_bool(&philo->mtx, &philo->full) == false && time_without_food > df->ttd)
		set_bool(&philo->mtx, &philo->dead, true);
	return (0);
}
