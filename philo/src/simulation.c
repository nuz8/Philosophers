/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:46:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 15:15:58 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			start_simulation(t_df *df);

static void	*start_dining(void *arg);
static void	philos_sync_or_spin(t_df *df);
static int	philos_stagger(t_phil *philo);

/*
Function to start the dining philosophers simulation
Thread Syntaxes:
	- pthread_create(thread addr/pointer, NULL, fn, fn_arg);
	- pthread_join(*thread, NULL);
	- pthread_detach(*thread);
Note: '*thread' is simply a thread object/instance, and not a pointer or address
to some thread instance
*/
int	start_simulation(t_df *df)
{
	int		i;
	t_phil	*philos;

	i = -1;
	philos = df->philos;
	while (++i < df->total_philos)
	{
		if (print_thread_error(CREATE, pthread_create(&(philos + i)->th_id,
					NULL, start_dining, (philos + i))))
			return (-1);
	}
	if (print_thread_error(CREATE, pthread_create(&df->manager, NULL,
				supervise, df)))
		return (-1);
	df->start_time = get_abs_time(MICRO);
	set_bool(&df->mtx, &df->all_threads_ready, true);
	i = -1;
	while (++i < df->total_philos)
	{
		if (print_thread_error(JOIN, pthread_join((philos + i)->th_id, NULL)))
			return (-1);
	}
	if (print_thread_error(JOIN, pthread_join(df->manager, NULL)))
		return (-1);
	return (0);
}

static void	*start_dining(void *arg)
{
	t_df	*df;
	t_phil	*philo;

	df = get_df();
	philo = (t_phil *)arg;
	philos_sync_or_spin(df);
	set_int(&df->mtx, &df->threads_running_nbr, INCREASE);
	philos_stagger(philo);
	while (!get_bool(&df->mtx, &df->sim_finished)
		&& get_bool(&philo->mtx, &philo->full) == false)
	{
		set_long(&philo->mtx, &philo->lastmeal_time, get_sim_time(MICRO));
		if (philo_should_exit(df, philo, BOTH))
			break ;
		if (philo_eat(df, philo) < 0)
			break ;
		if (philo_sleep(df, philo) < 0)
			break ;
		if (philo_think(df, philo) < 0)
			break ;
	}
	return (NULL);
}

// Spinlock to synchronize the starting time for all philos
static void	philos_sync_or_spin(t_df *df)
{
	while (!get_bool(&df->mtx, &df->all_threads_ready))
		continue ;
}

/*
Function to stagger the starting time of philos
	- Gives all even-numbered philos a late start by df->tte / 2
	- If total philos is odd, the last odd philo will start 100 usecs later
*/
static int	philos_stagger(t_phil *philo)
{
	t_df	*df;
	long	start_time;

	start_time = get_sim_time(MICRO);
	df = get_df();
	if (df->total_philos > 1 && philo->id % 2 && philo->id == df->total_philos)
	{
		if (log_event_safe(philo, THINKING))
			return (SIM_COMPLETED);
		if (ft_usleep(start_time, 100))
			return (SIM_COMPLETED);
	}
	else if (philo->id % 2 == 0)
	{
		if (log_event_safe(philo, THINKING))
			return (SIM_COMPLETED);
		if (ft_usleep(start_time, df->tte / 2))
			return (SIM_COMPLETED);
	}
	return (0);
}
