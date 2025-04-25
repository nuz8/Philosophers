/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:46:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 13:51:48 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	clear_out(t_df *df);

static void	clear_philos(t_df *df);
static void	clear_forks(t_df *df);

/*
Function to clean all mallocs and mutex inits
*/
void	clear_out(t_df *df)
{
	if (df->forks)
		clear_forks(df);
	if (df->philos)
		clear_philos(df);
	if (df->mtx_init == true)
		print_mutex_error(DESTROY, pthread_mutex_destroy(&df->mtx));	
	if (df)
		free(df);
}

/*
Function to clean all t_phil instances (mallocs and mutexes)
	- Checks to see if philos pointer has allocation with a null check
	- Checks if the mutex associated with each philo was initiated using the
	  the bool mtx_init
	- If mtx_init is true then destroys the mutex associated with this philo,
	  and carries on the checks for each element in the philos array
	- Finally, when all mutexes are destroyed, the philos allocation is free'd
*/
static void	clear_philos(t_df *df)
{
	int		i;

	i = -1;
	if (df->philos)
	{
		while (++i < df->total_philos)
			if ((df->philos + i)->mtx_init == true)
				print_mutex_error(DESTROY,
					pthread_mutex_destroy(&(df->philos + i)->mtx));
		free(df->philos);
		df->philos = NULL;
	}
}

/*
Function to clean all t_forks instances (mallocs and mutexes)
	- Checks to see if forks pointer has allocation with a null check
	- Checks if the mutex associated with each fork was initiated using the
	  the bool mtx_init
	- If mtx_init is true then destroys the mutex associated with this fork,
	  and carries on the checks for each element in the forks array
	- Finally, when all mutexes are destroyed, the philos allocation is free'd
*/
static void	clear_forks(t_df *df)
{
	int		i;

	i = -1;
	if (df->forks)
	{
		while (++i < df->total_philos)
			if ((df->forks + i)->mtx_init == true)
				print_mutex_error(DESTROY,
					pthread_mutex_destroy(&(df->forks + i)->mtx));
		free(df->forks);
		df->forks = NULL;
	}
}
