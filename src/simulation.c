/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:46:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/01 19:31:40 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			start_simulation(t_df *df);

static void	*start_dining(void *arg);
// static void	*supervise(void *arg);

// static int	update_philo_deaths(t_df *df);
// static int	update_philo_death(t_df *df, t_phil *philo);

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
	// if (pthread_create(&df->manager, NULL, supervise, df) < 0)
	// 	return (-1);
	
	usleep(2000);
	printf("Printing fork owners now...\n");
	// test_print_fork_owners();					// TPF
	
	i = -1;
	while (++i < df->total_philos)
	{
		if (pthread_join((philos + i)->th_id, NULL) == 0)
			printf(M"%ld\t\t\t%d is joining.\t\tMain thread\n"RST, get_sim_time(MILLI), (philos + i)->id);
		else
			return (-1);
		// if (pthread_join((philos + i)->th_id, NULL) < 0)
		// 	return (-1);
	}
	// printf(M"%ld\t\t\tManager thread is joining.\t\tMain thread\n"RST, get_sim_time(MILLI));
	// if (pthread_join(df->manager, NULL) < 0)
	// 	return (-1);
	return (0);
}

// static void	*start_dining(void *arg)
// {
// 	t_df	*df;
// 	int		i;
	
// 	df = (t_df *)arg;
// 	i = -1;
// 	while (++i < df->total_philos)
// 	{
// 		// if (df->sim_finished == true)
// 		// 	break ;
// 		// pthread_mutex_lock(&df->mtx);
// 		// df->max_meals++;
// 		// pthread_mutex_unlock(&df->mtx);

// 		while ((df->philos + i)->meals_left != 0)
// 		{
// 			pthread_mutex_lock(&((df->philos + i)->mtx));
			
// 			(df->philos + i)->meals_left--;
// 			pthread_mutex_unlock(&((df->philos + i)->mtx));
// 		}
// 	}
	
// 	return (NULL);
// }

static void	*start_dining(void *arg)
{
	t_df	*df;
	t_phil	*philo;
	
	df = get_df();
	philo = (t_phil *)arg;
	
	// while (get_int(&philo->mtx, &philo->meals_left) > 0)
	while (!df->sim_finished)
	{
		philo_eat(df, philo);
		philo_sleep(df, philo);
		philo_think(df, philo);
	}
	

	printf(M"%ld\t\t\t%d is exiting.\n"RST, get_sim_time(MILLI), philo->id);
	return (NULL);
}

/*
Function to execute supervising role using the df-thread manager

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
// 			pthread_mutex_lock(&((df->philos + i)->mtx));
// 			if ((df->philos + i)->dead == true)
// 			{
// 				log_event((df->philos + i), DIED);
// 				df->sim_finished = true;
// 				pthread_mutex_unlock(&((df->philos + i)->mtx));
// 				break ;
// 			}
// 			// if ((df->philos + i)->meals_left == 0)
// 			// 	philos_full++;
// 			if (get_int(&(df->philos + i)->mtx, &(df->philos + i)->meals_left) == 0)
// 				philos_full++;
			
// 			pthread_mutex_unlock(&((df->philos + i)->mtx));
// 		}
// 		if (philos_full == df->total_philos)
// 			df->sim_finished = true;	// neeed to protect these as philos, though they never update this value, they do have to access it for the checks they make during the sim
// 		// if (df->sim_finished == true)	// neeed to protect these as philos, though they never update this value, they do have to access it for the checks they make during the sim
// 		// 	break ;
// 	}
	
// 	printf(M"%ld\t\t\tManager thread is exiting.\n"RST, get_sim_time(MILLI));
// 	return (NULL);
// }

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

// static int	update_philo_death(t_df *df, t_phil *philo)
// {
// 	long	time_without_food;

// 	time_without_food = 0;
// 	if (get_int(&philo->mtx, &philo->meals_left) > 0)
// 	{
// 		time_without_food = get_sim_time(MICRO) - get_long(&philo->mtx,
// 				&philo->lastmeal_time);
// 		if (time_without_food > df->ttd)	// protect? maybe not...
// 			set_bool(&philo->mtx, &philo->dead, true);
// 	}
// 	return (0);
// }
