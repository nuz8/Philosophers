/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:46:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/05 22:37:05 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			start_simulation(t_df *df);

static void	*start_dining(void *arg);
// static void	switch_turns(t_df *df, t_phil *philo);
static void	*supervise(void *arg);
static int	update_philo_death(t_df *df, t_phil *philo);
// static int	update_philo_deaths(t_df *df);
static void	philos_stagger(t_phil *philo);


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
		// (philos + i)->lastmeal_time = 0;	// Protect?
		
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
		if (pthread_join((philos + i)->th_id, NULL) == 0)
			printf(M"%ld\t\t\t\tPhilo %d joined.\t\t(Main thread)\n"RST, get_sim_time(MILLI), (philos + i)->id);
		else
			return (-1);
		// if (pthread_join((philos + i)->th_id, NULL) < 0)
		// 	return (-1);
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

	
	// if (df->total_philos % 2 == 1 && philo->id % 2 == 0)
	// 	usleep(500);

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
		// switch_turns(df, philo);
		if (philo_sleep(df, philo) < 0)
			break ;
		if (philo_think(df, philo, false) < 0)
			break ;
	}
	// printf(M"%ld\t\t\t%d is exiting.\n"RST, get_sim_time(MILLI), philo->id);
	return (NULL);
}

// static void	switch_turns(t_df *df, t_phil *philo)
// {
// 	if (philo->id % 2 == 0)
// 	{
// 		if (get_int(&df->mtx_turn, &df->turn) != ODD_PHILOS)
// 			set_int(&df->mtx_turn, &df->turn, ODD_PHILOS);
// 	}
// 	else
// 	{
// 		if (get_int(&df->mtx_turn, &df->turn) != EVEN_PHILOS)
// 			set_int(&df->mtx_turn, &df->turn, EVEN_PHILOS);
// 	}
// }

/*
Function to execute supervising role using the df-thread 'manager'

Possible data-races info:
	Accessed philo-fields: philos->	(dead, meals_left)
	Accessed df-fields:		df->	(total_philos, max_meals, sim_finished)
*/
// static void	*supervise(void *arg)
// {
// 	t_df	*df;
// 	int		philos_full;
// 	int		i;
	
// 	df = (t_df *)arg;
// 	while (!df->sim_finished)
// 	{
// 		philos_full = 0;
// 		i = -1;
// 		while (++i < df->total_philos)
// 		{
// 			update_philo_death(df, df->philos + i);
// 			if ((df->philos + i)->dead == true)
// 			{
// 				log_event_unsafe((df->philos + i), DIED);
// 				df->sim_finished = true;
// 				break ;
// 			}
// 			if ((df->philos + i)->meals_left == 0)
// 				philos_full++;
// 		}
// 		if (philos_full == df->total_philos)
// 		{
// 			df->sim_finished = true;	// neeed to protect these as philos, though they never update this value, they do have to access it for the checks they make during the sim
// 			printf("Sim finished\n");
// 		}
// 	}	
// 	printf(M"%ld\t\t\tManager thread is exiting.\n"RST, get_sim_time(MILLI));
// 	return (NULL);
// }

/*
Supervise routine using mutexes
*/
static void	*supervise(void *arg)
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
				return (NULL) ;
			}
			if (get_int(&(df->philos + i)->mtx, &(df->philos + i)->meals_left) == 0)
				philos_full++;
		}
		if (philos_full == df->total_philos)
		{
			set_bool(&df->mtx, &df->sim_finished, true);
			printf("Exiting by all philos being full.\n");
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
	// if (get_int(&philo->mtx, &philo->meals_left) > 0)
	// {
	// 	time_without_food = get_sim_time(MICRO) - get_long(&philo->mtx,
	// 			&philo->lastmeal_time);
	// 	if (get_bool(&philo->mtx, &philo->full) == false && time_without_food > df->ttd)	// protect? maybe not...
	// 		set_bool(&philo->mtx, &philo->dead, true);
	// }
	
	time_without_food = get_sim_time(MICRO) - get_long(&philo->mtx,
			&philo->lastmeal_time);
	if (get_bool(&philo->mtx, &philo->full) == false && time_without_food > df->ttd)	// protect? maybe not...
		set_bool(&philo->mtx, &philo->dead, true);
		
	return (0);
}

// static int	update_philo_deaths(t_df *df)
// {
// 	t_phil	*philos;
// 	long	time_without_food;
// 	int		i;

// 	philos = df->philos;
// 	time_without_food = 0;
// 	i = -1;
// 	while (++i < df->total_philos)
// 	{
// 		if (get_int(&(philos + i)->mtx, &(philos + i)->meals_left) > 0)
// 		{
// 			time_without_food = get_sim_time(MICRO) -
// 					get_long(&(philos + i)->mtx, &(philos + i)->lastmeal_time);
// 			if (time_without_food > df->ttd)	// protect? maybe not...
// 				set_bool(&(philos + i)->mtx, &(philos + i)->dead, true);
// 		}
// 	}
// 	return (0);
// }

static void	philos_stagger(t_phil *philo)
{
	t_df	*df;
	long	start_time;

	start_time = get_sim_time(MICRO);
	df = get_df();
	if (df->total_philos % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(start_time, 3e4);
	}
	else
	{
		if (philo->id %2)
		philo_think(df, philo, true);
	}
}
