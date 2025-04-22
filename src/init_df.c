/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_df.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:13 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/22 15:38:21 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		init_df(int ac, char **av);
int		init_philos(t_df *df);
int		init_forks(t_df *df);

// int	init_df(int ac, char **av, t_df *df)
int	init_df(int ac, char **av)
{
	t_df	*df;

	df = get_df();
	if (parse_arguments(ac, av, df) < 0)
		return (-1);

	df->forks = malloc(sizeof(t_fork) * df->total_philos);
	if (!df->forks)
		return (-1);
	if (init_forks(df) < 0)
		return (-1);

	df->philos = malloc(sizeof(t_phil) * df->total_philos);
	if (!df->philos)
		return (-1);
	if (init_philos(df) < 0)
		return (-1);

	if (pthread_mutex_init(&df->mtx, NULL) < 0)			// TODO: error codes
		return (-1);
	df->mtx_init = true;

	df->start_time = get_abs_time(2);
	if (df->start_time < 0)
		return (-1);
	return (0);
}

/*
Function to init all the philos in the philos array
	- philo_id starts at 1 and onwards...
*/
int	init_philos(t_df *df)
{
	int		i;
	t_phil	*philos;

	philos = df->philos;
	i = -1;
	while (++i < df->total_philos)
	{
		spawn_philo(philos + i);
		(philos + i)->id = i + 1;
		if (pthread_mutex_init(&(philos + i)->mtx, NULL) < 0)
			return (-1);
		(philos + i)->mtx_init = true;
		if (i == df->total_philos - 1)
			(philos + i)->last_phil = true;
	}
	return (0);
}

/*
Function to init all forks in the forks array
	- fork_id starts at 0 and onwards...
*/
int	init_forks(t_df *df)
{
	int		i;
	t_fork	*forks;

	i = -1;
	forks = df->forks;
	while (++i < df->total_philos)
	{
		spawn_fork(forks + i);
		(forks + i)->id = i;
		if (pthread_mutex_init(&(forks + i)->mtx, NULL) < 0)
			return (-1);		
		(forks + i)->mtx_init = true;
	}
	return (0);
}
