/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_df.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:13 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 02:01:27 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		init_df(int ac, char **av);
int		init_philos(t_df *df);
int		init_forks(t_df *df);

// static void	tag_forks_v0(t_df * df);
static void	tag_forks(t_phil *philo);

// int	init_df(int ac, char **av, t_df *df)
int	init_df(int ac, char **av)
{
	t_df	*df;

	df = get_df();
	if (parse_arguments(ac, av, df) < 0)
		return (-1);
	if (init_forks(df) < 0)
		return (-1);
	if (init_philos(df) < 0)
		return (-1);
	if (print_mutex_error(INIT, pthread_mutex_init(&df->mtx, NULL)))
		return (-1);
	df->mtx_init = true;
	if (print_mutex_error(INIT, pthread_mutex_init(&df->mtx_write, NULL)))
		return (-1);
	df->mtx_write_init = true;
	return (0);
}

/*
Function to init all forks in the forks array
	- fork_id starts at 0 and onwards...
*/
int	init_forks(t_df *df)
{
	int		i;

	i = -1;
	df->forks = malloc(sizeof(t_fork) * df->total_philos);
	if (!df->forks)
		return (-1);
	while (++i < df->total_philos)
	{
		spawn_fork(df->forks + i);
		(df->forks + i)->id = i;
		if (pthread_mutex_init(&(df->forks + i)->mtx, NULL) < 0)
			return (-1);		
		(df->forks + i)->mtx_init = true;
	}
	return (0);
}

/*
Function to init all the philos in the philos array
	- philo_id starts at 1 and onwards...
*/
int	init_philos(t_df *df)
{
	int		i;

	df->philos = malloc(sizeof(t_phil) * df->total_philos);
	if (!df->philos)
		return (-1);
	i = -1;
	while (++i < df->total_philos)
	{
		spawn_philo(df->philos + i);
		(df->philos + i)->id = i + 1;
		if (pthread_mutex_init(&(df->philos + i)->mtx, NULL) < 0)
			return (-1);
		(df->philos + i)->mtx_init = true;
		tag_forks(df->philos + i);
		(df->philos + i)->meals_left = df->max_meals;
	}
	return (0);
}

/*
Function to tag the forks for the provided philo that it should be able to lock
when it is time to pick up the forks
*/
static void	tag_forks(t_phil *philo)
{
	t_df	*df;
	t_fork	*forks;

	df = get_df();
	forks = df->forks;
	
	// philo->fork1 = forks + (philo->id % df->total_philos);
	// philo->fork2 = forks + ((philo->id - 1) % df->total_philos);
	if (philo->id % 2)
	{
		philo->fork1 = forks + ((philo->id - 1) % df->total_philos);
		philo->fork2 = forks + (philo->id % df->total_philos);
	}
	else
	{
		philo->fork1 = forks + (philo->id % df->total_philos);
		philo->fork2 = forks + ((philo->id - 1) % df->total_philos);
	}
}
