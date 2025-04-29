/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:46:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/29 15:21:40 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	clear_out(t_df *df);

int		destroy_mutex_safely(t_mutex *mtx);	// to be removed

static void	clear_philos(t_df *df);
static void	clear_forks(t_df *df);
// static int	destroy_mutex_safely(t_mutex *mtx);	to be uncommented in place of above to-be-removed fn

/*
Function to clean all mallocs and mutex inits
*/
void	clear_out(t_df *df)
{
	if (df->forks)
		clear_forks(df);
	if (df->philos)
		clear_philos(df);
	// if (df->mtx_init == true)
	// 	print_mutex_error(DESTROY, pthread_mutex_destroy(&df->mtx));	// Old implementation
	if (df->mtx_init == true)
		destroy_mutex_safely(&df->mtx);
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
				destroy_mutex_safely(&(df->philos + i)->mtx);
				// print_mutex_error(DESTROY,
				// 	pthread_mutex_destroy(&(df->philos + i)->mtx));	// Old implementation
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
				destroy_mutex_safely(&(df->forks + i)->mtx);
				// print_mutex_error(DESTROY,
				// 	pthread_mutex_destroy(&(df->forks + i)->mtx));	// Old implementation
		free(df->forks);
		df->forks = NULL;
	}
}

/*
Function for safe mutex_destroy
  - Handles the EBUSY error when trying to destroy a mutex that is locked
  - If a mutex is locked, first unlocks it and then destroys
  - Prints any other mutex errors, including that coming from mutex_unlock
    when unlock fails
  - Returns 0 if successful
  - Returns -1 if something fails and error message is displayed
*/
// static int	destroy_mutex_safely(t_mutex *mtx)
int	destroy_mutex_safely(t_mutex *mtx)
{
	int	ret;
	
	ret = pthread_mutex_destroy(mtx);
	if (ret == 0)
		return (0);
	else if (ret == EBUSY)
	{
		print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx));
		ret = pthread_mutex_destroy(mtx);
		if (ret != 0)
			if (print_mutex_error(UNLOCK, ret) != 0)
				return (-1);
	}
	else
		if (print_mutex_error(DESTROY, ret) != 0)
			return (-1);
	return (0);
}
