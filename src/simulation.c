/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:46:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 02:24:27 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			start_simulation(t_df *df);

static void	*start_dining(void *arg);
static void	philos_sync_by_wait(t_df *df);
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
		if (pthread_create(&(philos + i)->th_id, NULL, start_dining,
				(philos + i)) < 0)
			return (-1);
	}
	if (pthread_create(&df->manager, NULL, supervise, df) < 0)
		return (-1);

	df->start_time = get_abs_time(MICRO);
	set_bool(&df->mtx, &df->all_threads_ready, true);
	
	i = -1;
	while (++i < df->total_philos)
	{
		// if (pthread_join((philos + i)->th_id, NULL) == 0)
		// 	printf(M"%ld\t\t\t\tPhilo %d joined.\t\t(Main thread)\n"RST, get_sim_time(MILLI), (philos + i)->id);
		// else
		// 	return (-1);
		if (pthread_join((philos + i)->th_id, NULL) < 0)
			return (-1);
	}
	if (pthread_join(df->manager, NULL) == 0)
		printf(M"%ld\t\t\t\tManager thread joined.\t(Main thread)\n"RST, get_sim_time(MILLI));
	else
		return (-1);
	// if (pthread_join(df->manager, NULL) < 0)
	// 	return (-1);
	return (0);
}

static void	*start_dining(void *arg)
{
	t_df	*df;
	t_phil	*philo;
	
	df = get_df();
	philo = (t_phil *)arg;

	philos_sync_by_wait(df);
	set_long(&philo->mtx, &philo->lastmeal_time, get_sim_time(MICRO));
	set_int(&df->mtx, &df->threads_running_nbr, INCREASE);
	philos_stagger(philo);
	
	while (!get_bool(&df->mtx, &df->sim_finished) && get_bool(&philo->mtx, &philo->full) == false)
	{
		if (philo_should_exit(df, philo, BOTH))
			break ;
		if (philo_eat(df, philo) < 0)
			break ;
		if (philo_sleep(df, philo) < 0)
			break ;
		if (philo_think(df, philo, false) < 0)
			break ;
	}
	// printf(M"%ld\t\t\t%d is exiting.\n"RST, get_sim_time(MILLI), philo->id);
	return (NULL);
}

// Spinlock to synchronize the starting time for all philos
static void	philos_sync_by_wait(t_df *df)
{
	while (!get_bool(&df->mtx, &df->all_threads_ready))
		continue ;
}

// static int	philos_stagger(t_phil *philo)
// {
// 	t_df	*df;
// 	long	start_time;

// 	// start_time = get_abs_time(MICRO);
// 	start_time = get_sim_time(MICRO);
// 	df = get_df();
// 	if (df->total_philos % 2 == 0)
// 	{
// 		if (philo->id % 2 == 0)
// 		{
// 			printf("\t\t\tPhilo %d will now start stagger sleeping here\n", philo->id);
// 			ft_usleep(start_time, 100);
// 			printf("\t\t\tPhilo %d just woke up from initial stagger sleep\n", philo->id);
// 		}
// 	}
// 	else
// 	{
// 		if (philo->id % 2)
// 		philo_think(df, philo, true);
// 	}
// }

static int	philos_stagger(t_phil *philo)
{
	t_df	*df;
	long	start_time;

	start_time = get_sim_time(MICRO);
	df = get_df();
	if (philo->id % 2 == 0)
	{
		if (log_event_safe(philo, THINKING))
			return (SIM_COMPLETED);
		if (ft_usleep(start_time, df->tte / 2))
			return (SIM_COMPLETED);
	}
	return (0);
}


// int	start_with_thinking(t_data *data, t_philo *philo)
// {
// 	unsigned long	start_time;

// 	start_time = get_time();
// 	if (try_print(data, philo, "is thinking", start_time))
// 		return (1);
// 	if (ft_sleep(start_time, philo->time_to_eat / 2, data))
// 		return (1);
// 	return (0);
// }
