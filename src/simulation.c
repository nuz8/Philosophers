/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:46:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/23 12:19:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			start_simulation(t_df *df);
static void	*start_dining(void *arg);



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
		if (pthread_create(&(philos + i)->th_id, NULL, start_dining, df) < 0)
			return (-1);
	}
	
	i = -1;
	while (++i < df->total_philos)
		if (pthread_join((philos + i)->th_id, NULL) < 0)
			return (-1);
	return (0);
}

static void	*start_dining(void *arg)
{
	t_df	*df;
	int		i;
	
	df = (t_df *)arg;
	i = -1;
	while (++i < 1000000)
	{
		pthread_mutex_lock(&df->mtx);
		(df->max_meals)++;
		pthread_mutex_unlock(&df->mtx);
	}
	return (NULL);
}
