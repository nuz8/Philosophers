/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:46:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/17 14:18:47 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"


void	clear_out(t_df *df, int mode);

/*
Function to clean all mallocs and mutex inits
  - Uses argument 'mode' to indicate whether a certain mutex successfully
	initialized before that point
  - Mode = 0, to only free mallocs
  - Mode = 1, to also destroy the mutex: df->mtx
*/
void	clear_out(t_df *df, int mode)
{
	int		i;
	t_phil	*philo;
	
	i = -1;
	if (df->philos)
		free(df->philos);
	if (mode == 1)
		pthread_mutex_destroy(df->mtx);
	if (mode == 2)
		while (++i < df->total_philos)
			if (pthread_mutex_destroy((df->philos + i)->mtx) < 0)
				print_errstr(ERR_STH);
		
	if (df)
		free(df);
}
