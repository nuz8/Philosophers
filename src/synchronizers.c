/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchronizers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 22:06:55 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/05 22:34:23 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	philos_sync_by_wait(t_df *df);
bool	all_threads_running(t_mutex *mtx, int *threads, int total_philos);

// SPINLOCK	to synchronize philos start
void	philos_sync_by_wait(t_df *df)
{
	while (!get_bool(&df->mtx, &df->all_threads_ready))
		continue ;
}

/*
 * Monitor busy waits 
 * until all threads are running
*/
bool	all_threads_running(t_mutex *mtx, int *threads, int total_philos)
{
	bool	ret;
	
	ret = false;

	print_mutex_error(LOCK, pthread_mutex_lock(mtx));
	if (*threads == total_philos)
		ret = true;
	print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx));
	return (ret);
}
