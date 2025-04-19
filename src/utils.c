/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:46:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/19 22:49:41 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"


void	clear_out(t_df *df);

/*
Function to clean all mallocs and mutex inits
  - Uses argument 'mode' to indicate whether a certain mutex successfully
	initialized before that point
  - Mode = 0, to only free mallocs
  - Mode = 1, to also destroy the mutex: df->mtx
*/
void	clear_out(t_df *df)
{
	int		i;

	i = -1;
	if (df->philos)
	{
		while (++i < df->total_philos)
			if ((df->philos + i)->mtx_init == true)
				if (pthread_mutex_destroy((df->philos + i)->mtx) < 0)
					print_errstr(ERR_STH);						// Handling TODO
		free(df->philos);
	}
	i = -1;
	if (df->forks)
	{
		while (++i < df->total_philos)
			if ((df->forks + i)->mtx_init == true)
				if (pthread_mutex_destroy((df->forks + i)->mtx) < 0)
					print_errstr(ERR_STH);						// Handling TODO
		free(df->forks);
	}
	if (df->mtx_init == true)
		pthread_mutex_destroy(df->mtx);	
	if (df)
		free(df);
}
